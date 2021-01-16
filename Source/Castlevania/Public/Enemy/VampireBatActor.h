//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/31/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "VampireBatActor.generated.h"

class ACastlevaniaPawn;

UCLASS()
class CASTLEVANIA_API AVampireBatActor : public AEnemyActor
{

	GENERATED_BODY()

	AVampireBatActor();

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound) override;
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void SetMovementBasedOnPlayerLocation();
	
	virtual void TimeStop(bool bEnable) override;
	
	UPROPERTY(EditDefaultsOnly)
	float Amplitude = 16.0f;

	// Used to accumulate time while moving, for sinusoidal movement. Allows pausing movement.
	float Accumulator = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float Frequency = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* FlyingFlipbook;
	
	UPROPERTY(EditDefaultsOnly)
	float InitialMovementSpeed = 100.0f;

	float InitialZ = 0.0f;
	
	float MovementSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	bool bStartHanging = false;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBoxComponent;
	
};
