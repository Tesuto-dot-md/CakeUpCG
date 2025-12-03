#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CakeUpMainMenuWidget.generated.h"

UCLASS()
class CAKEUP_API UCakeUpMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="CakeUp|Menu")
    void OnSoloClicked();

    UFUNCTION(BlueprintCallable, Category="CakeUp|Menu")
    void OnLocalCoopClicked();

    UFUNCTION(BlueprintCallable, Category="CakeUp|Menu")
    void OnHostClicked();

    UFUNCTION(BlueprintCallable, Category="CakeUp|Menu")
    void OnJoinClicked(const FString& Address);
};
