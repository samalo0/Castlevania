//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Spawn/LightPowerupSpawnActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaCameraActor.h"
#include "PaperFlipbookComponent.h"
#include "Components/PointLightComponent.h"
#include "WeaponActor.h"

ALightPowerupSpawnActor::ALightPowerupSpawnActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 1.0f/8.0f;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetComponentTickEnabled(false);
	
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(GetRootComponent());
	PointLightComponent->SetCastShadows(false);
	PointLightComponent->SetIntensityUnits(ELightUnits::Candelas);
	PointLightComponent->SetIntensity(1.0f);
	PointLightComponent->SetLightColor(FLinearColor(1.0f, 0.5f, 0.0f));
	PointLightComponent->SetAttenuationRadius(75.0f);
}

void ALightPowerupSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALightPowerupSpawnActor::OnBoxEndOverlap);
	
	// Check for camera overlap on begin play.
	TArray<AActor*> OverlappingActors;
	BoxComponent->GetOverlappingActors(OverlappingActors, ACastlevaniaCameraActor::StaticClass());
	if(OverlappingActors.Num() > 0)
	{
		SetActorTickEnabled(true);
		FlipbookComponent->SetComponentTickEnabled(true);
		PointLightComponent->SetVisibility(true);
	}
	else
	{
		FlipbookComponent->SetComponentTickEnabled(false);
		PointLightComponent->SetVisibility(false);
	}
}

void ALightPowerupSpawnActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	AWeaponActor* Weapon = Cast<AWeaponActor>(OtherActor);
	if(IsValid(Pawn) || IsValid(Weapon))
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		FlipbookComponent->SetLooping(false);
		FlipbookComponent->SetFlipbook(BurnOutFlipBook);
		FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ALightPowerupSpawnActor::OnFinishedPlaying);
		FlipbookComponent->PlayFromStart();
	}

	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		FlipbookComponent->SetComponentTickEnabled(true);
		SetActorTickEnabled(true);
		PointLightComponent->SetVisibility(true);
	}

	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ALightPowerupSpawnActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		FlipbookComponent->SetComponentTickEnabled(false);
		SetActorTickEnabled(false);
		PointLightComponent->SetVisibility(false);
	}
}

void ALightPowerupSpawnActor::OnFinishedPlaying()
{
	SpawnPowerup();
	
	Destroy();
}

void ALightPowerupSpawnActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PointLightComponent->SetIntensity(FMath::RandRange(MinimumLight, MaximumLight));
}
