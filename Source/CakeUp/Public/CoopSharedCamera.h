#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopSharedCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CAKEUP_API ACoopSharedCamera : public AActor
{
    GENERATED_BODY()

public:
    ACoopSharedCamera();

    virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(EditAnywhere, Category="Camera")
    float MinArm = 400.f;

    UPROPERTY(EditAnywhere, Category="Camera")
    float MaxArm = 1200.f;

    UPROPERTY(EditAnywhere, Category="Camera")
    float ZoomPerMeter = 6.f;

    UPROPERTY(EditAnywhere, Category="Camera")
    float Height = 300.f;

    UPROPERTY(EditAnywhere, Category="Camera")
    float LerpSpeed = 6.f;
};
