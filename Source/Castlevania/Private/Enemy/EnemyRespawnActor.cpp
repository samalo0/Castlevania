//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/30/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/EnemyRespawnActor.h"

#include "Components/BoxComponent.h"
#include "Core/CastlevaniaCameraActor.h"
#include "CastlevaniaGameModeBase.h"
#include "Enemy/EnemyActor.h"
#include "PaperFlipbookComponent.h"

AEnemyRespawnActor::AEnemyRespawnActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName(TEXT("Interactable")));

#if WITH_EDITORONLY_DATA
	
	FlipbookComponent = CreateEditorOnlyDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	if(FlipbookComponent != nullptr)
	{
		FlipbookComponent->SetupAttachment(GetRootComponent());
		FlipbookComponent->SetCollisionProfileName("NoCollision");
		FlipbookComponent->SetHiddenInGame(true);
		FlipbookComponent->SetGenerateOverlapEvents(false);
	}

#endif	
	
}

void AEnemyRespawnActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyRespawnActor::OnBoxBeginOverlap);

	// Check for camera overlap on begin play.
	TArray<AActor*> OverlappingActors;
	BoxComponent->GetOverlappingActors(OverlappingActors, ACastlevaniaCameraActor::StaticClass());
	if(OverlappingActors.Num() > 0)
	{
		for(AActor* Actor : OverlappingActors)
		{
			ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(Actor);
			if(IsValid(Camera) && !IsValid(EnemyReference))
			{
				SpawnEnemy();
			}
		}
	}
	
#if WITH_EDITORONLY_DATA
	if(FlipbookComponent != nullptr)
	{
		FlipbookComponent->SetComponentTickEnabled(false);	
	}
#endif

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(GameMode))
		{
			GameMode->OnClockTimeStop.AddDynamic(this, &AEnemyRespawnActor::TimeStop);
		}
	}
}

void AEnemyRespawnActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Spawn if the reference is bad and the camera overlaps, as long as time isn't stopped.
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera) && !IsValid(EnemyReference) && !bTimeIsStopped)
	{
		SpawnEnemy();
	}
}

void AEnemyRespawnActor::SpawnEnemy()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;	
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.ObjectFlags = RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
	EnemyReference = World->SpawnActor<AEnemyActor>(EnemyActorClass, GetActorTransform(), SpawnParameters);
}

void AEnemyRespawnActor::TimeStop(const bool bIsActive)
{
	bTimeIsStopped = bIsActive;
}
