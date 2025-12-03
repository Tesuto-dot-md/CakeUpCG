#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CakeUpMenuPlayerController.generated.h"

class UUserWidget;

UCLASS()
class CAKEUP_API ACakeUpMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACakeUpMenuPlayerController();

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<UUserWidget> MainMenuWidget;
};
