//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/1/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "TransitionVolume.generated.h"

class ACastlevaniaPawn;
class ACastlevaniaCameraActor;

UCLASS()
class CASTLEVANIA_API ATransitionVolume : public AVolume
{

	GENERATED_BODY()

public:

	ATransitionVolume();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnBeginOverlapAfterDelay() const;
	
	UPROPERTY(EditInstanceOnly)
	ACastlevaniaCameraActor* CameraReference;

	UPROPERTY(EditInstanceOnly)
	float CameraFadeTime = 0.1f;
	
	UPROPERTY(EditInstanceOnly)
	FVector NewCameraLocation = FVector::ZeroVector;

	UPROPERTY(EditInstanceOnly)
	float NewCameraMaximumX = 0.0f;
	
	UPROPERTY(EditInstanceOnly)
	float NewCameraMinimumX = 0.0f;
	
	UPROPERTY(EditInstanceOnly)
	FVector NewPawnLocation = FVector::ZeroVector;

	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;
	
};
