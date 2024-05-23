// Fill out your copyright notice in the Description page of Project Settings.

#include "ICPlayerState.h"

#include "ICInventory.h"
#include "Net/UnrealNetwork.h"

AICPlayerState::AICPlayerState()
{
	InventoryComponent = CreateDefaultSubobject<UICInventory>("Inventory");
}

void AICPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryComponent);
}
