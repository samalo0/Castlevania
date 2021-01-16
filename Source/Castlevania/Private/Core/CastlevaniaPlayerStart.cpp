//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/16/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Core/CastlevaniaPlayerStart.h"

#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"

ACastlevaniaPlayerStart::ACastlevaniaPlayerStart(const FObjectInitializer& ObjectInitializer) : APlayerStart(ObjectInitializer)
{
	#if WITH_EDITORONLY_DATA

		SpriteComponent = CreateEditorOnlyDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
		if(SpriteComponent != nullptr)
		{
			SpriteComponent->SetupAttachment(GetRootComponent());
			SpriteComponent->SetCollisionProfileName("NoCollision");
			SpriteComponent->SetHiddenInGame(true);
			SpriteComponent->SetMobility(EComponentMobility::Static);
		}
	
	#endif

	GetCapsuleComponent()->SetCapsuleSize(8.0f, 16.0f);
}
