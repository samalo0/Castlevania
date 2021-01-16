//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "PhantomBatActor.generated.h"

class ACastlevaniaCameraActor;
class ACastlevaniaPawn;

UENUM()
enum class EPhantomBatState
{
	Undefined,
	
	InitialWait,
	MoveToDestination,
	WaitRandomTime,
	GetPlayerLocation,
	GetLocationAwayFromPlayer,
	Death,
};

UCLASS()
class CASTLEVANIA_API APhantomBatActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	APhantomBatActor();

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound) override;
	
	virtual void Tick(float DeltaSeconds) override;	

protected:

	virtual void BeginPlay() override;

	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	virtual void OnFinishedPlaying() override;
	
	virtual void TimeStop(bool bEnable) override;
	
	FVector Destination = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly)
	float FastMovementSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	FVector FlyingCollisionBoxExtent = FVector(8.0f, 10.0f, 8.0f);
	
	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* FlyingFlipbook;
	
	bool bLastTowardsPlayer = false;
	
	UPROPERTY(EditDefaultsOnly)
	float MaximumWaitTime = 1.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MinimumWaitTime = 0.1f;

	float MovementSpeed = 0.0f;
	
	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;
	
	UPROPERTY(EditDefaultsOnly)
	float SlowMovementSpeed = 50.0f;
	
	UPROPERTY(VisibleInstanceOnly)
	EPhantomBatState State = EPhantomBatState::InitialWait;
	
	float WaitTime = 0.0f;
	
};

