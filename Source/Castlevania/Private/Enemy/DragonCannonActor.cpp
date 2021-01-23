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
#include "Kismet/GameplayStatics.h"

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
			World->GetTimerManager().SetTimer(TimerHandle, this, &ADragonCannonActor::PrepareToFire, DelayBetweenFiring);
		}
	}
}

void ADragonCannonActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
{
	Super::HitWithWeapon(Damage, Damage < Life, WeaponLocation);

	if(Damage >= Life)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}
	
	if(Life <= 0)
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}

void ADragonCannonActor::PrepareToFire()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		if(IsValid(Pawn))
		{
			bFireRight = Pawn->GetActorLocation().X > GetActorLocation().X; 
		}
		
		FlipbookComponent->SetFlipbook(RedFlipbook);
		World->GetTimerManager().SetTimer(TimerHandle, this, &ADragonCannonActor::StartFire, DelayForFirstFire);
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
			World->GetTimerManager().SetTimer(TimerHandle, this, &ADragonCannonActor::PrepareToFire, DelayBetweenFiring, false);
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

	FTransform Transform;
	if(bFireRight)
	{
		Transform = RightSideFireOffset * GetActorTransform();
	}
	else
	{
		// Set to fire left.
		Transform = LeftSideFireOffset * GetActorTransform();
		Transform.SetScale3D(FVector(-1.0f, 1.0f, 1.0f));
	}
	World->SpawnActor<AEnemyProjectileActor>(ProjectileClass, Transform, SpawnParameters);

	FireCount++;
}

void ADragonCannonActor::StartFire()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		FlipbookComponent->SetFlipbook(BlueFlipbook);

		SpawnProjectile();
		World->GetTimerManager().SetTimer(TimerHandle, this, &ADragonCannonActor::RepeatFire, DelayForRepeatFire, true);
	}
}

void ADragonCannonActor::TimeStop(const bool bIsActive)
{
	Super::TimeStop(bIsActive);
	
	if(Life <= 0)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		if(bIsActive)
		{
			World->GetTimerManager().PauseTimer(TimerHandle);	
		}
		else
		{
			World->GetTimerManager().UnPauseTimer(TimerHandle);
		}
	}
}
