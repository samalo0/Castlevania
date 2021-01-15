//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/30/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRespawnActor.generated.h"

class UPaperFlipbookComponent;
class AEnemyActor;
class UBoxComponent;

UCLASS(HideCategories = (Tick, Replication, Rendering, Collision, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API AEnemyRespawnActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AEnemyRespawnActor();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyActor> EnemyActorClass;

	UPROPERTY(Transient)
	AEnemyActor* EnemyReference;

#pragma region Components

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
#endif
	
#pragma endregion
	
};
