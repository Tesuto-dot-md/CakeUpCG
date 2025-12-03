#include "CakeUpPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

ACakeUpPlayerController::ACakeUpPlayerController()
{
    bReplicates = true;
    bShowMouseCursor = true;
}

void ACakeUpPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    ApplyDefaultMapping();
}

void ACakeUpPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ApplyDefaultMapping();
}

void ACakeUpPlayerController::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    ApplyDefaultMapping();
}

void ACakeUpPlayerController::ApplyDefaultMapping()
{
    if (!IsLocalController())
    {
        return;
    }

    ULocalPlayer* LP = GetLocalPlayer();
    if (!LP)
    {
        return;
    }

    const int32 LocalIndex = LP->GetLocalPlayerIndex();

    UInputMappingContext* ContextToUse = nullptr;

    if (LocalIndex == 0)
    {
        ContextToUse = DefaultInputContext;
    }
    else if (LocalIndex == 1)
    {
        ContextToUse = SecondPlayerInputContext ? SecondPlayerInputContext : DefaultInputContext;
    }
    else
    {
        ContextToUse = DefaultInputContext;
    }

    if (!ContextToUse)
    {
        UE_LOG(LogTemp, Warning,
               TEXT("ApplyDefaultMapping: no mapping context for local player %d"),
               LocalIndex);
        return;
    }

    if (UEnhancedInputLocalPlayerSubsystem* Subsys =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
    {
        Subsys->ClearAllMappings();
        Subsys->AddMappingContext(ContextToUse, /*Priority*/0);

        UE_LOG(LogTemp, Log,
               TEXT("ApplyDefaultMapping: local player %d got context %s"),
               LocalIndex,
               *GetNameSafe(ContextToUse));
    }
}
