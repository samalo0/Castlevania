//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"

#include "CastlevaniaStatics.h"
#include "Pickup/PickupActor.h"
#include "WeaponPickupActor.generated.h"

UCLASS()
class CASTLEVANIA_API AWeaponPickupActor : public APickupActor
{

	GENERATED_BODY()

protected:

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType = EWeaponType::Dagger;
	
};
