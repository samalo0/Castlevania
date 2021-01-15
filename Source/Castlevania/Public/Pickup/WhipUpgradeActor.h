//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Pickup/PickupActor.h"
#include "WhipUpgradeActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API AWhipUpgradeActor : public APickupActor
{

	GENERATED_BODY()

protected:

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
