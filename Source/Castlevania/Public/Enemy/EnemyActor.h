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

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound);

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
    virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex);
	
	UFUNCTION()
    virtual void OnFinishedPlaying();

	void RoundFlipbookLocation() const;

	void SpawnDrop();

	UFUNCTION()
	virtual void TimeStop(bool bEnable) {}
	
	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbook* BurnOutFlipbook;

	UPROPERTY(Transient)
	ACastlevaniaCameraActor* Camera;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<UClass*> ClassesToSpawnOnDestroy;

	UPROPERTY(EditDefaultsOnly)
	int32 DamageToPlayer = 2;

	UPROPERTY(Transient)
	ACastlevaniaGameModeBase* GameMode;
	
	UPROPERTY(EditDefaultsOnly)
	float ProbabilityOfDrop = 0.4f;

	UPROPERTY(EditDefaultsOnly)
	float ProbabilityOfShotDrop = 0.5f;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly)
	int32 Score = 100;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShotPickupActor> ShotUpgradeClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWhipUpgradeActor> WhipUpgradeClass;

	bool bWasHitWithWhip = false;
	
#pragma region Components

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
#pragma endregion
	
};
