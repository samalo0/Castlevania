//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/PickupActor.h"


#include "CastlevaniaFunctionLibrary.h"
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

	LocationFloat = GetActorLocation();
}

void APickupActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bOnGround)
	{
		return;
	}
		
	VelocityZ += GravityAcceleration * DeltaSeconds;

	LocationFloat.Z += VelocityZ * DeltaSeconds;
	if(LocationFloat.Z <= DesiredZ)
	{
		bOnGround = true;
		LocationFloat.Z = DesiredZ;
		SetActorTickEnabled(false);
	}
		
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);	
	}
}

