//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponActor.h"
#include "HolyWaterActor.generated.h"

class UBoxComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;

UCLASS()
class CASTLEVANIA_API AHolyWaterActor : public AWeaponActor
{

	GENERATED_BODY()

public:

	AHolyWaterActor();

	FORCEINLINE float GetRepeatDamageInterval() const { return RepeatDamageInterval; }
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnFinishedPlaying();
	
	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* BurnFlipbook;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* BurnSound;
	
	UPROPERTY(EditDefaultsOnly)
	float GravityAcceleration = -480.0f;
	
	UPROPERTY(EditDefaultsOnly)
	FVector InitialVelocity = FVector(50.0f, 0.0f, 100.0f);

	// How long it takes holy water to damage an enemy more than once, if it remains overlapping.
	UPROPERTY(EditDefaultsOnly)
	float RepeatDamageInterval = 0.5f;
	
	FVector Velocity = FVector::ZeroVector;

#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent; 
	
#pragma endregion
	
};
