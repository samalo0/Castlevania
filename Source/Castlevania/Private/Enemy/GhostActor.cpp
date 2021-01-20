//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/19/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/GhostActor.h"


#include "CastlevaniaFunctionLibrary.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "PaperFlipbookComponent.h"

AGhostActor::AGhostActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	DamageToPlayer = 3;
	Life = 4;
	Score = 300;
}

void AGhostActor::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(GameMode))
	{
		Pawn = GameMode->GetPlayerPawn();
		SetActorTickEnabled(true);
	}

	LocationFloat = GetActorLocation();
}

void AGhostActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
{
	Super::HitWithWeapon(Damage, bPlaySound, WeaponLocation);

	if(Life <= 0)
	{
		SetActorTickEnabled(false);
	}
}

void AGhostActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsValid(Pawn))
	{
		SetActorTickEnabled(false);
	}

	LocationFloat = FMath::VInterpConstantTo(LocationFloat, Pawn->GetActorLocation(), DeltaSeconds, InterpolationSpeed);
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);

		if(Pawn->GetActorLocation().X < LocationInteger.X)
		{
			FlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
		}
		else
		{
			FlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		}
	}
}

void AGhostActor::TimeStop(const bool bEnable)
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
