// Tava item collection test task 2024.

#pragma once

#include "CoreMinimal.h"
#include "ICStructures.h"
#include "GameFramework/GameStateBase.h"
#include "ICGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchEndDelegate, APlayerState*, Winner);

UCLASS()
class ITEMCOLLECTORTASK_API AICGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Required item type for play session, set from game mode */
	UPROPERTY(BlueprintReadOnly, Replicated)
	EItemType RequiredItemType = EItemType::None;

	/** Player can collect required only item, or all items, set from game mode */
	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bInteractOnlyWithRequiredItem = true;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 RequiredItemCount = 0;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float MatchDuration = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	float StartMatchTimestamp = 0.0f;

	/** Start match notification, call from game mode */
    void HandleStartMatch();
	void HandleEndMatch(APlayerState* Winner);

	UFUNCTION(NetMulticast, Reliable)
	void OnMatchEnded_Multicast(APlayerState* Winner);

	UPROPERTY(BlueprintAssignable)
	FOnMatchEndDelegate OnMatchEnd;
};
