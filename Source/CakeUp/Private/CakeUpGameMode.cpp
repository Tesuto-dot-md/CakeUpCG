#include "CakeUpGameMode.h"
#include "CakeUpCharacter.h"
#include "CakeUpPlayerController.h"
#include "CakeUpGameState.h"
#include "CakeUpInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

ACakeUpGameMode::ACakeUpGameMode()
{
    DefaultPawnClass      = ACakeUpCharacter::StaticClass();
    PlayerControllerClass = ACakeUpPlayerController::StaticClass();
    GameStateClass        = ACakeUpGameState::StaticClass();
}

void ACakeUpGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World) return;

    if (World->GetNetMode() == NM_DedicatedServer)
    {
        return;
    }

    UCakeUpInstance* CU_GI = World->GetGameInstance<UCakeUpInstance>();
    int32 TargetLocalPlayers = 1;

    if (CU_GI)
    {
        TargetLocalPlayers = FMath::Max(CU_GI->DesiredLocalPlayers, 1);
    }

    UGameInstance* GI = World->GetGameInstance();
    if (!GI) return;

    int32 NumLocalPlayers = GI->GetNumLocalPlayers();

    while (NumLocalPlayers < TargetLocalPlayers)
    {
        UGameplayStatics::CreatePlayer(World, NumLocalPlayers, true);
        ++NumLocalPlayers;
    }
}
