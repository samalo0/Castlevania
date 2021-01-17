//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/25/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupSpawnActor.generated.h"

class ABurnOutActor;
class ACastlevaniaCameraActor;
class AShotPickupActor;
class AWhipUpgradeActor;
class UBoxComponent;
class UPointLightComponent;

UCLASS(HideCategories = (Replication, Rendering, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API APowerupSpawnActor : public AActor
{

	GENERATED_BODY()

public:

	APowerupSpawnActor();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SpawnPowerup();
	
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<AActor> ClassToSpawnOnDestroy;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShotPickupActor> ShotPickupClass;
	
	UPROPERTY(EditDefaultsOnly, meta = (MakeEditWidget = true))
	FVector SpawnOffset = FVector(0.0f, 0.0f, 10.0f);

	UPROPERTY(EditInstanceOnly)
	bool bSpawnShotIfNotFullyUpgraded = false;
	
	UPROPERTY(EditInstanceOnly)
	bool bSpawnWhipUpgradeIfNotFullyUpgraded = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWhipUpgradeActor> WhipUpgradeClass;
	
#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

#pragma endregion
	
};

