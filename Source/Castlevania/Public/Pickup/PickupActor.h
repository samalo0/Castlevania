//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API APickupActor : public AActor
{

	GENERATED_BODY()
	
public:

	APickupActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
	
	float DesiredZ = 0.0f;

	// Gravity acceleration, in pixels per second squared.
	UPROPERTY(EditDefaultsOnly)
	float GravityAcceleration = -480.0f;

	FVector LocationFloat = FVector::ZeroVector;
	
	bool bOnGround = false;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* PickupSound;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;

	float VelocityZ = 0.0f;
};
