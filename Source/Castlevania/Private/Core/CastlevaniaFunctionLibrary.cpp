//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/17/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Core/CastlevaniaFunctionLibrary.h"

FVector UCastlevaniaFunctionLibrary::RoundVectorToInt(const FVector Vector)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("RoundVectorToInt"), STAT_RoundVectorToInt, STATGROUP_Castlevania);
	
	FVector OutVector = Vector;
	OutVector.X = FMath::RoundToInt(OutVector.X);
	OutVector.Z = FMath::RoundToInt(OutVector.Z);

	return OutVector;
}
