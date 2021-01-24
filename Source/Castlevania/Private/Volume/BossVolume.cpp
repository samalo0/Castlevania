//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/4/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Volume/BossVolume.h"

#include "Enemy/BossEnemyActor.h"
#include "Components/BrushComponent.h"
#include "CastlevaniaCameraActor.h"
#include "CastlevaniaGameModeBase.h"

ABossVolume::ABossVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetBrushComponent()->SetCollisionProfileName("Interactable");
}

void ABossVolume::BeginPlay()
{
	Super::BeginPlay();

	GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABossVolume::OnBeginOverlap);
}

void ABossVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		Camera->SetMinimumAndMaximumX(CameraMinimumX, CameraMaximumX);

		for(AVolume* Volume : BlockingVolumesToEnable)
		{
			Volume->GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

		if(IsValid(BossToEnable))
		{
			BossToEnable->TriggerBattle();
		}

		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
			if(IsValid(GameMode))
			{
				GameMode->PlayBossMusic();
			}
		}
		
		Destroy();
	}
}
