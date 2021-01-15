//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/27/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StairsActor.generated.h"

class UArrowComponent;
class UBoxComponent;
class UPaperSpriteComponent;

UCLASS(HideCategories = (Tick, Replication, Rendering, Input, LOD, Cooking))
class CASTLEVANIA_API AStairsActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AStairsActor();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    void OnDownBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnDownBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnUpBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnUpBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	bool bAscendsFromLeftToRight = true;
	
#pragma region Components

protected:
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* DownBoxComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* DownOffsetComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* DownSpriteComponent;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* UpBoxComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* UpOffsetComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* UpSpriteComponent;
	
#pragma endregion
	
};
