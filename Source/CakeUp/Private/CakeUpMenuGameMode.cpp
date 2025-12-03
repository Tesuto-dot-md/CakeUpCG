#include "CakeUpMenuGameMode.h"
#include "CakeUpMenuPlayerController.h"

ACakeUpMenuGameMode::ACakeUpMenuGameMode()
{
    DefaultPawnClass = nullptr; // без пешки
    PlayerControllerClass = ACakeUpMenuPlayerController::StaticClass();
}
