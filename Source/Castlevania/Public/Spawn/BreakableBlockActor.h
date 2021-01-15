//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Spawn/PowerupSpawnActor.h"
#include "BreakableBlockActor.generated.h"

class ASplashEffectActor;
class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API ABreakableBlockActor : public APowerupSpawnActor
{

	GENERATED_BODY()

public:

	ABreakableBlockActor();

protected:

	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASplashEffectActor> BreakEffectClass;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;
	
};
