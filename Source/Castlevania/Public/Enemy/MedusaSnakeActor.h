//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/23/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "MedusaSnakeActor.generated.h"

/**
 * Snakes spawned by the Medusa boss.
 */
UCLASS()
class CASTLEVANIA_API AMedusaSnakeActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	AMedusaSnakeActor();

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	bool RaycastToGround(FVector& HitLocation, const float Distance) const;
	
	UPROPERTY(EditDefaultsOnly)
	float FallRaycastInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float GravityAcceleration = -480.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 64.0f;

private:

	bool bFalling = false;
	
	float FallRaycastAccumulator = 0.0f;

	float VelocityZ = 0.0f;
};
