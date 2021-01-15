//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Effect/BonusPointsEffectActor.h"

#include "PaperSpriteComponent.h"

ABonusPointsEffectActor::ABonusPointsEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	LeftSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftSpriteComponent"));
	SetRootComponent(LeftSpriteComponent); 
	LeftSpriteComponent->SetCollisionProfileName("NoCollision");

	RightSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightSpriteComponent"));
	RightSpriteComponent->SetupAttachment(GetRootComponent()); 
	RightSpriteComponent->SetCollisionProfileName("NoCollision");
	RightSpriteComponent->SetRelativeLocation(FVector(8.0f, 0.0f, 0.0f));

	InitialLifeSpan = 1.0f;
}

void ABonusPointsEffectActor::SetValue(const int32 Value)
{
	int32 LeftDigit;
	if(Value < 1000)
	{
		LeftDigit = Value / 100;
		RightSpriteComponent->SetSprite(Sprite0);
	}
	else
	{
		LeftDigit = Value / 1000;
		RightSpriteComponent->SetSprite(Sprite00);
	}
		
	switch(LeftDigit)
	{
		case 1:
			LeftSpriteComponent->SetSprite(Sprite10);
			break;
		case 2:
			LeftSpriteComponent->SetSprite(Sprite20);
			break;
		case 4:
			LeftSpriteComponent->SetSprite(Sprite40);
			break;
		case 7:
			LeftSpriteComponent->SetSprite(Sprite70);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Can't match up value with bonus sprites."));
			break;
	}
}
