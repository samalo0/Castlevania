//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CastlevaniaPawnMovementComponent.generated.h"

class ACastlevaniaPawn;

/**
 * Movement component for 2D character.
 */
UCLASS()
class CASTLEVANIA_API UCastlevaniaPawnMovementComponent : public UPawnMovementComponent
{

	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	void Hurt(AActor* DamageCauser);
	
	void Jump();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Set during playback of whipping animation.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsAttacking = false;
	
	// Keeps track if the character is crouched (pressing down while feet on the ground).
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsCrouched = false;

	// If the character has died.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsDead = false;
	
	// Forces the character to walk to the right.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bForceMoveRight = false;
	
	// Keeps track if the character has been hurt and is flying back, unable to be controlled.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsHurt = false;

	// Keeps track if the character is in the air due to a jump.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsJumping = false;
	
	// Keeps track if the character is invincible.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsInvincible = false;
	
	// Keeps track of if the character's feet are on the ground.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsOnGround = false;
	
	// Keeps track of if the character has landed after being hurt, and is crouching to recover for a short time.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | General")
	bool bIsRecovering = false;
	
protected:

	void UpdateHorizontalMovement(FVector InputVector, float DeltaTime);

	void UpdateStairsMovement(FVector InputVector, float DeltaTime);
	
	void UpdateVerticalMovement(FVector InputVector, float DeltaTime);
	
	UPROPERTY(Transient)
	ACastlevaniaPawn* CastlevaniaPawn;

	// Climbing stairs maximum velocity, in pixels per second.
	UPROPERTY(EditDefaultsOnly, Category = "Movement | General")
	float ClimbingStairsMaximumVelocity = 30.0f;

	// Climbing stairs maximum velocity, in pixels per second.
	UPROPERTY(EditDefaultsOnly, Category = "Movement | General")
	float FellHardVelocityZ = -201.0f;
	
	// Gravity acceleration, in pixels per second squared.
	UPROPERTY(EditDefaultsOnly, Category = "Movement | General")
	float GravityAcceleration = -480.0f;

	// Velocity to set when hurt, the X is modified based on the hurt direction.
	UPROPERTY(EditDefaultsOnly, Category = "Movement | General")
	FVector HurtInstantaneousVelocity = FVector(75.0f, 0.0f, 125.0f);

	// Velocity to set in Z when jumping.
	UPROPERTY(EditDefaultsOnly, Category = "Movement | General")
	float JumpInstantaneousVelocity = 200.0f;
	
	// Variable to trigger a jump next tick.
	bool bJumpNextTick = false;

	// The amount of time to crouch after being hurt and landing before allowing movement.
	UPROPERTY(EditDefaultsOnly, Category = "Movement | General")
	float RecoveryInitialTime = 0.25f;

	float RecoveryTime = 0.0f;
	
	// Walking maximum velocity, in pixels per second.
	UPROPERTY(EditDefaultsOnly, Category = "Movement | General")
	float WalkingMaximumVelocity = 60.0f;

#pragma region Stairs

public:

	// If the character is inside the area where they can move to ascend stairs.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | Stairs")
	bool bCanMoveToAscendStairs = false;

	// If the character is inside the area where they can move to descend stairs.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | Stairs")
	bool bCanMoveToDescendStairs = false;

	// If the character is currently on stairs.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | Stairs")
	bool bIsOnStairs = false;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | Stairs")
	bool bStairsAscendFromLeftToRight = true;
	
	// The transform of the stairs starting location.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | Stairs")
	FVector StairsRightWorldLocation = FVector::ZeroVector;

	// The transform of the stairs ending location.
	UPROPERTY(VisibleInstanceOnly, Category = "Movement | Stairs")
	FVector StairsLeftWorldLocation = FVector::ZeroVector;
	
#pragma endregion
	
};

