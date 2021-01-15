//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BonusPointsEffectActor.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API ABonusPointsEffectActor : public AActor
{

	GENERATED_BODY()
	
public:	

	ABonusPointsEffectActor();

	void SetValue(int32 Value);
	
protected:

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite0;

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite00;
	
	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite10;

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite20;

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite40;

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite70;

	
#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* LeftSpriteComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* RightSpriteComponent;
	
#pragma endregion
	
};
