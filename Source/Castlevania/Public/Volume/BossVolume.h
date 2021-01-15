//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/4/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "BossVolume.generated.h"

class AEnemyActor;

UCLASS()
class CASTLEVANIA_API ABossVolume : public AVolume
{

	GENERATED_BODY()

public:

	ABossVolume();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditInstanceOnly)
	TArray<AVolume*> BlockingVolumesToEnable;

	UPROPERTY(EditInstanceOnly)
	AEnemyActor* BossToEnable;
	
	UPROPERTY(EditInstanceOnly)
	float CameraMaximumX = 0.0f;
	
	UPROPERTY(EditInstanceOnly)
	float CameraMinimumX = 0.0f;

};
