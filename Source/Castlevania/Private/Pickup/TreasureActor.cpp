//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/TreasureActor.h"

#include "Pawn/CastlevaniaPawn.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ATreasureActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
				GameInstance->AddScore(Score);

				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = this;
				SpawnParameters.ObjectFlags = RF_Transient;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				
				ABonusPointsEffectActor* Actor = World->SpawnActor<ABonusPointsEffectActor>(BonusPointsEffectClass, GetActorLocation() + LocationOffset, FRotator::ZeroRotator, SpawnParameters);
				if(IsValid(Actor))
				{
					Actor->SetValue(Score);
				}
				
				UGameplayStatics::PlaySound2D(this, PickupSound);
				Destroy();
			}
		}
	}	
}
