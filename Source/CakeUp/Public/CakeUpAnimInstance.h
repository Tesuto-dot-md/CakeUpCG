#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CakeUpAnimInstance.generated.h"

UCLASS(Blueprintable)
class CAKEUP_API UCakeUpAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(BlueprintReadOnly, Category="Anim", meta=(AllowPrivateAccess="true"))
    TObjectPtr<APawn> OwningPawn = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="Anim", meta=(AllowPrivateAccess="true"))
    float Speed = 0.f;

    UPROPERTY(BlueprintReadOnly, Category="Anim", meta=(AllowPrivateAccess="true"))
    float Direction = 0.f; // -180..180

    UPROPERTY(BlueprintReadOnly, Category="Anim", meta=(AllowPrivateAccess="true"))
    bool bIsInAir = false;

    UPROPERTY(BlueprintReadOnly, Category="Anim", meta=(AllowPrivateAccess="true"))
    bool bIsMoving = false;
};
