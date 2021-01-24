//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/23/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BossEnemyActor.h"
#include "MedusaActor.generated.h"

class AMedusaSnakeActor;

UENUM()
enum class EMedusaState : uint8
{
	WaitingForTrigger,
};

/**
 * Child class of enemy for the Medusa boss.
 */
UCLASS()
class CASTLEVANIA_API AMedusaActor : public ABossEnemyActor
{

	GENERATED_BODY()

public:

	AMedusaActor();

	virtual void Tick(float DeltaSeconds) override;

	virtual void TriggerBattle() override;
	
protected:

	void OnBossFightStart();
	
	UPROPERTY(EditDefaultsOnly)
	float BossFightDelay = 2.0f;
	
	UPROPERTY(EditInstanceOnly)
	AActor* HeadSpriteToDestroy;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMedusaSnakeActor> MedusaSnakeClass; 

	UPROPERTY(VisibleInstanceOnly)
	EMedusaState State = EMedusaState::WaitingForTrigger;
	
};
