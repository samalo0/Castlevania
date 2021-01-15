//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/31/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/VampireBatActor.h"

#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"

AVampireBatActor::AVampireBatActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AVampireBatActor::HitWithWeapon(const int32 Damage, const bool bPlaySound)
{
	SetActorTickEnabled(false);

	Super::HitWithWeapon(Damage, bPlaySound);
}

void AVampireBatActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		if(IsValid(GameMode))
		{
			Pawn = GameMode->GetPlayerPawn();
			if(IsValid(Pawn))
			{
				if(Pawn->GetActorLocation().X < GetActorLocation().X)
				{
					FlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
					MovementSpeed = InitialMovementSpeed * -1.0f;
				}
				else
				{
					MovementSpeed = InitialMovementSpeed;
				}
		
				InitialZ = GetActorLocation().Z;
				SetActorTickEnabled(true);
			}
		}
	}
}

void AVampireBatActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		const FVector Location = GetActorLocation();
		const float NewX = Location.X + MovementSpeed * DeltaSeconds;
	
		const float Time = World->GetTimeSeconds();
		const float NewZ = InitialZ + Amplitude * FMath::Sin(2 * PI * Frequency * Time);
		
		SetActorLocation(FVector(NewX, Location.Y, NewZ));	
	}
}

void AVampireBatActor::TimeStop(const bool bEnable)
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
