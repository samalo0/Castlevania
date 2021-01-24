//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/19/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/MedusaActor.h"

#include "CastlevaniaCameraActor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

AMedusaActor::AMedusaActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	FlipbookComponent->SetHiddenInGame(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMedusaActor::OnBossFightStart()
{
	HeadSpriteToDestroy->Destroy();
	
	FlipbookComponent->SetHiddenInGame(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SetActorTickEnabled(true);
}

void AMedusaActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch(State)
	{
	case EMedusaState::WaitingForTrigger:
		// todo again
		break;
	}
}

void AMedusaActor::TriggerBattle()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &AMedusaActor::OnBossFightStart, BossFightDelay);
	}
}
