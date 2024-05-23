// Tava item collection test task 2024.

#pragma once

#include "CoreMinimal.h"
#include "ICStructures.h"
#include "GameFramework/GameModeBase.h"
#include "ICGameMode.generated.h"


class ACollectableItem;

UCLASS()
class ITEMCOLLECTORTASK_API AICGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PreInitializeComponents() override;
	virtual void InitGameState() override;
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CollectableItemsSpawnCount = 10;

	/** Class for spawn collectable items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACollectableItem> CollectableItemClass;

	/** Required item type for play session */
	EItemType RequiredItemType = EItemType::None;

	/** Target count of items for end match */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RequiredItemCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bInteractOnlyWithRequiredItem = true;

	/** Time until match ends */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MatchDuration = 30.f;
	
protected:
	void SpawnCollectableItems();

	/** Start collect items match state */
	void StartMatch();

	/** End collect items match state */
	void EndMatch(AController* Winner);

	void OnUpdateItem(EItemType Item, int32 NewCount, AController* Player);

	FTimerHandle MatchTimer;
};
