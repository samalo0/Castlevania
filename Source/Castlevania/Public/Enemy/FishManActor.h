//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/2/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "FishManActor.generated.h"

class ACastlevaniaPawn;
class AFireBallActor;

UENUM()
enum class EFishManState
{
	JumpingOutOfWater,
	FallingToLand,
	Walking,
	Attacking,
	RecoveringFromAttack,
	JumpingIntoWater,
};

UCLASS()
class CASTLEVANIA_API AFishManActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	AFishManActor();

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound) override;
	
	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

	virtual void TimeStop(bool bEnable) override;
	
	float Accumulator = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float AttackRecoveryTime = 0.5f;
	
	float AttackTime = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* AttackingFlipbook;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFireBallActor> FireBallClass;

	UPROPERTY(EditDefaultsOnly)
	FVector FireballSpawnOffset = FVector(11.0f,0.0f,5.0f);
	
	UPROPERTY(EditDefaultsOnly)
	float GravityAcceleration = -480.0f;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* LandSound;
	
	UPROPERTY(EditDefaultsOnly)
	float MaximumAttackTime = 2.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MinimumAttackTime = 0.25f;

	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;
	
	UPROPERTY(EditDefaultsOnly)
	float InitialVelocityZ = 375.0f;

	float RaycastAccumulator = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float RaycastInterval = 0.2f;
	
	UPROPERTY(VisibleInstanceOnly)
	EFishManState State = EFishManState::JumpingOutOfWater;
	
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 16.0f;

	UPROPERTY(Transient)
	UPaperFlipbook* WalkingFlipbook;
};

