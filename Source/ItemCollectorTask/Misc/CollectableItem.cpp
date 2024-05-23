// Tava item collection test task 2024.


#include "CollectableItem.h"

#include "ICGameMode.h"
#include "ICInventory.h"
#include "ICPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogCollectableItem, All, All);

ACollectableItem::ACollectableItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

bool ACollectableItem::Interact_Implementation(APawn* Pawn)
{
	if (!HasAuthority())
	{
		UE_LOG(LogCollectableItem, Warning, TEXT("No authority for interaction"))
		return false;
	}

	// Check is required item
	if (const auto GM = GetWorld()->GetAuthGameMode<AICGameMode>())
	{
		if (GM->bInteractOnlyWithRequiredItem && GM->RequiredItemType != ItemType)
		{
			return false;
		}
	}
	
	const auto PlayerState = Pawn ? Pawn->GetPlayerState() : nullptr;
	const auto PlayerInventory = PlayerState ? PlayerState->FindComponentByClass<UICInventory>() : nullptr;
	if (!PlayerInventory) return false;

	PlayerInventory->AddItem(ItemType);
	Destroy();

	return true;
}

