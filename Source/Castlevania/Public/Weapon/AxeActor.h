//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponActor.h"
#include "AxeActor.generated.h"

class UBoxComponent;
class UPaperFlipbookComponent;

UCLASS()
class CASTLEVANIA_API AAxeActor : public AWeaponActor
{

	GENERATED_BODY()

public:

	AAxeActor();
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent; 
	
	UPROPERTY(EditDefaultsOnly)
	float GravityAcceleration = -480.0f;
	
	UPROPERTY(EditDefaultsOnly)
	FVector InitialVelocity = FVector(50.0f, 0.0f, 200.0f);

	FVector Velocity = FVector::ZeroVector;
	
};
