//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeartActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API AHeartActor : public AActor
{

	GENERATED_BODY()

public:

	AHeartActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float DesiredX = 0.0f;
	
	float DesiredZ = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float InterpolationSpeed = 20.0f;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* PickupSound;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Hearts = 5;

	UPROPERTY(EditDefaultsOnly)
	float SineAmplitude = 8.0f;

	UPROPERTY(EditDefaultsOnly)
	float SineFrequency = 0.5f;
		
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;
	
};
