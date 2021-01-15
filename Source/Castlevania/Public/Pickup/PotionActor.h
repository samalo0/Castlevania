//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Pickup/PickupActor.h"
#include "PotionActor.generated.h"

UCLASS()
class CASTLEVANIA_API APotionActor : public APickupActor
{

	GENERATED_BODY()

protected:

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void OnPotionExpired();

	UPROPERTY(EditDefaultsOnly)
	float PotionDuration = 6.0f;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* PotionExpiredSound;
	
};
