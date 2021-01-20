//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/19/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "GhostActor.generated.h"

class ACastlevaniaPawn;

/**
 * Child enemy that is a ghost, which follows the player slowly.
 */
UCLASS()
class CASTLEVANIA_API AGhostActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	AGhostActor();

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float InterpolationSpeed = 40.0f;

	FVector LocationFloat;
	
	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn; 
	
};
