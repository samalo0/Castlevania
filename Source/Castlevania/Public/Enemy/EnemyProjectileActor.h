//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"

#include "PaperFlipbookComponent.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectileActor.generated.h"

class UPaperSpriteComponent;

UCLASS(HideCategories = (Replication, Rendering, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API AEnemyProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemyProjectileActor();

	FORCEINLINE int32 GetDamageToPlayer() const { return DamageToPlayer; }
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	void BurnOutWithFlipbook();
	
	UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex);

	UFUNCTION()
	void OnFinishedPlaying();
	
	UPROPERTY(EditDefaultsOnly)
	int32 DamageToPlayer = 4;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* DestroySound;
	
	UPROPERTY(EditDefaultsOnly)
	float InitialVelocity = 100.0f;

	FVector LocationFloat = FVector::ZeroVector;
	
	float Velocity = 0.0f;

#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;

#pragma endregion
	
};

