//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Weapon/WeaponActor.h"

#include "BonusPointsEffectActor.h"
#include "CastlevaniaGameInstance.h"

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	LocationFloat = GetActorLocation();
}

void AWeaponActor::Hit()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
	if(!IsValid(GameInstance))
	{
		return;
	}
	
	NumberOfEnemiesKilled++;

	int32 BonusDisplay;
	int32 BonusAdder;
	
	switch(NumberOfEnemiesKilled)
	{
	case 2:
		BonusDisplay = 1000;
		BonusAdder = 1000;
		break;
	case 3:
		BonusDisplay = 2000;
		BonusAdder = 1000;
		break;
	case 4:
		BonusDisplay = 4000;
		BonusAdder = 2000;
		break;
	case 5:
		BonusDisplay = 7000;
		BonusAdder = 3000;
		break;
	default:
		return;
	}

	GameInstance->AddScore(BonusAdder);

	if(IsValid(BonusPointsEffectActor))
	{
		BonusPointsEffectActor->SetLifeSpan(1.0f);
		BonusPointsEffectActor->SetValue(BonusDisplay);
		BonusPointsEffectActor->SetActorLocation(GetActorLocation());
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.ObjectFlags = RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	BonusPointsEffectActor = World->SpawnActor<ABonusPointsEffectActor>(BonusPointsEffectClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
	if(IsValid(BonusPointsEffectActor))
	{
		BonusPointsEffectActor->SetValue(BonusDisplay);
	}
}
