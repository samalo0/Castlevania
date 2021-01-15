//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/PotionActor.h"

#include "CastlevaniaPawn.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"

void APotionActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		UWorld* World = GetWorld();
		if(!IsValid(World))
		{
			return;
		}

		SetLifeSpan(0.0f);
		
		UGameplayStatics::PlaySound2D(this, PickupSound);

		// Set player invincibility auto-timeout.
		Pawn->MovementComponent->bIsInvincible = true;
		Pawn->SetInvincibleTimer(PotionDuration);

		SpriteComponent->SetVisibility(false);
		SpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		FTimerHandle TimerHandle; 
		World->GetTimerManager().SetTimer(TimerHandle, this, &APotionActor::OnPotionExpired, PotionDuration, false);
	}
}

void APotionActor::OnPotionExpired()
{
	UGameplayStatics::PlaySound2D(this, PotionExpiredSound);
	
	Destroy();
}
