//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/25/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "EnterCastleVolume.generated.h"

class ACastlevaniaPawn;
class ACastlevaniaCameraActor;

UCLASS()
class CASTLEVANIA_API AEnterCastleVolume : public AVolume
{

	GENERATED_BODY()

public:

	AEnterCastleVolume();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnEndOverlapDelayed();

	UPROPERTY(EditInstanceOnly)
	ACastlevaniaCameraActor* CameraReference;
	
	UPROPERTY(EditAnywhere	)
	float CameraFadeTime = 0.1f;
	
	UPROPERTY(EditAnywhere)
	USoundBase* EnterSound;

	UPROPERTY(EditAnywhere)
	FVector NewCameraLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float NewCameraMaximumX = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float NewCameraMinimumX = 0.0f;
	
	UPROPERTY(EditAnywhere)
	FVector NewPawnLocation = FVector::ZeroVector;
	
	UPROPERTY(Transient)
	ACastlevaniaPawn* Pawn;
	
	UPROPERTY(EditDefaultsOnly)
	float SwitchDelay = 0.3f;
	
};

