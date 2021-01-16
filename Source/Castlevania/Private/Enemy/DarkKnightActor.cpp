//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/16/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/DarkKnightActor.h"

#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

ADarkKnightActor::ADarkKnightActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	Life = 4;
}

void ADarkKnightActor::HitWithWeapon(const int32 Damage, const bool bPlaySound)
{
	Super::HitWithWeapon(Damage, bPlaySound);

	if(Life <= 0)
	{
		SetActorTickEnabled(false);
	}
}

void ADarkKnightActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Check for walking into space.
	Accumulator += DeltaSeconds;
	if(Accumulator >= LineTraceInterval)
	{
		Accumulator = 0.0f;

		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			const FVector Start = GetActorLocation() + FVector(BoxComponent->GetScaledBoxExtent().X * FlipbookComponent->GetComponentScale().X, 0.0f, -BoxComponent->GetScaledBoxExtent().Z);
			const FVector End = Start + FVector(0.0f, 0.0f, -8.0f);

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

			FHitResult OutHit;
			if(!World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams))
			{
				FlipbookComponent->SetWorldScale3D(FVector(FlipbookComponent->GetComponentScale().X * -1.0f, 1.0f, 1.0f));
				return;
			}
		}

		if(IsValid(World))
		{
			const FVector Start = GetActorLocation() + FVector(BoxComponent->GetScaledBoxExtent().X * FlipbookComponent->GetComponentScale().X, 0.0f, 0.0f);
			const FVector End = Start + FVector(8.0f * FlipbookComponent->GetComponentScale().X, 0.0f, 0.0f);

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

			FHitResult OutHit;
			if(World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams))
			{
				FlipbookComponent->SetWorldScale3D(FVector(FlipbookComponent->GetComponentScale().X * -1.0f, 1.0f, 1.0f));
				return;
			}
		}

		float TurnAround = FMath::FRandRange(0.0f, 1.0f);
		if(TurnAround <= ProbabilityOfTurningAround)
		{
			FlipbookComponent->SetWorldScale3D(FVector(FlipbookComponent->GetComponentScale().X * -1.0f, 1.0f, 1.0f));
			return;
		}
	}

	// Set new location, if the dark knight did not turn around.
	FVector Location = GetActorLocation();
	Location.X += MovementSpeed * FlipbookComponent->GetComponentScale().X * DeltaSeconds;
	SetActorLocation(Location);
}

