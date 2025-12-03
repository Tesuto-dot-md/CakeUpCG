#include "CoopSharedCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"

ACoopSharedCamera::ACoopSharedCamera()
{
    PrimaryActorTick.bCanEverTick = true;
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SetRootComponent(SpringArm);
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bDoCollisionTest = true;
    SpringArm->TargetArmLength = MinArm;
    SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ACoopSharedCamera::BeginPlay()
{
    Super::BeginPlay();
}

void ACoopSharedCamera::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn* P0 = nullptr;
    APawn* P1 = nullptr;

    for (TActorIterator<APawn> It(GetWorld()); It; ++It)
    {
        APawn* P = *It;
        if (P->GetController() && P->GetController()->IsPlayerController())
        {
            if (!P0) P0 = P;
            else if (!P1) { P1 = P; break; }
        }
    }

    if (!P0)
        return;

    FVector TargetLoc = P0->GetActorLocation();
    float targetArm = MinArm;

    if (P1)
    {
        const FVector A = P0->GetActorLocation();
        const FVector B = P1->GetActorLocation();
        const FVector Mid = (A + B) * 0.5f;
        const float Dist = FVector::Distance(A, B);

        TargetLoc = Mid;
        targetArm = FMath::Clamp(MinArm + Dist * ZoomPerMeter, MinArm, MaxArm);
    }

    TargetLoc.Z += Height;

    const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaSeconds, LerpSpeed);
    SetActorLocation(NewLoc);
    SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, targetArm, DeltaSeconds, LerpSpeed);
}
