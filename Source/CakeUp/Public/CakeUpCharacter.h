#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CakeUpCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class UInputAction;
class ACarryableCake;

UCLASS()
class CAKEUP_API ACakeUpCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACakeUpCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
    TObjectPtr<USphereComponent> InteractSphere;

    // Input actions (assign in BP or defaults)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> MoveAction;

    // Не используем для поворота камеры — только чтобы «пинговать» обновление курсора при нужде
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> JumpAction;

    // Camera settings (fixed top-down)
    UPROPERTY(EditAnywhere, Category="Camera")
    float CameraPitchDegrees = 60.f;

    UPROPERTY(EditAnywhere, Category="Camera")
    float CameraArmLength = 600.f;

    UFUNCTION(Server, Unreliable)
    void Server_SetIdleFaceRotation(float Yaw);

    // Facing settings
    UPROPERTY(EditAnywhere, Category="Facing")
    float IdleFaceInterpSpeed = 8.f;

    UPROPERTY(EditAnywhere, Category="Facing")
    float MinSpeedToUseMoveFacing = 6.f;

    // Nearby cake reference (owner-only)
    UPROPERTY(Transient)
    TObjectPtr<ACarryableCake> LastNearbyCake;

    // Input handlers
    void HandleMove(const struct FInputActionValue& Value);
    void HandleLook(const struct FInputActionValue& Value);
    void HandleInteract(const struct FInputActionValue& Value);

    UFUNCTION(Server, Reliable)
    void Server_RequestToggleCarry(ACarryableCake* Cake);

    // Overlap handlers
    UFUNCTION()
    void OnInteractBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                         const FHitResult& SweepResult);

    UFUNCTION()
    void OnInteractEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Поворот к курсору, когда стоим
    void FaceCursorWhenIdle(float DeltaSeconds);
};
