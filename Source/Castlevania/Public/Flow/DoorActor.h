//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/30/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

class ACastlevaniaPawn;
class ACastlevaniaCameraActor;
class UPaperSpriteComponent;
class UBoxComponent;
class UPaperSprite;

UENUM()
enum class EDoorState : uint8
{
	Closed,
	Opening,
	Open,
	Closing,
	FinalClosed
};

UCLASS(HideCategories = (Tick, Replication, Rendering, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API ADoorActor : public AActor
{

	GENERATED_BODY()
	
public:	

	ADoorActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	float CameraInterpolationSpeed = 64.0f;
	
	UPROPERTY(EditInstanceOnly)
	ACastlevaniaCameraActor* CameraReference;
	
	UPROPERTY(Transient)
	UPaperSprite* ClosedSprite;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* DoorSound;

	float FrameAccumulator = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float FrameTime = 0.5f;

	UPROPERTY(EditInstanceOnly)
	float NewCameraMinimumX = 0.0f;

	UPROPERTY(EditInstanceOnly)
	float NewCameraMaximumX = 0.0f;
	
	UPROPERTY(EditInstanceOnly)
	float NewCameraX = 0.0f;
	
	UPROPERTY(EditInstanceOnly)
	float NewPawnX = 0.0f;

	// The new stage number to set when going through the door.
	UPROPERTY(EditInstanceOnly)
	int32 NewStage = 0;
	
	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Open1Sprite;

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Open2Sprite;

	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;
	
	UPROPERTY(VisibleInstanceOnly)
	EDoorState State = EDoorState::Closed;

#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;
	
#pragma endregion
	
};
