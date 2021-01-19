//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/18/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrusherActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API ACrusherActor : public AActor
{

	GENERATED_BODY()
	
public:	

	ACrusherActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

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
