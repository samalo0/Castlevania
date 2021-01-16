//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/16/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "DarkKnightActor.generated.h"

/**
 * Child class of enemy for the dark knight (a wandering enemy who takes 4 damage to destroy).
 */
UCLASS()
class CASTLEVANIA_API ADarkKnightActor : public AEnemyActor
{

	GENERATED_BODY()

public:
	
	ADarkKnightActor();
	
};
