//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/23/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "BossEnemyActor.generated.h"

/**
 * Child class of enemy for boss enemies.
 */
UCLASS()
class CASTLEVANIA_API ABossEnemyActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;

    virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void TriggerBattle() {}

protected:

	virtual void OnFinishedPlaying() override;
	
};
