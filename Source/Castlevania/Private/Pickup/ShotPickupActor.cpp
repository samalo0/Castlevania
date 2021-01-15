//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/2/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/ShotPickupActor.h"

#include "CastlevaniaGameInstance.h"
#include "CastlevaniaPawn.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"

void AShotPickupActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
				UGameplayStatics::PlaySound2D(this, PickupSound);

				if(GameInstance->GetWeaponType() != EWeaponType::Clock && GameInstance->GetWeaponType() != EWeaponType::None)
				{
					Pawn->AddMaximumWeaponInstance();
					GameInstance->SetShot(GameInstance->GetShot() + 1);
				}
				Destroy();
			}
		}
	}
}

void AShotPickupActor::SetIsTripleShot(const bool bIs)
{
	if(bIs)
	{
		bIsTripleShot = true;
		SpriteComponent->SetSprite(TripleShotSprite);
	}
}
