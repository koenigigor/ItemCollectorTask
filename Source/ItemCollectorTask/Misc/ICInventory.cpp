// Tava item collection test task 2024.


#include "ICInventory.h"

#include "Net/UnrealNetwork.h"


void FInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	// Not provided to delete elements
	if (!RemovedIndices.IsEmpty())
		checkNoEntry();
}

void FInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	// Update count in acceleration map, and notify delegate
	for (const auto& Index : AddedIndices)
	{
		const auto Item = InventoryItems[Index];
		InventoryItemsAccelerationMap.Add(Item.ItemType, Item.Count);

		if (OwningInventory.IsValid())
		{
			OwningInventory->OnUpdateItem.Broadcast(Item.ItemType, Item.Count);
		}
	}
}

void FInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	// Update count in acceleration map, and notify delegate
	for (const auto& Index : ChangedIndices)
	{
		const auto Item = InventoryItems[Index];
		auto& AccelerationCountRef = InventoryItemsAccelerationMap.FindChecked(Item.ItemType);
		AccelerationCountRef = Item.Count;

		if (OwningInventory.IsValid())
		{
			OwningInventory->OnUpdateItem.Broadcast(Item.ItemType, Item.Count);
		}
	}	
}

void FInventoryList::AddItem(EItemType ItemType)
{
	// Find item or create new
	const auto ItemPtr = InventoryItems.FindByPredicate([&](const FInventoryEntry& InventoryEntry){ return InventoryEntry.ItemType == ItemType; });
	auto& Item = ItemPtr ? *ItemPtr : InventoryItems.Add_GetRef({ItemType, 0});

	// Add count
	Item.Count += 1;

	// Notify replication
	MarkItemDirty(Item);

	// Update acceleration map on server side
	auto& ServerMapCount = InventoryItemsAccelerationMap.FindOrAdd(Item.ItemType);
	ServerMapCount = Item.Count;

	// Call delegate on server side
	if (OwningInventory.IsValid())
	{
		OwningInventory->OnUpdateItem.Broadcast(Item.ItemType, Item.Count);
	}
}

int32 FInventoryList::GetItemCount(EItemType ItemType) const
{
	const auto CountPtr = InventoryItemsAccelerationMap.Find(ItemType);
	return CountPtr ? *CountPtr : 0;
}

//

UICInventory::UICInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UICInventory::OnRegister()
{
	Super::OnRegister();

	InventoryList.OwningInventory = MakeWeakObjectPtr(this);
}

void UICInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UICInventory::AddItem(EItemType ItemType)
{
	ensure(GetOwner()->HasAuthority());

	InventoryList.AddItem(ItemType);
}

int32 UICInventory::GetItemCount(EItemType ItemType) const
{
	return InventoryList.GetItemCount(ItemType);
}




