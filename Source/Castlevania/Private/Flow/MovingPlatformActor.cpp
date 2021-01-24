//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/16/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Flow/MovingPlatformActor.h"

#include "Components/BoxComponent.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Pawn/CastlevaniaPawn.h"
#include "PaperSpriteComponent.h"

AMovingPlatformActor::AMovingPlatformActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
	RootSceneComponent->SetMobility(EComponentMobility::Static);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetMobility(EComponentMobility::Movable);
	BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
			
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(BoxComponent);
	SpriteComponent->SetCollisionProfileName("BlockAll");
	SpriteComponent->SetGenerateOverlapEvents(false);
	SpriteComponent->SetMobility(EComponentMobility::Movable);
}

void AMovingPlatformActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	InitialWorldLocation = GetActorLocation();
	
	SecondaryWorldLocation = (SecondaryRelativeTransform * GetActorTransform()).GetLocation();
	
	bSecondaryIsOnRight = InitialWorldLocation.X < SecondaryWorldLocation.X;

	bHeadingRight = bSecondaryIsOnRight;
	
	Acceleration = MaximumVelocity / TurnAroundTime;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatformActor::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatformActor::OnBoxEndOverlap);

	UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
	if(IsValid(GameInstance))
	{
		if(GameInstance->GetStage() == ActiveStage)
		{
			SetActorTickEnabled(true);	
		}

		GameInstance->OnStageChanged.AddUObject(this, &AMovingPlatformActor::OnStageChanged);
	}
}

void AMovingPlatformActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		Pawn->AttachToComponent(BoxComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMovingPlatformActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		Pawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AMovingPlatformActor::OnStageChanged(int32& Stage)
{
	SetActorTickEnabled(ActiveStage == Stage);
}

void AMovingPlatformActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector CurrentLocation = BoxComponent->GetComponentLocation();

	FVector DesiredLocation;
	FVector StartingLocation;
	if(bSecondaryIsOnRight)
	{
		DesiredLocation = bHeadingRight? SecondaryWorldLocation : InitialWorldLocation;
		StartingLocation = bHeadingRight? InitialWorldLocation : SecondaryWorldLocation;
	}
	else
	{
		DesiredLocation = bHeadingRight? InitialWorldLocation : SecondaryWorldLocation;
		StartingLocation = bHeadingRight? SecondaryWorldLocation : InitialWorldLocation;
	}

	const bool bStarting = FVector::Dist(CurrentLocation, StartingLocation) <= TurnAroundDistance;
	const bool bTurningAround = FVector::Dist(CurrentLocation, DesiredLocation) <= TurnAroundDistance;

	if((bHeadingRight && bTurningAround) ||
		(!bHeadingRight && bStarting))
	{
		Velocity -= Acceleration * DeltaSeconds;
		if(Velocity < -1.0f * MaximumVelocity)
		{
			Velocity = -1.0f * MaximumVelocity;
		}
	}
	else if((!bHeadingRight && bTurningAround) ||
		(bHeadingRight && bStarting))
	{
		Velocity += Acceleration * DeltaSeconds;
		if(Velocity > MaximumVelocity)
		{
			Velocity = MaximumVelocity;
		}
	}	

	const FVector NewLocation = FVector(CurrentLocation.X + Velocity * DeltaSeconds, CurrentLocation.Y, CurrentLocation.Z);
	BoxComponent->SetWorldLocation(NewLocation);

	FVector RoundLocation = SpriteComponent->GetComponentLocation();
	RoundLocation.X = FMath::RoundToInt(NewLocation.X);
	SpriteComponent->SetWorldLocation(RoundLocation);

	if((bHeadingRight && NewLocation.X >= DesiredLocation.X) ||
		(!bHeadingRight && NewLocation.X <= DesiredLocation.X))
	{
		bHeadingRight = !bHeadingRight;
	}
}


