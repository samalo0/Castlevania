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

UCLASS(HideCategories = (Tick, Replication, Rendering, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API ACrusherActor : public AActor
{

	GENERATED_BODY()
	
public:	

	ACrusherActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleInstanceOnly)
	FVector LocationFloat;
	
	UPROPERTY(EditAnywhere)
	float LowerVelocity = -32.0f;
	
	UPROPERTY(VisibleInstanceOnly)
	bool bMovingDown = true;

	UPROPERTY(EditAnywhere)
	float RaiseVelocity = 32.0f;
	
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector StopRelativeLocation = FVector(0.0f, 0.0f, -48.0f);
	
#pragma region Components

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;
	
#pragma endregion
	
};
