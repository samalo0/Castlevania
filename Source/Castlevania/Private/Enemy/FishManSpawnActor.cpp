//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/FishManSpawnActor.h"

#include "Kismet/GameplayStatics.h"
#include "SplashEffectActor.h"

void AFishManSpawnActor::SpawnEnemy(const FTransform Transform)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(this, SplashSound);
		
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.ObjectFlags = RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for(int32 Index = 0; Index < 3; Index++)
	{
		World->SpawnActor<ASplashEffectActor>(SplashEffectClass, Transform, SpawnParameters);	
	}

	Super::SpawnEnemy(Transform);
}
