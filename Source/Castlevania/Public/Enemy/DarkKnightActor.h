//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/16/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "DarkKnightActor.generated.h"

/**
 * Child class of enemy for the dark knight (a wandering enemy who takes 4 damage to destroy).
 */
UCLASS()
class CASTLEVANIA_API ADarkKnightActor : public AEnemyActor
{

	GENERATED_BODY()

public:
	
	ADarkKnightActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;
	
	float Accumulator = 0.0f;

	// How often to line trace to make sure the knight isn't moving into open space.
	UPROPERTY(EditDefaultsOnly)
	float LineTraceInterval = 0.5f;
	
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 25.0f;

	UPROPERTY(EditDefaultsOnly)
	float ProbabilityOfTurningAround = 0.15f;
	
};
