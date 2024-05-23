// Tava item collection test task 2024.

#pragma once

#include "CoreMinimal.h"
#include "ICStructures.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Sphere,
	Cube,
	Cylinder
};

