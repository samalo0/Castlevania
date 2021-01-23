//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

class ACastlevaniaCameraActor;
class ACastlevaniaGameModeBase;
class AShotPickupActor;
class UBoxComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class AHitEffectActor;
class AWhipUpgradeActor;

UCLASS(HideCategories = (Tick, Replication, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API AEnemyActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AEnemyActor();

	FORCEINLINE int32 GetDamageToPlayer() const { return DamageToPlayer; }

	UFUNCTION()
	virtual void HitWithWeapon(int32 Damage, bool bPlaySound, const FVector WeaponLocation);

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
    virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex);
	
	UFUNCTION()
    virtual void OnFinishedPlaying();

	void SpawnDrop();
	
	UFUNCTION()
	virtual void TimeStop(const bool bIsActive) { bIsTimeStopped = bIsActive; }

	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | General")
	int32 DamageToPlayer = 2;

	bool bHolyWaterBurning = false;

	FTimerHandle HolyWaterTimer;
	
	FVector LocationFloat = FVector::ZeroVector;
	
	bool bIsTimeStopped = false;
	
#pragma region Components

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
#pragma endregion

#pragma region Damaged

protected:

	void SpawnHitEffect(const FVector SpawnLocation);
	
	// Flipbook to play when the enemy is destroyed.
	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	UPaperFlipbook* BurnOutFlipbook;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	TArray<UClass*> ClassesToSpawnOnDestroy;

	float HitCooldown = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	float HitCooldownTime = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	TSubclassOf<AHitEffectActor> HitEffectActorClass;
	
	// The amount of damage that the enemy can take before being destroyed.
	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	int32 Life = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	float ProbabilityOfDrop = 0.4f;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	float ProbabilityOfShotDrop = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	int32 Score = 100;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	TSubclassOf<AShotPickupActor> ShotUpgradeClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnemyActor | Damaged")
	TSubclassOf<AWhipUpgradeActor> WhipUpgradeClass;

	bool bWasHitWithWhip = false;
	
#pragma endregion

#pragma region Reference

protected:
	
	// Transient reference to the camera.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "EnemyActor | Reference")
	ACastlevaniaCameraActor* Camera;

	// Transient reference to the game mode.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "EnemyActor | Reference")
	ACastlevaniaGameModeBase* GameMode;
	
#pragma endregion
	
};
