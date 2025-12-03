#include "CakeUpMenuPlayerController.h"

#include "Blueprint/UserWidget.h"

ACakeUpMenuPlayerController::ACakeUpMenuPlayerController()
{
    bShowMouseCursor = true;
}

void ACakeUpMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);
        }
    }
}
