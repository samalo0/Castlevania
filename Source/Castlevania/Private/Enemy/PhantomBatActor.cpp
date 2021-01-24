//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/PhantomBatActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaCameraActor.h"
#include "CastlevaniaFunctionLibrary.h"
#include "CastlevaniaGameInstance.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "PaperFlipbookComponent.h"

APhantomBatActor::APhantomBatActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APhantomBatActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		if(IsValid(GameMode))
		{
			Pawn = GameMode->GetPlayerPawn();
		}
	}

	FlipbookComponent->SetComponentTickEnabled(false);
}

void APhantomBatActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
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
	
	const int32 EnemyHealth = GameInstance->GetEnemyHealth();
	if(EnemyHealth == 0)
	{
		State = EPhantomBatState::Death;
		SetActorTickEnabled(false);
	}
}

void APhantomBatActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	switch(State)
	{
	case EPhantomBatState::MoveToDestination:
		{
			LocationFloat = FMath::VInterpConstantTo(LocationFloat, Destination, DeltaSeconds, MovementSpeed);
			const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
			if(!GetActorLocation().Equals(LocationInteger, 0.99f))
			{
				SetActorLocation(LocationInteger);

				if(LocationInteger.Equals(Destination, 0.99f))
				{
					State = EPhantomBatState::WaitRandomTime;
					WaitTime = FMath::FRandRange(MinimumWaitTime, MaximumWaitTime);
				}				
			}
		}
		break;
	case EPhantomBatState::WaitRandomTime:
		WaitTime -= DeltaSeconds;
		if(WaitTime <= 0.0f)
		{
			// Allow twice in a row with a random chance.
			const float TwiceInARow = FMath::FRandRange(0.0f, 1.0f);
			if(TwiceInARow < 0.25f)
			{
				bLastTowardsPlayer = !bLastTowardsPlayer;
			}
			
			if(bLastTowardsPlayer)
			{
				bLastTowardsPlayer = false;
				State = EPhantomBatState::GetLocationAwayFromPlayer;
				MovementSpeed = SlowMovementSpeed;
			}
			else
			{
				bLastTowardsPlayer = true;
				State = EPhantomBatState::GetPlayerLocation;
				MovementSpeed = FastMovementSpeed;
			}
		}
		break;
	case EPhantomBatState::GetPlayerLocation:
		if(IsValid(Pawn))
		{
			const FVector PawnLocation = Pawn->GetActorLocation();
			Destination = FVector(PawnLocation.X, GetActorLocation().Y, PawnLocation.Z);
			MovementSpeed = FastMovementSpeed;
			
			State = EPhantomBatState::MoveToDestination;
		}
		break;
	case EPhantomBatState::GetLocationAwayFromPlayer:
		if(IsValid(Pawn) && IsValid(Camera))
		{
			const FVector CameraLocation = Camera->GetActorLocation();
			float AdderX = FMath::FRandRange(64.0f, 96.0f);
			if(Pawn->GetActorLocation().X > CameraLocation.X)
			{
				AdderX *= -1.0f;
			}
			const float AdderZ = FMath::FRandRange(-32.0f, 32.0f);

			Destination = UCastlevaniaFunctionLibrary::RoundVectorToInt(FVector(CameraLocation.X + AdderX, GetActorLocation().Y, CameraLocation.Z + AdderZ));
			
			State = EPhantomBatState::MoveToDestination;
		}
		break;
	default:
		break;
	}
}

void APhantomBatActor::TimeStop(const bool bEnable)
{
	if(State != EPhantomBatState::InitialWait && State != EPhantomBatState::Death)
	{
		if(bEnable)
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

void APhantomBatActor::TriggerBattle()
{
	State = EPhantomBatState::MoveToDestination;

	FlipbookComponent->SetComponentTickEnabled(true);
	FlipbookComponent->SetFlipbook(FlyingFlipbook);
	BoxComponent->SetBoxExtent(FlyingCollisionBoxExtent);

	MovementSpeed = SlowMovementSpeed;
	
	if(IsValid(Camera))
	{
		const FVector CameraLocation = Camera->GetActorLocation();
		Destination = UCastlevaniaFunctionLibrary::RoundVectorToInt(FVector(CameraLocation.X, GetActorLocation().Y, CameraLocation.Z));
	}
	
	SetActorTickEnabled(true);
}
