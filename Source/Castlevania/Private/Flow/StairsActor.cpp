//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/27/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Flow/StairsActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaPawn.h"
#include "PaperSpriteComponent.h"

AStairsActor::AStairsActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
	RootSceneComponent->SetMobility(EComponentMobility::Static);

	DownBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DownBoxComponent"));
	DownBoxComponent->SetupAttachment(GetRootComponent());
	DownBoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
	DownBoxComponent->SetBoxExtent(FVector(8.0f, 25.0f, 16.0f));
	DownBoxComponent->SetMobility(EComponentMobility::Static);

	UpBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("UpBoxComponent"));
	UpBoxComponent->SetupAttachment(GetRootComponent());
	UpBoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
	UpBoxComponent->SetBoxExtent(FVector(16.0f, 25.0f, 16.0f));
	UpBoxComponent->SetMobility(EComponentMobility::Static);

	DownOffsetComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DownOffsetComponent"));
	DownOffsetComponent->SetupAttachment(DownBoxComponent);
	DownOffsetComponent->SetMobility(EComponentMobility::Static);
	
	DownSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DownSpriteComponent"));
	DownSpriteComponent->SetupAttachment(DownOffsetComponent);
	DownSpriteComponent->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));
	DownSpriteComponent->SetRelativeLocation(FVector(-9.0f, 0.2f, 0.0f));
	DownSpriteComponent->SetCollisionProfileName("NoCollision");
	DownSpriteComponent->SetHiddenInGame(true);
	DownSpriteComponent->SetGenerateOverlapEvents(false);
	DownSpriteComponent->SetMobility(EComponentMobility::Static);

	UpOffsetComponent = CreateDefaultSubobject<USceneComponent>(TEXT("UpOffsetComponent"));
	UpOffsetComponent->SetupAttachment(UpBoxComponent);
	UpOffsetComponent->SetMobility(EComponentMobility::Static);
	
	UpSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("UpSpriteComponent"));
	UpSpriteComponent->SetupAttachment(UpOffsetComponent);
	UpSpriteComponent->SetRelativeLocation(FVector(0.0f, 0.2f, 0.0f));
	UpSpriteComponent->SetCollisionProfileName("NoCollision");
	UpSpriteComponent->SetHiddenInGame(true);
	UpSpriteComponent->SetGenerateOverlapEvents(false);
	UpSpriteComponent->SetMobility(EComponentMobility::Static);
}

void AStairsActor::BeginPlay()
{
	Super::BeginPlay();

	DownBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStairsActor::OnDownBoxBeginOverlap);

	DownBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AStairsActor::OnDownBoxEndOverlap);
	
	UpBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStairsActor::OnUpBoxBeginOverlap);

	UpBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AStairsActor::OnUpBoxEndOverlap);
}

void AStairsActor::OnDownBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		if(bAscendsFromLeftToRight)
		{
			Pawn->SetCanMoveToStairs(true, true, bAscendsFromLeftToRight, UpOffsetComponent->GetComponentLocation(), DownOffsetComponent->GetComponentLocation());	
		}
		else
		{
			Pawn->SetCanMoveToStairs(true, true, bAscendsFromLeftToRight, DownOffsetComponent->GetComponentLocation(), UpOffsetComponent->GetComponentLocation());
		}
	}
}

void AStairsActor::OnDownBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		Pawn->SetCanMoveToStairs(false);
	}
}

void AStairsActor::OnUpBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		if(bAscendsFromLeftToRight)
		{
			Pawn->SetCanMoveToStairs(true, false, bAscendsFromLeftToRight, UpOffsetComponent->GetComponentLocation(), DownOffsetComponent->GetComponentLocation());	
		}
		else
		{
			Pawn->SetCanMoveToStairs(true, false, bAscendsFromLeftToRight, DownOffsetComponent->GetComponentLocation(), UpOffsetComponent->GetComponentLocation());			
		}
	}
}

void AStairsActor::OnUpBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		Pawn->SetCanMoveToStairs(false);
	}
}
