//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/23/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BossEnemyActor.h"
#include "MedusaActor.generated.h"

class AMedusaSnakeActor;

UENUM()
enum class EMedusaState : uint8
{
	Waiting,
	MovingToDestination,
	GetDestinationNearPlayer,
	GetDestinationAwayFromPlayer,
	SpawnSnakes,
	Death,
};

/**
 * Child class of enemy for the Medusa boss.
 */
UCLASS()
class CASTLEVANIA_API AMedusaActor : public ABossEnemyActor
{

	GENERATED_BODY()

public:

	AMedusaActor();

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void TriggerBattle() override;
	
protected:

	void OnBossFightStart();

	void SpawnSnakes(bool bRightSide);

	virtual void TimeStop(const bool bIsActive) override;
	
	UPROPERTY(EditDefaultsOnly)
	float BossFightDelay = 2.0f;
	
	UPROPERTY(EditInstanceOnly)
	AActor* HeadSpriteToDestroy;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMedusaSnakeActor> MedusaSnakeClass; 

	UPROPERTY(EditDefaultsOnly)
	float MaximumMovementDistance = 6 * 16.0f;

	// Speed that medusa can move in pixels/second.
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 64.0f;
		
	UPROPERTY(VisibleInstanceOnly)
	EMedusaState State = EMedusaState::Waiting;

	UPROPERTY(EditDefaultsOnly)
	FVector2D WaitTimeLimits = FVector2D(0.5f, 2.0f);

	UPROPERTY(EditDefaultsOnly)
	FVector2D XClamp = FVector2D(-1488.0f, -1272.0f);
	
	UPROPERTY(EditDefaultsOnly)
	FVector2D ZClamp = FVector2D(632.0f - 16.0f, 632.0f + 16.0f);
	
private:

	FVector Destination = FVector::ZeroVector;

	bool bLastTowardsPlayer = false;
	
	float WaitTime = 0.0f;
	
};
