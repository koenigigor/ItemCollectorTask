// Tava item collection test task 2024.

#pragma once

#include "CoreMinimal.h"
#include "ICStructures.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ICInventory.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryUpdate, EItemType Item, int32 NewCount);

USTRUCT()
struct FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryEntry(){};
	FInventoryEntry(EItemType InItemType, int32 InCount)
		: ItemType(InItemType), Count(InCount) {};
	
	UPROPERTY()
	EItemType ItemType = EItemType::None;
	
	UPROPERTY()
	int32 Count = 0;
};

USTRUCT()
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FInventoryList() {};

	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInventoryEntry, FInventoryList>(InventoryItems, DeltaParams, *this);
	}

	void AddItem(EItemType ItemType);
	int32 GetItemCount(EItemType ItemType) const;
	
	UPROPERTY()
	TArray<FInventoryEntry> InventoryItems;

	TMap<EItemType, int32> InventoryItemsAccelerationMap;

	TWeakObjectPtr<UICInventory> OwningInventory = nullptr;
};

template <>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

//

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMCOLLECTORTASK_API UICInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UICInventory();
	virtual void OnRegister() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddItem(EItemType ItemType);

	UFUNCTION(BlueprintPure)
	int32 GetItemCount(EItemType ItemType) const;

	FInventoryUpdate OnUpdateItem;
	
protected:
	UPROPERTY(Replicated)
	FInventoryList InventoryList;
};
