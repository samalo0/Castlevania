//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/18/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/CrusherActor.h"

#include "CastlevaniaFunctionLibrary.h"
#include "PaperSpriteComponent.h"

ACrusherActor::ACrusherActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
	RootSceneComponent->SetMobility(EComponentMobility::Static);
	
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(GetRootComponent());
	SpriteComponent->SetMobility(EComponentMobility::Movable);
	SpriteComponent->SetCollisionProfileName("OverlapOnlyPawn");
}

void ACrusherActor::BeginPlay()
{
	Super::BeginPlay();

	LocationFloat = SpriteComponent->GetRelativeLocation();
		
	// Todo only enable when on screen??
	SetActorTickEnabled(true);
}

void ACrusherActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bMovingDown)
	{
		LocationFloat.Z += LowerVelocity * DeltaSeconds;		
	}
	else
	{
		LocationFloat.Z += RaiseVelocity * DeltaSeconds;
	}
	
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!SpriteComponent->GetRelativeLocation().Equals(LocationInteger))
	{
		SpriteComponent->SetRelativeLocation(LocationInteger);

		if((bMovingDown && LocationInteger.Z <= StopRelativeLocation.Z) ||
		   (!bMovingDown && LocationInteger.Z >= 0.0f))
		{
			bMovingDown = !bMovingDown;
		}
	}
}

