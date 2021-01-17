//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "ZombieActor.generated.h"

UCLASS()
class CASTLEVANIA_API AZombieActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	AZombieActor();

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation) override;

	virtual void TimeStop(bool bEnable) override;
	
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 50.0f;
	
};
