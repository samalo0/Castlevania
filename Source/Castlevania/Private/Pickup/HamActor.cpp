//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/HamActor.h"

#include "CastlevaniaGameInstance.h"
#include "CastlevaniaPawn.h"
#include "Kismet/GameplayStatics.h"

void AHamActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
		
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
			if(IsValid(GameInstance))
			{
				GameInstance->AddPlayerHealth(Health);
			}
		}

		Destroy();
	}
}
