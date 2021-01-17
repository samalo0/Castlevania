//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pickup/CrossActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaCameraActor.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "Engine/DirectionalLight.h"
#include "EnemyActor.h"
#include "PaperSpriteComponent.h"
#include "Components/LightComponent.h"
#include "Kismet/GameplayStatics.h"

void ACrossActor::Flash()
{
	CurrentNumberOfFlashes++;
	if(CurrentNumberOfFlashes >= NumberOfFlashes)
	{
		DirectionalLight->GetLightComponent()->SetIntensity(InitialIntensity);
		Destroy();	
	}
	else if(CurrentNumberOfFlashes % 2)
	{
		DirectionalLight->GetLightComponent()->SetIntensity(InitialIntensity);
	}
	else
	{
		DirectionalLight->GetLightComponent()->SetIntensity(InitialIntensity * IntensityMultiplier);
	}
}

void ACrossActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);

		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
			if(IsValid(GameMode))
			{
				ACastlevaniaCameraActor* Camera = GameMode->GetCamera();
				if(IsValid(Camera))
				{
					TArray<AActor*> OverlappingActors;
					Camera->GetBoxComponent()->GetOverlappingActors(OverlappingActors, AEnemyActor::StaticClass());
					for(AActor* Actor : OverlappingActors)
					{
						AEnemyActor* Enemy = Cast<AEnemyActor>(Actor);
						if(IsValid(Enemy))
						{
							Enemy->HitWithWeapon(10, false, Enemy->GetActorLocation());
						}
					}
				}
			}

			DirectionalLight = GameMode->GetDirectionalLight();
			if(IsValid(DirectionalLight))
			{
				FTimerHandle TimerHandle;
				World->GetTimerManager().SetTimer(TimerHandle, this, &ACrossActor::Flash, FlashTime / NumberOfFlashes, true);

				InitialIntensity = DirectionalLight->GetLightComponent()->Intensity; 
				DirectionalLight->GetLightComponent()->SetIntensity(InitialIntensity * IntensityMultiplier);

				SpriteComponent->SetVisibility(false);
				SpriteComponent->SetGenerateOverlapEvents(false);
			}
		}
	}
}
