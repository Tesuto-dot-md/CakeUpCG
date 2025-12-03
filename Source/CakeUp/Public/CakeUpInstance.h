#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CakeUpInstance.generated.h"

UCLASS()
class CAKEUP_API UCakeUpInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Сколько локальных игроков хотим на карте с геймплеем
    UPROPERTY(BlueprintReadWrite, Category="CakeUp|Session")
    int32 DesiredLocalPlayers = 1;

    // Имя карты с геймплеем (подставь свою)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CakeUp|Session")
    FName GameplayMapName = TEXT("CakeUpMap");

    UFUNCTION(BlueprintCallable, Category="CakeUp|Session")
    void StartSoloGame();

    UFUNCTION(BlueprintCallable, Category="CakeUp|Session")
    void StartLocalCoopGame();

    UFUNCTION(BlueprintCallable, Category="CakeUp|Session")
    void HostOnlineGame();

    UFUNCTION(BlueprintCallable, Category="CakeUp|Session")
    void JoinOnlineGame(const FString& Address);
};
