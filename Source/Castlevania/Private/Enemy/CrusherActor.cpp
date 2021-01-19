//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/18/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/CrusherActor.h"

#include "PaperSpriteComponent.h"

ACrusherActor::ACrusherActor()
{
	PrimaryActorTick.bCanEverTick = false;
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

	// The in game crusher has more states. Need to do this differently to make it smoother.
	// Todo only enable when on screen?
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &ACrusherActor::IncrementState, StateChangeSpeed, true);
	}
}

void ACrusherActor::EnableSpriteComponent(const bool bEnableNotDisable, UPaperSpriteComponent* Component)
{
	if(bEnableNotDisable)
	{
		Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Component->SetHiddenInGame(false);
	}
	else
	{
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetHiddenInGame(true);
	}
}

void ACrusherActor::IncrementState()
{
	if(bMovingDown)
	{
		State++;
		if(State > MaximumLower)
		{
			bMovingDown = false;
			State = MaximumLower - 1;
		}
	}
	else
	{
		State--;
		if(State < 0)
		{
			bMovingDown = true;
			State = 1;
		}
	}
	
	switch(State)
	{
	case 0:
		SpriteComponent1->SetSprite(BaseSprite);

		EnableSpriteComponent(true, SpriteComponent1);
		EnableSpriteComponent(false, SpriteComponent2);
		EnableSpriteComponent(false, SpriteComponent3);
		EnableSpriteComponent(false, SpriteComponent4);
		break;
	case 1:
		SpriteComponent1->SetSprite(ArmSprite);
		SpriteComponent2->SetSprite(BaseSprite);

		EnableSpriteComponent(true, SpriteComponent1);
		EnableSpriteComponent(true, SpriteComponent2);
		EnableSpriteComponent(false, SpriteComponent3);
		EnableSpriteComponent(false, SpriteComponent4);
		break;
	case 2:
		SpriteComponent1->SetSprite(ArmSprite);
		SpriteComponent2->SetSprite(ArmSprite);
		SpriteComponent3->SetSprite(BaseSprite);

		EnableSpriteComponent(true, SpriteComponent1);
		EnableSpriteComponent(true, SpriteComponent2);
		EnableSpriteComponent(true, SpriteComponent3);
		EnableSpriteComponent(false, SpriteComponent4);
		break;
	case 3:
		SpriteComponent1->SetSprite(ArmSprite);
		SpriteComponent2->SetSprite(ArmSprite);
		SpriteComponent3->SetSprite(ArmSprite);
		SpriteComponent4->SetSprite(BaseSprite);

		EnableSpriteComponent(true, SpriteComponent1);
		EnableSpriteComponent(true, SpriteComponent2);
		EnableSpriteComponent(true, SpriteComponent3);
		EnableSpriteComponent(true, SpriteComponent4);
		break;			
	default:
		break;
	}	

}

