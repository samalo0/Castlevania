//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/2/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Pickup/PickupActor.h"
#include "ShotPickupActor.generated.h"

class UPaperSprite;

UCLASS()
class CASTLEVANIA_API AShotPickupActor : public APickupActor
{

	GENERATED_BODY()

public:

	void SetIsTripleShot(bool bIs);
	
protected:

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	bool bIsTripleShot = false;
	
	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* TripleShotSprite;
	
};
