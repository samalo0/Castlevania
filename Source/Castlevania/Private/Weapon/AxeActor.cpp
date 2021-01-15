//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Weapon/AxeActor.h"

#include "CastlevaniaCameraActor.h"
#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AAxeActor::AAxeActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName("Weapon");
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetGenerateOverlapEvents(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetRootComponent());
	AudioComponent->SetAutoActivate(false);

	WeaponDamage = 2;
}

void AAxeActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AAxeActor::OnEndOverlap);

	Velocity = InitialVelocity;
	
	const FVector Scale = GetActorScale();
	if(Scale.X < 0)
	{
		Velocity.X *= -1.0f;
	}

	SetActorTickEnabled(true);

	AudioComponent->Activate();
}

void AAxeActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		AudioComponent->Deactivate();
		Destroy();
	}
}

void AAxeActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Velocity.Z += GravityAcceleration * DeltaSeconds;

	const FVector NewLocation = GetActorLocation() + Velocity * DeltaSeconds;
	SetActorLocation(NewLocation);

	// Round flipbook location to the nearest pixel.
	FVector RoundLocation = NewLocation;
	RoundLocation.X = static_cast<float>(FMath::RoundToInt(RoundLocation.X));
	RoundLocation.Z = static_cast<float>(FMath::RoundToInt(RoundLocation.Z));
	FlipbookComponent->SetWorldLocation(RoundLocation);
}

