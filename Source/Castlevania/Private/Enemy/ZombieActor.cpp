//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/ZombieActor.h"

#include "CastlevaniaFunctionLibrary.h"
#include "PaperFlipbookComponent.h"

AZombieActor::AZombieActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AZombieActor::BeginPlay()
{
	Super::BeginPlay();

	MovementSpeed *= GetActorScale3D().X;
	SetActorTickEnabled(true);
}

void AZombieActor::HitWithWeapon(const int32 Damage, const bool bPlaySound)
{
	SetActorTickEnabled(false);

	Super::HitWithWeapon(Damage, bPlaySound);
}

void AZombieActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LocationFloat.X += MovementSpeed * DeltaSeconds;
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);	
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);	
	}
}

void AZombieActor::TimeStop(const bool bEnable)
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

