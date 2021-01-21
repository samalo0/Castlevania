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

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;
	
protected:

	virtual void BeginPlay() override;

	void PrepareToFire();
	
	void RepeatFire();
	
	void SpawnProjectile();
	
	void StartFire();
	
	UPROPERTY(Transient)
	UPaperFlipbook* BlueFlipbook;

	UPROPERTY(EditDefaultsOnly)
	float DelayBetweenFiring = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float DelayForFirstFire = 0.25f;

	UPROPERTY(EditDefaultsOnly)
	float DelayForRepeatFire = 0.75f;
	
	int32 FireCount = 0;

	bool bFireRight = true;
	
	UPROPERTY(EditDefaultsOnly, meta = (MakeEditWidget = true))
	FTransform LeftSideFireOffset = FTransform(FVector(0.0f, 0.0f, 0.0f));
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfProjectilesToFire = 3;
	
	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectileActor> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* RedFlipbook;

	UPROPERTY(EditDefaultsOnly, meta = (MakeEditWidget = true))
	FTransform RightSideFireOffset = FTransform(FVector(0.0f, 0.0f, 0.0f));
	
	UPROPERTY(EditDefaultsOnly)
	float StartFireTime = 3.0f;

	FTimerHandle TimerHandle;
	
};
