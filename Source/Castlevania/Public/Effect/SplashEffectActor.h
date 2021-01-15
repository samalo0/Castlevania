//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/1/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplashEffectActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CASTLEVANIA_API ASplashEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASplashEffectActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex);
	
	UPROPERTY(EditDefaultsOnly)
	float GravityAcceleration = -480.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float InitialVelocityX = 50.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float InitialVelocityZ = 200.0f;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;
	
	FVector Velocity = FVector::ZeroVector;
	
};
