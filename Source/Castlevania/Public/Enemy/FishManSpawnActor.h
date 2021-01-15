//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemySpawnActor.h"
#include "FishManSpawnActor.generated.h"

class ASplashEffectActor;
UCLASS()
class CASTLEVANIA_API AFishManSpawnActor : public AEnemySpawnActor
{

	GENERATED_BODY()

protected:

	virtual void SpawnEnemy(FTransform RelativeTransform) override;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* SplashSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASplashEffectActor> SplashEffectClass;
	
};
