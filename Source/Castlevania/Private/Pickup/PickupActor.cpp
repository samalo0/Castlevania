//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/PickupActor.h"

#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SetRootComponent(SpriteComponent);
	SpriteComponent->SetCollisionProfileName("Pickup");

	InitialLifeSpan = 4.0f;
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	SpriteComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnBeginOverlap);
	
	// Line trace to the floor, to find where the pickup will fall.
	FHitResult OutHit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation() + FVector(0.0f, 0.0f, -240.0f);
	if(World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams))
	{
		DesiredZ = static_cast<float>(FMath::RoundToInt(OutHit.Location.Z + SpriteComponent->GetCollisionShape().GetExtent().Z));
		SetActorTickEnabled(true);
	}
}

void APickupActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bOnGround)
	{
		return;
	}
	const FVector CurrentLocation = GetActorLocation();
	
	VelocityZ += GravityAcceleration * DeltaSeconds;
	float Z = CurrentLocation.Z + VelocityZ * DeltaSeconds;
	if(Z <= DesiredZ)
	{
		bOnGround = true;
		Z = DesiredZ;
		SetActorTickEnabled(false);
	}
		
	SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, Z));
}

