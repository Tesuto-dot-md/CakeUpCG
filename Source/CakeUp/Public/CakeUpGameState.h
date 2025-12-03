#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CakeUpGameState.generated.h"

UCLASS()
class CAKEUP_API ACakeUpGameState : public AGameStateBase
{
    GENERATED_BODY()
public:
    ACakeUpGameState();

    UPROPERTY(ReplicatedUsing=OnRep_PickupEnabled, BlueprintReadOnly, Category="Rules")
    bool bPickupEnabled;

    UFUNCTION()
    void OnRep_PickupEnabled();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};