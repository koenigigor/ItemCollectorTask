// Tava item collection test task 2024.


#include "ICGameState.h"

#include "Net/UnrealNetwork.h"

void AICGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, RequiredItemType, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ThisClass, bInteractOnlyWithRequiredItem, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ThisClass, RequiredItemCount, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ThisClass, MatchDuration, COND_InitialOnly);
	DOREPLIFETIME(ThisClass, StartMatchTimestamp);
}

void AICGameState::HandleStartMatch()
{
	ensure(HasAuthority());
	
	StartMatchTimestamp = GetServerWorldTimeSeconds();
}

void AICGameState::HandleEndMatch(APlayerState* Winner)
{
	ensure(HasAuthority());
	
	OnMatchEnded_Multicast(Winner);
}

void AICGameState::OnMatchEnded_Multicast_Implementation(APlayerState* Winner)
{
	OnMatchEnd.Broadcast(Winner);
}

