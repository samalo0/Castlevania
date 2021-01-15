//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Spawn/PowerupSpawnActor.h"
#include "LightPowerupSpawnActor.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;

UCLASS()
class CASTLEVANIA_API ALightPowerupSpawnActor : public APowerupSpawnActor
{

	GENERATED_BODY()

public:

	ALightPowerupSpawnActor();
	
protected:

	virtual void BeginPlay() override;;

	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UFUNCTION()
    void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
    void OnFinishedPlaying();
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* BurnOutFlipBook;
	
	UPROPERTY(EditDefaultsOnly)
	float MaximumLight = 1.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MinimumLight = 0.1f;

#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* PointLightComponent; 

#pragma endregion
	
};
