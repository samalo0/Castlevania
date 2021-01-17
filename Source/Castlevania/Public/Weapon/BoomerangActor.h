//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponActor.h"
#include "BoomerangActor.generated.h"

class ACastlevaniaCameraActor;
class UBoxComponent;
class UPaperFlipbookComponent;

UENUM()
enum class EBoomerangState
{
	Idle,

	StartGoingRight,
	SlowingDownRight,
	SpeedingUpLeft,
	FinalGoingLeft,

	StartGoingLeft,
	SlowingDownLeft,
	SpeedingUpRight,
	FinalGoingRight
};

UCLASS()
class CASTLEVANIA_API ABoomerangActor : public AWeaponActor
{

	GENERATED_BODY()

public:

	ABoomerangActor();

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateState(const float DeltaSeconds);
	
	UPROPERTY(Transient)
	ACastlevaniaCameraActor* Camera;

	float DesiredX = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float InitialInterpolationSpeed = 128.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float Range = 96.0f;

	UPROPERTY(VisibleInstanceOnly)
	EBoomerangState State = EBoomerangState::Idle;

	UPROPERTY(EditDefaultsOnly)
	float TurnAroundAcceleration = -256.0f;

	float Velocity = 100.0f;
	
#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
#pragma endregion
	
};
