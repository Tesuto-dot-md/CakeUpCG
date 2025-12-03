#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CakeUpGameMode.generated.h"

UCLASS(minimalapi)
class ACakeUpGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACakeUpGameMode();

protected:
    virtual void BeginPlay() override;
};
