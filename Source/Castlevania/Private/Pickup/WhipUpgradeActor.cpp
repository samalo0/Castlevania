//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/WhipUpgradeActor.h"

#include "Pawn/CastlevaniaPawn.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AWhipUpgradeActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
			if(IsValid(GameInstance))
			{
				Pawn->UpgradeWhip();
				GameInstance->SetWhipType(Pawn->GetWhipType());
				
				UGameplayStatics::PlaySound2D(this, PickupSound);
				Destroy();
			}
		}
	}
}
