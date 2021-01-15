//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitEffectActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API AHitEffectActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AHitEffectActor();

protected:
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;
	
};
