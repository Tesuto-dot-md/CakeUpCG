#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarryableCake.generated.h"

class UStaticMeshComponent;
class ACakeUpCharacter;

UENUM(BlueprintType)
enum class ECakeState : uint8
{
    Free     UMETA(DisplayName="Free"),
    Carried  UMETA(DisplayName="Carried")
};

UCLASS()
class CAKEUP_API ACarryableCake : public AActor
{
    GENERATED_BODY()

public:
    ACarryableCake();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Вызываешь с клиента/локального игрока: торт сам запросит у сервера взять/бросить */
    UFUNCTION(BlueprintCallable, Category="Cake")
    void RequestToggleCarry(ACakeUpCharacter* ByCharacter);

    /** Опционально: пнуть торт (на клиенте, уйдёт на сервер) */
    UFUNCTION(BlueprintCallable, Category="Cake")
    void RequestImpulse(const FVector& Impulse, bool bVelChange = true);

protected:
    /** Основной меш торта */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cake")
    TObjectPtr<UStaticMeshComponent> Mesh;

    /** Текущее состояние торта */
    UPROPERTY(ReplicatedUsing=OnRep_State, BlueprintReadOnly, Category="Cake")
    ECakeState State = ECakeState::Free;

    /** Кто сейчас несёт торт (может быть nullptr) */
    UPROPERTY(ReplicatedUsing=OnRep_Carrier, BlueprintReadOnly, Category="Cake")
    ACakeUpCharacter* Carrier = nullptr;

    /** Имя сокета на скелете персонажа для крепления торта */
    UPROPERTY(EditDefaultsOnly, Category="Cake")
    FName AttachSocketName = TEXT("CakeSocket");

    /** Частота обновления по сети для свободного торта */
    UPROPERTY(EditDefaultsOnly, Category="Network")
    float NetHz = 30.f;

    /** --- RPC --- */

    UFUNCTION(Server, Reliable)
    void Server_ToggleCarry(ACakeUpCharacter* ByCharacter);
    void Server_ToggleCarry_Implementation(ACakeUpCharacter* ByCharacter);

    UFUNCTION(Server, Reliable)
    void Server_AddImpulse(const FVector& Impulse, bool bVelChange);
    void Server_AddImpulse_Implementation(const FVector& Impulse, bool bVelChange);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_Wake();
    void Multicast_Wake_Implementation();

    /** --- OnRep --- */

    UFUNCTION()
    void OnRep_State();

    UFUNCTION()
    void OnRep_Carrier();

    /** --- Внутренняя логика --- */

    void ApplyState_Server(); // сервер: физика/репликация
    void ApplyState_Client(); // клиент: только визуал/attach

    void AttachToCarrier();
    void DetachFromCarrier();
};
