//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/17/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/MedusaHeadActor.h"


#include "PaperFlipbookComponent.h"
#include "Core/CastlevaniaFunctionLibrary.h"
#include "Core/CastlevaniaGameModeBase.h"
#include "Pawn/CastlevaniaPawn.h"

AMedusaHeadActor::AMedusaHeadActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AMedusaHeadActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(GameMode))
		{
			ACastlevaniaPawn* Pawn = GameMode->GetPlayerPawn();
			if(IsValid(Pawn))
			{
				if(Pawn->GetActorLocation().X < GetActorLocation().X)
				{
					MovementSpeed *= -1.0f;
				}
			}
		}
	}
	
	InitialLocation = GetActorLocation();
	SetActorTickEnabled(true);
}

void AMedusaHeadActor::TimeStop(const bool bEnable)
{
	if(bEnable)
	{
		FlipbookComponent->SetComponentTickEnabled(false);
		SetActorTickEnabled(false);	
	}
	else
	{
		SetActorTickEnabled(true);
		FlipbookComponent->SetComponentTickEnabled(true);
	}
}

void AMedusaHeadActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
{
	SetActorTickEnabled(false);
	
	Super::HitWithWeapon(Damage, bPlaySound, WeaponLocation);
}

void AMedusaHeadActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Accumulator += DeltaSeconds;
	
	LocationFloat.X += MovementSpeed * DeltaSeconds;
	LocationFloat.Z = InitialLocation.Z + Amplitude * FMath::Sin(2 * PI * Frequency * Accumulator);
	
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);
	}
}
