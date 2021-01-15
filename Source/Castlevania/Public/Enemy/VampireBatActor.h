//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/31/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "VampireBatActor.generated.h"

class ACastlevaniaPawn;
UCLASS()
class CASTLEVANIA_API AVampireBatActor : public AEnemyActor
{

	GENERATED_BODY()

	AVampireBatActor();

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound) override;
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	virtual void TimeStop(bool bEnable) override;
	
	UPROPERTY(EditDefaultsOnly)
	float Amplitude = 16.0f;

	UPROPERTY(EditDefaultsOnly)
	float Frequency = 0.5f;
	
	UPROPERTY(EditDefaultsOnly)
	float InitialMovementSpeed = 100.0f;

	float InitialZ = 0.0f;
	
	float MovementSpeed = 0.0f;
	
	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;
	
};
