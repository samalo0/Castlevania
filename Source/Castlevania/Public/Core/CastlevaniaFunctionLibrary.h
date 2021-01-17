//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/17/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CastlevaniaFunctionLibrary.generated.h"

DECLARE_STATS_GROUP(TEXT("CastlevaniaLibraryStat"), STATGROUP_Castlevania, STATCAT_Advanced);

/**
 * Library for functions used across classes and blueprints. 
 */
UCLASS()
class CASTLEVANIA_API UCastlevaniaFunctionLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:
	
	// Returns the input vector rounded to integer values.
	static FVector RoundVectorToInt(const FVector Vector);
	
};
