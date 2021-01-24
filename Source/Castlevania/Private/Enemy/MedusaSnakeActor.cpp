//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/23/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/MedusaSnakeActor.h"

#include "CastlevaniaFunctionLibrary.h"
#include "Components/BoxComponent.h"

AMedusaSnakeActor::AMedusaSnakeActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMedusaSnakeActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
{
	SetActorTickEnabled(false);
	
	Super::HitWithWeapon(Damage, bPlaySound, WeaponLocation);
}

void AMedusaSnakeActor::BeginPlay()
{
	Super::BeginPlay();

	FVector HitLocation;
	if(RaycastToGround(HitLocation, -48.0f))
	{	
		LocationFloat = HitLocation + FVector(0.0f, 0.0f, BoxComponent->GetScaledBoxExtent().Z);
		const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
		
		SetActorLocation(LocationInteger); 
		MovementSpeed *= GetActorScale3D().X;
	}
}

bool AMedusaSnakeActor::RaycastToGround(FVector& HitLocation, const float Distance) const
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return false;
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	FHitResult OutHit;
	const FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, -(BoxComponent->GetScaledBoxExtent().Z + 1.0f));
	const FVector End = Start + FVector(0.0f, 0.0f, Distance);
	if(World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams))
	{
		HitLocation = OutHit.Location;
		return true;
	}

	return false;
}

void AMedusaSnakeActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LocationFloat.X += MovementSpeed * DeltaSeconds;

	if(bFalling)
	{
		VelocityZ += GravityAcceleration * DeltaSeconds;
		LocationFloat.Z += VelocityZ * DeltaSeconds; 
	}

	FallRaycastAccumulator += DeltaSeconds;
	
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);

		if(!bFalling && FallRaycastAccumulator >= FallRaycastInterval)
		{
			FallRaycastAccumulator = 0.0f;
			FVector HitLocation;
			if(!RaycastToGround(HitLocation, -8.0f))
			{
				bFalling = true;				
			}
		}
	}
}

