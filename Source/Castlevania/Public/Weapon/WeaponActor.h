//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class ABonusPointsEffectActor;

UCLASS(HideCategories = (Replication, Rendering, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API AWeaponActor : public AActor
{

	GENERATED_BODY()

public:

	FORCEINLINE int32 GetWeaponDamage() const { return WeaponDamage; }
	
	virtual void Hit();
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(Transient)
	ABonusPointsEffectActor* BonusPointsEffectActor;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABonusPointsEffectActor> BonusPointsEffectClass;

	// Float location used for interpolation; to prevent moving the actor until it's moved a full pixel.
	FVector LocationFloat = FVector::ZeroVector;
	
	int32 NumberOfEnemiesKilled = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 WeaponDamage = 1;
	
};
