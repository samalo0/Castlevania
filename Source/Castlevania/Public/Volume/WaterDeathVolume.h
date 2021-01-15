//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/1/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Volume/DeathVolume.h"
#include "WaterDeathVolume.generated.h"

UCLASS()
class CASTLEVANIA_API AWaterDeathVolume : public ADeathVolume
{

	GENERATED_BODY()

public:

	AWaterDeathVolume();
	
protected:

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SplashEffectClass;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* SplashSound;
	
};
