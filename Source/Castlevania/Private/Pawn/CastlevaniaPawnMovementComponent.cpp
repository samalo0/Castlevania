//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pawn/CastlevaniaPawnMovementComponent.h"

#include "CastlevaniaPawn.h"

void UCastlevaniaPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CastlevaniaPawn = Cast<ACastlevaniaPawn>(PawnOwner);
	ensure(IsValid(CastlevaniaPawn));
}

void UCastlevaniaPawnMovementComponent::Hurt(AActor* DamageCauser)
{
	if(!IsValid(PawnOwner))
	{
		return;
	}

	bIsInvincible = true;

	if(bIsOnStairs)
	{
		bIsRecovering = true;
		RecoveryTime = RecoveryInitialTime;
		CastlevaniaPawn->SetInvincibleTimer(CastlevaniaPawn->InitialInvincibilityTime);	
	}
	else
	{
		bIsHurt = true;

		Velocity.Z = HurtInstantaneousVelocity.Z;

		if(IsValid(DamageCauser))
		{
			Velocity.X = DamageCauser->GetActorLocation().X > PawnOwner->GetActorLocation().X ? -1.0f * HurtInstantaneousVelocity.X : HurtInstantaneousVelocity.X;	
		}
		else
		{
			Velocity.X = HurtInstantaneousVelocity.X;
		}	
	}
}

void UCastlevaniaPawnMovementComponent::Jump()
{
	if(bIsOnGround && !bIsCrouched && !bIsOnStairs)
	{
		bJumpNextTick = true;
	}
}

void UCastlevaniaPawnMovementComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
    if(!IsValid(PawnOwner) || !IsValid(UpdatedComponent) || ShouldSkipUpdate(DeltaTime))
    {
    	return;
    }

	// Consume input.
	const FVector InputVector = ConsumeInputVector();	

	// Block all inputs while recovering; after being hurt, force a wait time before you can move.
	if(bIsRecovering)
	{
		RecoveryTime -= DeltaTime;
		if(RecoveryTime <= 0.0f)
		{
			bIsRecovering = false;
		}
		return;
	}

	if(bIsOnStairs)
	{
		UpdateStairsMovement(InputVector, DeltaTime);
		return;
	}
	
	UpdateVerticalMovement(InputVector, DeltaTime);
	
	UpdateHorizontalMovement(InputVector, DeltaTime);
	
	// Prevent crouch and jump while whipping or hurt.
	if(!bIsAttacking && !bIsHurt)
	{
		// Check for jump.
		if(bJumpNextTick)
		{
			Velocity.Z += JumpInstantaneousVelocity;
			bJumpNextTick = false;
			bIsJumping = true;
		}
		else
		{
			// Check for crouch.
			const float VerticalDirection = InputVector.Z;
			if(bIsOnGround)
			{
				if(VerticalDirection == -1.0f && Velocity.IsNearlyZero() && !bIsOnStairs)
				{
					bIsCrouched = true;
				}
				else
				{
					bIsCrouched = false;
				}
			}	
		}
	}
	
    UpdateComponentVelocity();
}

