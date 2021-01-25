//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/19/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/MedusaActor.h"

#include "Components/BoxComponent.h"
#include "Core/CastlevaniaCameraActor.h"
#include "Core/CastlevaniaFunctionLibrary.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Enemy/MedusaSnakeActor.h"
#include "PaperFlipbookComponent.h"

AMedusaActor::AMedusaActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	FlipbookComponent->SetHiddenInGame(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMedusaActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
{
	Super::HitWithWeapon(Damage, bPlaySound, WeaponLocation);

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

	// Check for triggering death; disable tick to prevent movement after death.
	const int32 EnemyHealth = GameInstance->GetEnemyHealth();
	if(EnemyHealth == 0)
	{
		State = EMedusaState::Death;
		SetActorTickEnabled(false);
	}
}

void AMedusaActor::OnBossFightStart()
{
	HeadSpriteToDestroy->Destroy();
	
	FlipbookComponent->SetHiddenInGame(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Destination = GetActorLocation();
	State = EMedusaState::Waiting;
	WaitTime = FMath::RandRange(WaitTimeLimits.X, WaitTimeLimits.Y);
	SetActorTickEnabled(true);
}

void AMedusaActor::SpawnSnakes(const bool bRightSide)
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

	FTransform Transform = GetActorTransform();
	if(!bRightSide)
	{
		Transform.SetScale3D(FVector(-1.0f, 1.0f, 1.0f));
	}
	
	World->SpawnActor<AMedusaSnakeActor>(MedusaSnakeClass, Transform, SpawnParameters);
}

void AMedusaActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch(State)
	{
	case EMedusaState::Waiting:
		WaitTime -= DeltaSeconds;
		if(WaitTime <= 0)
		{
			WaitTime = 0.0f;
			State = EMedusaState::SpawnSnakes;
		}
		break;
	case EMedusaState::MovingToDestination:
		{
			LocationFloat = FMath::VInterpConstantTo(LocationFloat, Destination, DeltaSeconds, MovementSpeed);
			const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
			if(!GetActorLocation().Equals(LocationInteger, 0.99f))
			{
				SetActorLocation(LocationInteger);
				if(LocationInteger.Equals(Destination, 0.99f))
				{
					WaitTime = FMath::RandRange(WaitTimeLimits.X, WaitTimeLimits.Y);
					State = EMedusaState::Waiting;
				}
			}
		}
		break;
	case EMedusaState::GetDestinationNearPlayer:
		if(IsValid(Pawn))
		{
			const FVector PawnLocation = Pawn->GetActorLocation();
			if(FVector::Dist(PawnLocation, GetActorLocation()) > MaximumMovementDistance)
			{
				const FVector Direction = (PawnLocation - GetActorLocation()).GetSafeNormal();
				const FVector CloseToPawn = GetActorLocation() + Direction * MaximumMovementDistance;
				Destination.X = FMath::Clamp(CloseToPawn.X, XClamp.X, XClamp.Y);
				Destination.Z = FMath::Clamp(CloseToPawn.Z, ZClamp.X, ZClamp.Y);
			}
			else
			{
				Destination.X = FMath::Clamp(PawnLocation.X, XClamp.X, XClamp.Y);
				Destination.Z = FMath::Clamp(PawnLocation.Z, ZClamp.X, ZClamp.Y);
			}

			bLastTowardsPlayer = true;
			State = EMedusaState::MovingToDestination;
		}
		break;
	case EMedusaState::GetDestinationAwayFromPlayer:
		if(IsValid(Pawn))
		{
			const FVector PawnLocation = Pawn->GetActorLocation();
			
			const FVector Direction = (GetActorLocation() - PawnLocation).GetSafeNormal();
			const FVector AwayFromPawn = GetActorLocation() + Direction * MaximumMovementDistance;
			Destination.X = FMath::Clamp(AwayFromPawn.X, XClamp.X, XClamp.Y);
			Destination.Z = FMath::Clamp(AwayFromPawn.Z, ZClamp.X, ZClamp.Y);

			bLastTowardsPlayer = false;
			State = EMedusaState::MovingToDestination;
		}
		break;
	case EMedusaState::SpawnSnakes:
		if(IsValid(Pawn))
		{
			SpawnSnakes(Pawn->GetActorLocation().X > GetActorLocation().X);

			if(bLastTowardsPlayer && FMath::Abs(GetActorLocation().X - Pawn->GetActorLocation().X) < MaximumMovementDistance)
			{
				State = EMedusaState::GetDestinationAwayFromPlayer;
			}
			else
			{
				State = EMedusaState::GetDestinationNearPlayer;
			}
		}
		break;
	default:
		break;
	}
}

void AMedusaActor::TimeStop(const bool bIsActive)
{
	if(State != EMedusaState::Waiting && State != EMedusaState::Death)
	{
		if(bIsActive)
		{
			SetActorTickEnabled(false);
			FlipbookComponent->Stop();
		}
		else 
		{
			SetActorTickEnabled(true);
			FlipbookComponent->Play();
		}	
	}
}

void AMedusaActor::TriggerBattle()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &AMedusaActor::OnBossFightStart, BossFightDelay);
	}
}
