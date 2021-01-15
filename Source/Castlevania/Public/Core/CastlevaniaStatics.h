//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "CastlevaniaStatics.generated.h"

#define ECC_Weapon				ECC_GameTraceChannel1
#define ECC_Interactable		ECC_GameTraceChannel2
#define ECC_Pickup				ECC_GameTraceChannel3
#define ECC_CameraVisibility	ECC_GameTraceChannel4
#define ECC_Enemy				ECC_GameTraceChannel5

UENUM()
enum class EWhipType : uint8
{
	Leather,
    Short,
    Long
};

UENUM()
enum class EWeaponType : uint8
{
	None,
	Axe,
	Boomerang,
	Clock,
	Dagger,
	HolyWater
};