void UCastlevaniaPawnMovementComponent::UpdateHorizontalMovement(const FVector InputVector, const float DeltaTime)
{
	if(!bIsHurt)
	{
		if(bIsCrouched || (bIsAttacking && bIsOnGround))
		{
			Velocity.X = 0.0f;

			if(bIsCrouched && !bIsAttacking)
			{
				CastlevaniaPawn->SetFacingDirection(InputVector.X);
			}
		}
		else if(bIsOnGround)
		{
			if(!bForceMove)
			{
				if((bCanMoveToAscendStairs && InputVector.Z == 1.0f) || (bCanMoveToDescendStairs && InputVector.Z == -1.0f))
				{
					const int32 CurrentX = FMath::RoundToInt(CastlevaniaPawn->GetActorLocation().X);
					int32 DesiredX;
					if(bCanMoveToAscendStairs)
					{
						DesiredX = FMath::RoundToInt(bStairsAscendFromLeftToRight? StairsLeftWorldLocation.X : StairsRightWorldLocation.X);
					}
					else
					{
						DesiredX = FMath::RoundToInt(bStairsAscendFromLeftToRight? StairsRightWorldLocation.X : StairsLeftWorldLocation.X);
					}
					
					if(CurrentX == DesiredX)
					{
						bIsOnStairs = true;
						Velocity.X = 0.0f;

						if(bCanMoveToAscendStairs)
						{
							if(bStairsAscendFromLeftToRight)
							{
								CastlevaniaPawn->SetActorLocation(StairsLeftWorldLocation);							
							}
							else
							{
								CastlevaniaPawn->SetActorLocation(StairsRightWorldLocation);
							}	
						}
						else
						{
							if(bStairsAscendFromLeftToRight)
							{
								CastlevaniaPawn->SetActorLocation(StairsRightWorldLocation);							
							}
							else
							{
								CastlevaniaPawn->SetActorLocation(StairsLeftWorldLocation);
							}
						}
					}
					else if(CurrentX < DesiredX)
					{
						Velocity.X = WalkingMaximumVelocity;
						CastlevaniaPawn->SetFacingDirection(1.0f);
					}
					else
					{
						Velocity.X = WalkingMaximumVelocity * -1.0f;
						CastlevaniaPawn->SetFacingDirection(-1.0f);
					}
				}
				else
				{
					Velocity.X = WalkingMaximumVelocity * InputVector.X;
					CastlevaniaPawn->SetFacingDirection(InputVector.X);
				}
			}
			else
			{
				if(bForceMoveRight)
				{
					Velocity.X = WalkingMaximumVelocity * 1.0f;
					CastlevaniaPawn->SetFacingDirection(1.0f);	
				}
				else
				{
					Velocity.X = WalkingMaximumVelocity * -1.0f;
					CastlevaniaPawn->SetFacingDirection(-1.0f);	
				}
			}
		}		
	}
	
	const FVector DesiredHorizontalMovement = FVector(Velocity.X, 0.0f, 0.0f) * DeltaTime;
	if(!DesiredHorizontalMovement.IsNearlyZero())
	{
		FHitResult HorizontalHit;
		SafeMoveUpdatedComponent(DesiredHorizontalMovement, UpdatedComponent->GetComponentRotation(), true, HorizontalHit);
	}
}

