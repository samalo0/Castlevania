//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/18/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/CrusherActor.h"

#include "PaperSpriteComponent.h"

ACrusherActor::ACrusherActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpriteComponent1 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent1"));
	SetRootComponent(SpriteComponent1);
	SpriteComponent1->SetCollisionProfileName("OverlapOnlyPawn");

	SpriteComponent2 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent2"));
	SpriteComponent2->SetupAttachment(GetRootComponent());
	SpriteComponent2->SetCollisionProfileName("OverlapOnlyPawn");

	SpriteComponent3 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent3"));
	SpriteComponent3->SetupAttachment(GetRootComponent());
	SpriteComponent3->SetCollisionProfileName("OverlapOnlyPawn");

	SpriteComponent4 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent4"));
	SpriteComponent4->SetupAttachment(GetRootComponent());
	SpriteComponent4->SetCollisionProfileName("OverlapOnlyPawn");
}

void ACrusherActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACrusherActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

