//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/17/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "MedusaHeadActor.generated.h"

/**
 * Medusa head oscillating enemy class. 
 */
UCLASS()
class CASTLEVANIA_API AMedusaHeadActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	AMedusaHeadActor();
	
	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;
	
	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;
	
	virtual void TimeStop(bool bEnable) override;
	
	float Accumulator = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float Amplitude = 48.0f;

	UPROPERTY(EditDefaultsOnly)
	float Frequency = 0.5f;
	
	FVector InitialLocation = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 64.0f;
	
};
