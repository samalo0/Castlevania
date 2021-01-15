//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Pickup/PickupActor.h"
#include "CrossActor.generated.h"

class ADirectionalLight;

UCLASS()
class CASTLEVANIA_API ACrossActor : public APickupActor
{

	GENERATED_BODY()
	
protected:
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void Flash();

	int32 CurrentNumberOfFlashes = 0;

	UPROPERTY(Transient)
	ADirectionalLight* DirectionalLight;

	UPROPERTY(EditDefaultsOnly)
	float FlashTime = 0.5f;
	
	float InitialIntensity = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float IntensityMultiplier = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfFlashes = 5;
	
};
