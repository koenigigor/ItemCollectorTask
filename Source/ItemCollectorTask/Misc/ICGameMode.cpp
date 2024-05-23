// Tava item collection test task 2024.


#include "ICGameMode.h"

#include "CollectableItem.h"
#include "ICGameState.h"
#include "ICInventory.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogICGameMode, All, All);

void AICGameMode::PreInitializeComponents()
{
	// Chose required collectable
	//	(in PreInit because i wanna replicate it InitialOnly. So need choose before create GameState)
	const auto Enum = StaticEnum<EItemType>();
	RequiredItemType = EItemType(FMath::RandRange(1, Enum->NumEnums() - 2)); // 0 is None, Last is default hidden value
	UE_LOG(LogICGameMode, Log, TEXT("RequiredItemType = %s"), *Enum->GetNameStringByValue(static_cast<int>(RequiredItemType)))

	Super::PreInitializeComponents();
}

void AICGameMode::InitGameState()
{
	Super::InitGameState();

	UE_LOG(LogICGameMode, Display, TEXT("InitGameState"))
	if (const auto GS = Cast<AICGameState>(GameState))
	{
		GS->RequiredItemType = RequiredItemType;
		GS->bInteractOnlyWithRequiredItem = bInteractOnlyWithRequiredItem;
		GS->RequiredItemCount = RequiredItemCount;
		GS->MatchDuration = MatchDuration;
	}
}

void AICGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnCollectableItems();
	StartMatch();
}

void AICGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	// Listen to inventory for required items 
	check(NewPlayer && NewPlayer->PlayerState);
	const auto Inventory = NewPlayer->PlayerState->FindComponentByClass<UICInventory>();
	check(Inventory);

	Inventory->OnUpdateItem.AddUObject(this, &ThisClass::OnUpdateItem, NewPlayer);
}

void AICGameMode::StartMatch()
{
	// Start match timer
	GetWorldTimerManager().SetTimer(
		MatchTimer,
		FTimerDelegate::CreateLambda([&]
		{
			EndMatch(nullptr);
		}),
		MatchDuration,
		false);

	if (const auto GS = Cast<AICGameState>(GameState))
	{
		GS->HandleStartMatch();
	}
}

void AICGameMode::EndMatch(AController* Winner)
{
	if (const auto GS = Cast<AICGameState>(GameState))
	{
		const auto WinnerPlayerState = Winner ? Winner->PlayerState : nullptr;
		GS->HandleEndMatch(WinnerPlayerState);
	}

	// Restart level after short delay
	constexpr float RestartMatchDelay = 3.f;
	GetWorldTimerManager().SetTimer(
		MatchTimer,
		FTimerDelegate::CreateLambda([&]
		{
			GetWorld()->ServerTravel(UGameplayStatics::GetCurrentLevelName(this));
		}),
		RestartMatchDelay,
		false);
}

void AICGameMode::SpawnCollectableItems()
{
	if (!ensure(CollectableItemClass)) return;

	// Get spawn points
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), SpawnPoints);

	// Shuffle array if spawn points more then need
	if (SpawnPoints.Num() > CollectableItemsSpawnCount)
	{
		const int32 LastIndex = SpawnPoints.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			const auto Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				SpawnPoints.Swap(i, Index);
			}
		}
	}

	// Spawn collectables
	//todo ensure count spawned items more then requires
	const auto SpawnCount = FMath::Min(CollectableItemsSpawnCount, SpawnPoints.Num());
	check(SpawnCount >= 0);
	for (int32 i = 0; i <= SpawnCount; ++i)
	{
		const auto SpawnPoint = SpawnPoints[i];
		GetWorld()->SpawnActor(CollectableItemClass, &SpawnPoint->GetActorTransform());
	}
}

void AICGameMode::OnUpdateItem(EItemType Item, int32 NewCount, AController* Player)
{
	if (Item == RequiredItemType && NewCount == RequiredItemCount)
	{
		EndMatch(Player);
	}
}
