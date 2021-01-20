//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/19/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "DragonCannonActor.generated.h"

class ACastlevaniaPawn;
class AEnemyProjectileActor;

/**
 * Child enemy class for the dragon cannon (a stationary, projectile firing) enemy.
 */
UCLASS()
class CASTLEVANIA_API ADragonCannonActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	ADragonCannonActor();

protected:

	virtual void BeginPlay() override;

	void RepeatFire();
	
	void SpawnProjectile();
	
	void StartFire();
	
	UPROPERTY(Transient)
	UPaperFlipbook* BlueFlipbook;

	int32 FireCount = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfProjectilesToFire = 3;
	
	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectileActor> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* RedFlipbook;

	UPROPERTY(EditDefaultsOnly)
	float RepetitiveFireTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float StartFireTime = 3.0f;

	FTimerHandle TimerHandle;
	
};
