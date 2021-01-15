//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/30/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathVolume.generated.h"

class UBoxComponent;

UCLASS()
class CASTLEVANIA_API ADeathVolume : public AActor
{

	GENERATED_BODY()

public:

	ADeathVolume();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnBeginOverlapAfterDelay();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
};
