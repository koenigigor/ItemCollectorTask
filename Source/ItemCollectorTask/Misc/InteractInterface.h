// Tava item collection test task 2024.

#pragma once

#include "InteractInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractInterface : public UInterface
{
    GENERATED_BODY()
};

class IInteractInterface
{    
    GENERATED_BODY()

public:
    /** This actor interact with some pawn */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool Interact(APawn* Pawn);
};