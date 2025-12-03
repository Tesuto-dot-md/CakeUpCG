#include "CarryableCake.h"

#include "CakeUpCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

ACarryableCake::ACarryableCake()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    bAlwaysRelevant = true;
    SetReplicateMovement(true);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    // Движение актора реплицируем, сам компонент не обязан
    Mesh->SetIsReplicated(false);
    Mesh->SetSimulatePhysics(false);
    Mesh->SetEnableGravity(true);

    SetNetUpdateFrequency(NetHz);
    SetMinNetUpdateFrequency(FMath::Min(NetHz, 10.f));
}

void ACarryableCake::BeginPlay()
{
    Super::BeginPlay();

    // Первичное применение состояния
    if (HasAuthority())
    {
        // Сервер решает физику
        ApplyState_Server();
    }
    else
    {
        // Клиенты НИКОГДА не симулируют, только визуал
        Mesh->SetSimulatePhysics(false);
    }

    ApplyState_Client();
}

void ACarryableCake::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Жёсткий анти-асинхрон: если вдруг где-то на клиенте включили физику – вырубаем
    if (!HasAuthority() && Mesh && Mesh->IsSimulatingPhysics())
    {
        UE_LOG(LogTemp, Error,
               TEXT("[Cake] CLIENT %s симулирует физику, отключаем"),
               *GetName());
        Mesh->SetSimulatePhysics(false);
    }
}

void ACarryableCake::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACarryableCake, State);
    DOREPLIFETIME(ACarryableCake, Carrier);
}

/* =====================  Публичные API  ===================== */

void ACarryableCake::RequestToggleCarry(ACakeUpCharacter* ByCharacter)
{
    if (!ByCharacter)
        return;

    // Всегда идём через сервер
    if (HasAuthority())
    {
        Server_ToggleCarry(ByCharacter);
    }
    else
    {
        Server_ToggleCarry(ByCharacter);
    }
}

void ACarryableCake::RequestImpulse(const FVector& Impulse, bool bVelChange)
{
    if (HasAuthority())
    {
        Server_AddImpulse(Impulse, bVelChange);
    }
    else
    {
        Server_AddImpulse(Impulse, bVelChange);
    }
}

/* =====================  RPC  ===================== */

void ACarryableCake::Server_ToggleCarry_Implementation(ACakeUpCharacter* ByCharacter)
{
    if (!ByCharacter)
        return;

    // Если торт свободен – берём в руки
    if (State == ECakeState::Free)
    {
        State = ECakeState::Carried;
        Carrier = ByCharacter;
    }
    else // уже несут – бросаем
    {
        State = ECakeState::Free;
        Carrier = nullptr;
    }

    ApplyState_Server();
    ApplyState_Client();     // применим на сервере
    ForceNetUpdate();
}

void ACarryableCake::Server_AddImpulse_Implementation(const FVector& Impulse, bool bVelChange)
{
    if (State != ECakeState::Free)
    {
        // Нельзя пнуть, пока несут
        return;
    }

    if (Mesh && Mesh->IsSimulatingPhysics())
    {
        Mesh->AddImpulse(Impulse, NAME_None, bVelChange);
        ForceNetUpdate();
        Multicast_Wake();
    }
}

void ACarryableCake::Multicast_Wake_Implementation()
{
    if (Mesh)
    {
        Mesh->WakeRigidBody();
    }
}

/* =====================  OnRep  ===================== */

void ACarryableCake::OnRep_State()
{
    // На клиентах: только визуал / attach
    ApplyState_Client();
}

void ACarryableCake::OnRep_Carrier()
{
    // На клиентах: только визуал / attach
    ApplyState_Client();
}

/* =====================  Логика состояний  ===================== */

void ACarryableCake::ApplyState_Server()
{
    if (!HasAuthority() || !Mesh)
        return;

    switch (State)
    {
    case ECakeState::Free:
        {
            DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

            Mesh->SetSimulatePhysics(true);
            Mesh->SetEnableGravity(true);

            SetReplicateMovement(true);
            break;
        }
    case ECakeState::Carried:
        {
            Mesh->SetSimulatePhysics(false);
            Mesh->SetEnableGravity(false);

            SetReplicateMovement(false); // движение идёт от родителя (персонажа)

            AttachToCarrier();
            break;
        }
    default:
        break;
    }
}

void ACarryableCake::ApplyState_Client()
{
    if (!Mesh)
        return;

    // Клиенты никогда не симулируют
    Mesh->SetSimulatePhysics(false);

    switch (State)
    {
    case ECakeState::Free:
        {
            DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            // Движение придёт через ReplicateMovement, ничего больше не делаем
            break;
        }
    case ECakeState::Carried:
        {
            AttachToCarrier();
            break;
        }
    default:
        break;
    }
}

void ACarryableCake::AttachToCarrier()
{
    if (!Carrier)
        return;

    USkeletalMeshComponent* Skel = Carrier->GetMesh();
    if (!Skel)
        return;

    AttachToComponent(
        Skel,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        AttachSocketName
    );
}

void ACarryableCake::DetachFromCarrier()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
