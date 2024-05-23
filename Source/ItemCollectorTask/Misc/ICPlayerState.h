// Tava item collection test task 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ICPlayerState.generated.h"

class UICInventory;

UCLASS()
class ITEMCOLLECTORTASK_API AICPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AICPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	UICInventory* InventoryComponent = nullptr;
};
