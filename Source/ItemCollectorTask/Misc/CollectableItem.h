// Tava item collection test task 2024.

#pragma once

#include "CoreMinimal.h"
#include "ICStructures.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "CollectableItem.generated.h"

UCLASS(Abstract)
class ITEMCOLLECTORTASK_API ACollectableItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
public:	
	ACollectableItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::None;
	
protected:
	virtual bool Interact_Implementation(APawn* Pawn) override;
};
