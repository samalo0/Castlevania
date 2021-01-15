//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/EnemySpawnActor.h"

#include "Components/BoxComponent.h"
#include "Core/CastlevaniaCameraActor.h"
#include "CastlevaniaGameModeBase.h"
#include "Enemy/EnemyActor.h"
#include "PaperFlipbookComponent.h"

AEnemySpawnActor::AEnemySpawnActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ActiveSpawnBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(ActiveSpawnBoxComponent);
	ActiveSpawnBoxComponent->SetCollisionProfileName("Enemy");

#if WITH_EDITORONLY_DATA

	FlipbookComponent = CreateEditorOnlyDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	if(FlipbookComponent != nullptr)
	{
		FlipbookComponent->SetupAttachment(GetRootComponent());
		FlipbookComponent->SetCollisionProfileName("NoCollision");
		FlipbookComponent->SetHiddenInGame(true);	
	}
	
#endif
	
}

void AEnemySpawnActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
	if(IsValid(GameMode))
	{
		GameMode->OnClockTimeStop.AddDynamic(this, &AEnemySpawnActor::TimeStop);
	}
	
	ActiveSpawnBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnActor::OnBoxBeginOverlap);

	ActiveSpawnBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemySpawnActor::OnBoxEndOverlap);

	// Check for overlap on begin play.
	TArray<AActor*> OverlappingActors;
	ActiveSpawnBoxComponent->GetOverlappingActors(OverlappingActors, ACastlevaniaCameraActor::StaticClass());
	if(OverlappingActors.Num() > 0)
	{
		bIsSpawning = true;
		World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnActor::OnSpawnDurationElapsed, FMath::FRandRange(MinimumSpawnTime, MaximumSpawnTime));
	}

#if WITH_EDITORONLY_DATA
	if(FlipbookComponent != nullptr)
	{
		FlipbookComponent->SetComponentTickEnabled(false);	
	}
#endif
}

void AEnemySpawnActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			bIsSpawning = true;
			if(!bIsTimeStopped)
			{
				World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnActor::OnSpawnDurationElapsed, FMath::FRandRange(MinimumSpawnTime, MaximumSpawnTime));				
			}
		}
	}
}

void AEnemySpawnActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			bIsSpawning = false;
			World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		}
	}
}

void AEnemySpawnActor::TimeStop(const bool bEnable)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	bIsTimeStopped = bEnable;
	
	if(bEnable)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
	else if(bIsSpawning)
	{
		World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnActor::OnSpawnDurationElapsed, FMath::FRandRange(MinimumSpawnTime, MaximumSpawnTime));
	}
}

#pragma region Spawn

void AEnemySpawnActor::OnSpawnDurationElapsed()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
		
	// Clean up destroyed actors.
	for(int32 Index = SpawnedActors.Num() - 1; Index >= 0; Index--)
	{
		if(!IsValid(SpawnedActors[Index]))
		{
			SpawnedActors.RemoveAt(Index, 1);
		}
	}

	// Set for next time.
	World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnActor::OnSpawnDurationElapsed, FMath::FRandRange(MinimumSpawnTime, MaximumSpawnTime));
	
	// If too many exist, return.
	if(SpawnedActors.Num() >= MaximumInstances)
	{
		return;
	}

	const int32 RandomLocation = FMath::RandRange(0, SpawnRelativeTransforms.Num() - 1);	
	SpawnEnemy(SpawnRelativeTransforms[RandomLocation]);
}

void AEnemySpawnActor::SpawnEnemy(const FTransform RelativeTransform)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	// Spawn a new actor.
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.ObjectFlags = RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEnemyActor* Actor = World->SpawnActor<AEnemyActor>(EnemyClassToSpawn, RelativeTransform * GetActorTransform(), SpawnParameters);
	if(IsValid(Actor))
	{
		SpawnedActors.Add(Actor);
	}
}

#pragma endregion


