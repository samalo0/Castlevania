//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/19/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/DragonCannonActor.h"

#include "Pawn/CastlevaniaPawn.h"
#include "Core/CastlevaniaGameModeBase.h"
#include "Enemy/EnemyProjectileActor.h"
#include "PaperFlipbookComponent.h"

ADragonCannonActor::ADragonCannonActor()
{
	Score = 400;
	Life = 12;
}

void ADragonCannonActor::BeginPlay()
{
	Super::BeginPlay();

	BlueFlipbook = FlipbookComponent->GetFlipbook();

	UWorld* World = GetWorld();
	if(IsValid(GameMode) && IsValid(World))
	{
		Pawn = GameMode->GetPlayerPawn();
		if(IsValid(Pawn))
		{
			World->GetTimerManager().SetTimer(TimerHandle, this, &ADragonCannonActor::StartFire, StartFireTime);
		}
	}
}

void ADragonCannonActor::RepeatFire()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		SpawnProjectile();
		if(FireCount >= NumberOfProjectilesToFire)
		{
			FireCount = 0;
			World->GetTimerManager().SetTimer(TimerHandle, this, &ADragonCannonActor::StartFire, StartFireTime, false);
		}		
	}
}

void ADragonCannonActor::SpawnProjectile()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.ObjectFlags |= RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	World->SpawnActor<AEnemyProjectileActor>(ProjectileClass, GetActorTransform(), SpawnParameters);

	FireCount++;
}

void ADragonCannonActor::StartFire()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		SpawnProjectile();
		World->GetTimerManager().SetTimer(TimerHandle, this, &ADragonCannonActor::StartFire, RepetitiveFireTime, true);
	}
}
