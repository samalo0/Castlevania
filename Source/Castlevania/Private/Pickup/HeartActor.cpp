//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/HeartActor.h"

#include "CastlevaniaFunctionLibrary.h"
#include "CastlevaniaGameInstance.h"
#include "Pawn/CastlevaniaPawn.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"

AHeartActor::AHeartActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SetRootComponent(SpriteComponent);
	SpriteComponent->SetCollisionProfileName("Pickup");

	InitialLifeSpan = 4.0f;
}

void AHeartActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	SpriteComponent->OnComponentBeginOverlap.AddDynamic(this, &AHeartActor::OnBeginOverlap);
	
	// Line trace to the floor, to find where the pickup will fall.
	FHitResult OutHit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation() + FVector(0.0f, 0.0f, -240.0f);
	if(World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams))
	{
		DesiredLocation = GetActorLocation();
		DesiredLocation.Z = OutHit.Location.Z + SpriteComponent->GetCollisionShape().GetExtent().Z;
		DesiredLocation = UCastlevaniaFunctionLibrary::RoundVectorToInt(DesiredLocation);
		SetActorTickEnabled(true);
	}

	LocationFloat = GetActorLocation();
}

void AHeartActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
			if(IsValid(GameInstance))
			{
				GameInstance->AddHearts(Hearts);
				UGameplayStatics::PlaySound2D(this, PickupSound);
				Destroy();
			}
		}
	}
}

void AHeartActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Accumulator += DeltaSeconds;
	const float XAdder = SineAmplitude * FMath::Sin(2 * PI * SineFrequency * Accumulator);

	LocationFloat.X = DesiredLocation.X + XAdder;
	LocationFloat.Z = FMath::FInterpConstantTo(LocationFloat.Z, DesiredLocation.Z, DeltaSeconds, InterpolationSpeed);

	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);
		if(FMath::IsNearlyEqual(LocationFloat.Z, DesiredLocation.Z, 0.99f))
		{
			SetActorTickEnabled(false);
		}	
	}
}