void UCastlevaniaPawnMovementComponent::UpdateStairsMovement(FVector InputVector, const float DeltaTime)
{
	const int32 GridLocation = FMath::RoundToInt(CastlevaniaPawn->GetActorLocation().X) % 8; 
	if(GridLocation == 0 && bIsAttacking)
	{
		return;
	}

	// If no input, but not on the step properly, move to the correct location.
	if(InputVector.X == 0.0f && InputVector.Z == 0.0f)
	{
		if(GridLocation != 0)
		{
			if(CastlevaniaPawn->GetFacingRightDirection())
			{
				InputVector.X = 1.0f;					
			}
			else
			{
				InputVector.X = -1.0f;
			}
		}
		else if(FMath::IsNearlyEqual(CastlevaniaPawn->GetActorLocation().X, StairsLeftWorldLocation.X, 1.0f) ||
				(FMath::IsNearlyEqual(CastlevaniaPawn->GetActorLocation().X, StairsRightWorldLocation.X, 1.0f)))
		{
			// Force movement from the first step or on the lip of the top step.
			if(CastlevaniaPawn->GetFacingRightDirection())
			{
				InputVector.X = 1.0f;					
			}
			else
			{
				InputVector.X = -1.0f;
			}
		}
	}
	
	if(bStairsAscendFromLeftToRight)
	{
		if(InputVector.X == 1.0f || InputVector.Z == 1.0f)
		{
			// Ascend.
			const float Adder = ClimbingStairsMaximumVelocity * DeltaTime;
			const FVector NewLocation = CastlevaniaPawn->GetActorLocation() + FVector(Adder, 0.0f, Adder);
			if(NewLocation.X >= StairsRightWorldLocation.X)
			{
				CastlevaniaPawn->SetActorLocation(StairsRightWorldLocation);
				bIsOnStairs = false;
			}
			else
			{
				CastlevaniaPawn->SetActorLocation(NewLocation);
				CastlevaniaPawn->SetFacingDirection(1.0f);
			}
		}
		else if(InputVector.X == -1.0f || InputVector.Z == -1.0f)
		{
			// Descend.
			const float Adder = ClimbingStairsMaximumVelocity * -DeltaTime;
			const FVector NewLocation = CastlevaniaPawn->GetActorLocation() + FVector(Adder, 0.0f, Adder);
			if(NewLocation.X <= StairsLeftWorldLocation.X)
			{
				CastlevaniaPawn->SetActorLocation(StairsLeftWorldLocation);
				bIsOnStairs = false;
			}
			else
			{
				CastlevaniaPawn->SetActorLocation(NewLocation);
				CastlevaniaPawn->SetFacingDirection(-1.0f);
			}
		}
	}
	else
	{
		if(InputVector.X == 1.0f || InputVector.Z == -1.0f)
		{
			// Descend.
			const float Adder = ClimbingStairsMaximumVelocity * DeltaTime;
			const FVector NewLocation = CastlevaniaPawn->GetActorLocation() + FVector(Adder, 0.0f, -Adder);
			if(NewLocation.X >= StairsRightWorldLocation.X)
			{
				CastlevaniaPawn->SetActorLocation(StairsRightWorldLocation);
				bIsOnStairs = false;
			}
			else
			{
				CastlevaniaPawn->SetActorLocation(NewLocation);
				CastlevaniaPawn->SetFacingDirection(1.0f);
			}
		}
		else if(InputVector.X == -1.0f || InputVector.Z == 1.0f)
		{
			// Ascend.
			const float Adder = ClimbingStairsMaximumVelocity * DeltaTime;
			const FVector NewLocation = CastlevaniaPawn->GetActorLocation() + FVector(-Adder, 0.0f, Adder);
			if(NewLocation.X <= StairsLeftWorldLocation.X)
			{
				CastlevaniaPawn->SetActorLocation(StairsLeftWorldLocation);
				bIsOnStairs = false;
			}
			else
			{
				CastlevaniaPawn->SetActorLocation(NewLocation);
				CastlevaniaPawn->SetFacingDirection(-1.0f);
			}
		}
	}
}

void UCastlevaniaPawnMovementComponent::UpdateVerticalMovement(FVector InputVector, const float DeltaTime)
{
	Velocity.Z += GravityAcceleration * DeltaTime;
	
	const FVector DesiredVerticalMovement = FVector(0.0f, 0.0f, Velocity.Z * DeltaTime);

	FHitResult VerticalHit;
	SafeMoveUpdatedComponent(DesiredVerticalMovement, UpdatedComponent->GetComponentRotation(), true, VerticalHit);
	if(VerticalHit.IsValidBlockingHit())
	{
		// Check for landing.
		if(VerticalHit.Normal.Z == 1.0f)
		{
			bIsOnGround = true;
			bIsJumping = false;

			if(Velocity.Z < FellHardVelocityZ)
			{
				CastlevaniaPawn->PlayFellSound();
				bIsRecovering = true;
				RecoveryTime = RecoveryInitialTime;
				bIsCrouched = true;
			}
			
			// If landing while hurt, stop being hurt and enter recovery, unless dead.
			if(bIsHurt)
			{
				bIsHurt = false;

				if(!bIsDead)
				{
					bIsRecovering = true;
					RecoveryTime = RecoveryInitialTime;
					bIsCrouched = true;

					CastlevaniaPawn->SetInvincibleTimer(CastlevaniaPawn->InitialInvincibilityTime);	
				}

				Velocity = FVector::ZeroVector;
			}
		}

		Velocity.Z = 0.0f;
	}
	else
	{
		// In the air.
		bIsOnGround = false;
	}	
}
