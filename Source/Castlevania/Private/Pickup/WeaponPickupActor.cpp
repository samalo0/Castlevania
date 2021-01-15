//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/WeaponPickupActor.h"

#include "Pawn/CastlevaniaPawn.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AWeaponPickupActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
				GameInstance->SetWeapon(WeaponType);
				Pawn->SetWeapon(WeaponType);

				UGameplayStatics::PlaySound2D(this, PickupSound);
				Destroy();
			}
		}
	}	
}
