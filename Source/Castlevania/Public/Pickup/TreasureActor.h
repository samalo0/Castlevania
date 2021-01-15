//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"

#include "BonusPointsEffectActor.h"
#include "PickupActor.h"
#include "TreasureActor.generated.h"

UCLASS()
class CASTLEVANIA_API ATreasureActor : public APickupActor
{

	GENERATED_BODY()
	
protected:

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABonusPointsEffectActor> BonusPointsEffectClass;

	UPROPERTY(EditDefaultsOnly)
	FVector LocationOffset = FVector(0.0f, 0.0f, 32.0f);
	
	UPROPERTY(EditDefaultsOnly)
	int32 Score = 100;
	
};
