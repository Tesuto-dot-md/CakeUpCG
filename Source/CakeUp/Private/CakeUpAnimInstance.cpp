#include "CakeUpAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
// ВАЖНО: нужная библиотека для CalculateDirection
#include "KismetAnimationLibrary.h"

void UCakeUpAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    OwningPawn = TryGetPawnOwner();
}

void UCakeUpAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwningPawn)
    {
        OwningPawn = TryGetPawnOwner();
    }
    if (!OwningPawn) return;

    const FVector Vel = OwningPawn->GetVelocity();
    Speed = Vel.Size2D();
    bIsMoving = Speed > 3.f;

    if (const ACharacter* Char = Cast<ACharacter>(OwningPawn))
    {
        if (const UCharacterMovementComponent* Move = Char->GetCharacterMovement())
        {
            bIsInAir = Move->IsFalling();
        }
        else
        {
            bIsInAir = false;
        }

        // БЫЛО: UKismetMathLibrary::CalculateDirection — не существует для этого кейса
        // СТАЛО: UKismetAnimationLibrary::CalculateDirection
        Direction = UKismetAnimationLibrary::CalculateDirection(Vel, Char->GetActorRotation());
    }
    else
    {
        bIsInAir = false;
        Direction = 0.f;
    }
}
