#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CakeUpPlayerController.generated.h"   // ← последний include, очень важно

class UInputMappingContext;

UCLASS()
class CAKEUP_API ACakeUpPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACakeUpPlayerController();

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnRep_PlayerState() override;

protected:
    // IMC для первого локального игрока (WASD и т.п.)
    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputMappingContext> DefaultInputContext;

    // IMC для второго локального игрока (стрелки и т.п.)
    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputMappingContext> SecondPlayerInputContext;

    void ApplyDefaultMapping();
};
