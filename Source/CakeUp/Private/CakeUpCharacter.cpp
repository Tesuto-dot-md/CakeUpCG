#include "CakeUpCharacter.h"
#include "CarryableCake.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

ACakeUpCharacter::ACakeUpCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    // Движение мягче
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 240.f, 0.f); // было 360 — убрал дёрганье на диагонали
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MaxAcceleration = 2048.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
    GetCharacterMovement()->BrakingFriction = 4.f;

    // Прыжок
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.55f;

    // Камера: фиксированный top-down, без наследования поворотов
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = CameraArmLength;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bDoCollisionTest = true;
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw   = false;
    CameraBoom->bInheritRoll  = false;
    CameraBoom->SetRelativeRotation(FRotator(-CameraPitchDegrees, 0.f, 0.f));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Триггер для взаимодействий
    InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
    InteractSphere->SetupAttachment(RootComponent);
    InteractSphere->SetSphereRadius(160.f);
    InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    InteractSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    InteractSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ACakeUpCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (InteractSphere)
    {
        InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ACakeUpCharacter::OnInteractBegin);
        InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ACakeUpCharacter::OnInteractEnd);
    }

    if (CameraBoom)
    {
        CameraBoom->bUsePawnControlRotation = false;
        CameraBoom->bInheritPitch = false;
        CameraBoom->bInheritYaw   = false;
        CameraBoom->bInheritRoll  = false;
        CameraBoom->SetRelativeRotation(FRotator(-CameraPitchDegrees, 0.f, 0.f));
        CameraBoom->TargetArmLength = CameraArmLength;
    }
}

void ACakeUpCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    FaceCursorWhenIdle(DeltaSeconds);
}

void ACakeUpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACakeUpCharacter::HandleMove);
            EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACakeUpCharacter::HandleMove);
        }
        if (LookAction)
        {
            // Камеру не крутим — просто оставим, если нужно слушать курсор (но логика — в Tick)
            EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACakeUpCharacter::HandleLook);
        }
        if (InteractAction)
        {
            EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ACakeUpCharacter::HandleInteract);
        }
        if (JumpAction)
        {
            EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }
    }
}

void ACakeUpCharacter::HandleMove(const FInputActionValue& Value)
{
    const FVector2D Input = Value.Get<FVector2D>();
    if (!Controller) return;

    // Направления строго относительно камеры
    const FRotator CamRot = FollowCamera ? FollowCamera->GetComponentRotation() : FRotator::ZeroRotator;
    const FRotator YawRot(0.f, CamRot.Yaw, 0.f);

    const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector RightDir   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDir, Input.Y);
    AddMovementInput(RightDir,   Input.X);
}

void ACakeUpCharacter::HandleLook(const FInputActionValue& /*Value*/)
{
    // Специально пусто — камера не вращается, взгляд к курсору делаем в Tick()
}

void ACakeUpCharacter::HandleInteract(const FInputActionValue& /*Value*/)
{
    if (ACarryableCake* Cake = LastNearbyCake.Get())
    {
        Server_RequestToggleCarry(Cake);
    }
}

void ACakeUpCharacter::Server_RequestToggleCarry_Implementation(ACarryableCake* Cake)
{
    if (IsValid(Cake))
    {
        // серверная логика на актёре торта
        Cake->TryToggleCarryFromServer(this);
    }
}

void ACakeUpCharacter::OnInteractBegin(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor,
                                       UPrimitiveComponent* /*OtherComp*/, int32 /*OtherBodyIndex*/, bool /*bFromSweep*/,
                                       const FHitResult& /*SweepResult*/)
{
    if (ACarryableCake* Cake = Cast<ACarryableCake>(OtherActor))
    {
        LastNearbyCake = Cake;
    }
}

void ACakeUpCharacter::OnInteractEnd(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor,
                                     UPrimitiveComponent* /*OtherComp*/, int32 /*OtherBodyIndex*/)
{
    if (OtherActor == LastNearbyCake.Get())
    {
        LastNearbyCake = nullptr;
    }
}

void ACakeUpCharacter::FaceCursorWhenIdle(float DeltaSeconds)
{
    if (!IsLocallyControlled()) return;

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
        {
            // Локальная мышь только у первого игрока
            if (LP->GetLocalPlayerIndex() > 0)
            {
                return;
            }
        }
    }

    const float Speed2D = GetVelocity().Size2D();
    if (Speed2D >= MinSpeedToUseMoveFacing) return;

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        FHitResult Hit;
        if (PC->GetHitResultUnderCursor(ECC_Visibility, true, Hit))
        {
            FVector To = Hit.ImpactPoint - GetActorLocation();
            To.Z = 0.f;
            if (To.SizeSquared() > 4.f)
            {
                const float TargetYaw = To.Rotation().Yaw;
                const FRotator TargetRot(0.f, TargetYaw, 0.f);
                const FRotator NewRot = FMath::RInterpTo(
                    GetActorRotation(),
                    TargetRot,
                    DeltaSeconds,
                    IdleFaceInterpSpeed);

                // Мгновенно крутимся локально — чтобы игроку было приятно
                SetActorRotation(NewRot);

                // И сообщаем серверу, чтобы ВСЕ видели примерно тот же поворот
                Server_SetIdleFaceRotation(NewRot.Yaw);
            }
        }
    }
}


void ACakeUpCharacter::Server_SetIdleFaceRotation_Implementation(float Yaw)
{
    // На сервере тоже не крутимся, если персонаж бежит
    const float Speed2D = GetVelocity().Size2D();
    if (Speed2D >= MinSpeedToUseMoveFacing)
    {
        return;
    }

    const FRotator TargetRot(0.f, Yaw, 0.f);
    SetActorRotation(TargetRot);
}
