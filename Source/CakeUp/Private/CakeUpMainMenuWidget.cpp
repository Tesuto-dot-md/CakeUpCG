#include "CakeUpMainMenuWidget.h"
#include "CakeUpInstance.h"
#include "Kismet/GameplayStatics.h"

void UCakeUpMainMenuWidget::OnSoloClicked()
{
    if (UWorld* World = GetWorld())
    {
        if (UCakeUpInstance* GI = World->GetGameInstance<UCakeUpInstance>())
        {
            GI->StartSoloGame();
        }
    }
}

void UCakeUpMainMenuWidget::OnLocalCoopClicked()
{
    if (UWorld* World = GetWorld())
    {
        if (UCakeUpInstance* GI = World->GetGameInstance<UCakeUpInstance>())
        {
            GI->StartLocalCoopGame();
        }
    }
}

void UCakeUpMainMenuWidget::OnHostClicked()
{
    if (UWorld* World = GetWorld())
    {
        if (UCakeUpInstance* GI = World->GetGameInstance<UCakeUpInstance>())
        {
            GI->HostOnlineGame();
        }
    }
}

void UCakeUpMainMenuWidget::OnJoinClicked(const FString& Address)
{
    if (UWorld* World = GetWorld())
    {
        if (UCakeUpInstance* GI = World->GetGameInstance<UCakeUpInstance>())
        {
            GI->JoinOnlineGame(Address);
        }
    }
}
