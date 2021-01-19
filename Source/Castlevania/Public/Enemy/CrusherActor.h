//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/18/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrusherActor.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API ACrusherActor : public AActor
{

	GENERATED_BODY()
	
public:	

	ACrusherActor();
	
protected:

	virtual void BeginPlay() override;

	static void EnableSpriteComponent(bool bEnableNotDisable, UPaperSpriteComponent* Component);

	void IncrementState();

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* ArmSprite;
	
	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* BaseSprite;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin=1, ClampMax=3))
	int32 MaximumLower = 3;

	UPROPERTY(EditAnywhere)
	bool bMovingDown = true;
	
	UPROPERTY(EditAnywhere)
	float StateChangeSpeed = 0.5f;

	int32 State = 0;
	
#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent1;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent2;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent3;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent4;
	
#pragma endregion
	
};
