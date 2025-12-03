#include "CakeUpGameState.h"
#include "Net/UnrealNetwork.h"

ACakeUpGameState::ACakeUpGameState()
{
    bReplicates = true;
    bPickupEnabled = true;
}

void ACakeUpGameState::OnRep_PickupEnabled()
{
    // UI / audio hook
}

void ACakeUpGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACakeUpGameState, bPickupEnabled);
}