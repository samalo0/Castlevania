//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/16/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Flow/MovingPlatformActor.h"

AMovingPlatformActor::AMovingPlatformActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

