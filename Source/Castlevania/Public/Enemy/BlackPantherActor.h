//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyActor.h"
#include "BlackPantherActor.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;

UENUM()
enum class EBlackPantherState : uint8
{
	Idle,
	Jump,
	Run,

	Burning,
	
	Max UMETA(Hidden)
};

UCLASS()
class CASTLEVANIA_API ABlackPantherActor : public AEnemyActor
{

	GENERATED_BODY()

public:

	ABlackPantherActor();

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

	virtual void HitWithWeapon(int32 Damage, bool bPlaySound) override;
	
	UFUNCTION()
	void OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void TimeStop(bool bEnable) override;
	
	UPROPERTY(EditDefaultsOnly)
	float GravityAcceleration = -480.0f;
	
	UPROPERTY(EditDefaultsOnly)
	FVector InitialJumpVelocity = FVector(100.0f, 0.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly)
	float InitialMovementSpeed = 125.0f;
	
	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* JumpSprite;

	FVector JumpVelocity = FVector::ZeroVector;
	
	float MovementSpeed = 125.0f;

	float RaycastAccumulator = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float RaycastInterval = 0.1f;
	
	UPROPERTY(VisibleInstanceOnly)
	EBlackPantherState State = EBlackPantherState::Idle;
	
#pragma region Components
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBoxComponent;
	
#pragma endregion
	
};
