//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/25/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Spawn/CandleActor.h"

ACandleActor::ACandleActor()
{
	SpawnOffset = FVector::ZeroVector;
	MinimumLight = 0.05f;
	MaximumLight = 0.20f;
}
