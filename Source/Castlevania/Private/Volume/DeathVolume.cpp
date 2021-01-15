//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/30/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Volume/DeathVolume.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"

ADeathVolume::ADeathVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetMobility(EComponentMobility::Static);
	BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
	BoxComponent->SetBoxExtent(FVector(256.0f, 25.0f, 8.0f));
}

void ADeathVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeathVolume::OnBeginOverlap);
}

void ADeathVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	UWorld* World = GetWorld();
	if(IsValid(Pawn) && IsValid(World))
	{
		Pawn->InstantDeath();
		
		ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(GameMode))
		{
			GameMode->PlayDeathMusic();

			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, this, &ADeathVolume::OnBeginOverlapAfterDelay, 2.0f, false);
		}
	}
}

void ADeathVolume::OnBeginOverlapAfterDelay()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(GameMode))
		{
			GameMode->StartLevelTransition(false);
		}
	}
}
