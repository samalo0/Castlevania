//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnActor.generated.h"

class UBoxComponent;
class ACastlevaniaCameraActor;
class AEnemyActor;
class UPaperFlipbookComponent;

UCLASS()
class CASTLEVANIA_API AEnemySpawnActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AEnemySpawnActor();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TimeStop(bool bEnable);
	
	UPROPERTY(EditInstanceOnly)
	ACastlevaniaCameraActor* CameraActor;

	bool bIsSpawning = false;
	
	bool bIsTimeStopped = false;
	
#pragma region Components

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ActiveSpawnBoxComponent;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
#endif
	
#pragma endregion

#pragma region Spawn

protected:

	void OnSpawnDurationElapsed();

	virtual void SpawnEnemy(FTransform RelativeTransform);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyActor> EnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MaximumInstances = 5; 

	UPROPERTY(EditAnywhere)
	float MaximumSpawnTime = 1.5f;

	UPROPERTY(EditAnywhere)
	float MinimumSpawnTime = 0.25f;
	
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	TArray<FTransform> SpawnRelativeTransforms;
	
	UPROPERTY(Transient, VisibleInstanceOnly)
	TArray<AActor*> SpawnedActors;
	
#pragma endregion
	
};

