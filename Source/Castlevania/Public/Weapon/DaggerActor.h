//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/24/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponActor.h"
#include "DaggerActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API ADaggerActor : public AWeaponActor
{

	GENERATED_BODY()
	
public:	

	ADaggerActor();

	virtual void Hit() override { Destroy(); }
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	float InterpolationSpeed = 250.0f;

	FVector FloatLocation;
	FVector IntegerLocation;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* SpawnSound;
	
	UPROPERTY(EditDefaultsOnly)
	float TravelSpeed = 200.0f;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;
	
};
