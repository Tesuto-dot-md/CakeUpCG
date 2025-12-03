#include "CakeUpInstance.h"   // ← первым

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UCakeUpInstance::StartSoloGame()
{
    DesiredLocalPlayers = 1;

    if (UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevel(World, GameplayMapName);
    }
}

void UCakeUpInstance::StartLocalCoopGame()
{
    DesiredLocalPlayers = 2;

    if (UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevel(World, GameplayMapName);
    }
}

void UCakeUpInstance::HostOnlineGame()
{
    DesiredLocalPlayers = 1; // онлайн: по одному локальному игроку на машину

    if (UWorld* World = GetWorld())
    {
        FString Options = GameplayMapName.ToString() + TEXT("?listen");
        UGameplayStatics::OpenLevel(World, FName(*Options), true);
    }
}

void UCakeUpInstance::JoinOnlineGame(const FString& Address)
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
        {
            PC->ClientTravel(Address, TRAVEL_Absolute);
        }
    }
}
