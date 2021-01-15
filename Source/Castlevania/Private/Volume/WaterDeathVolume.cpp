//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/1/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Volume/WaterDeathVolume.h"

#include "CastlevaniaPawn.h"
#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AWaterDeathVolume::AWaterDeathVolume()
{
	BoxComponent->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
}

void AWaterDeathVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	AEnemyActor* Enemy = Cast<AEnemyActor>(OtherActor);
	if(IsValid(Pawn) || IsValid(Enemy))
	{
		UGameplayStatics::PlaySound2D(this, SplashSound);
	
		UWorld* World = GetWorld();
		if(IsValid(World) && IsValid(SplashEffectClass))
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			SpawnParameters.ObjectFlags = RF_Transient;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			for(int32 Index = 0; Index < 3; Index++)
			{
				World->SpawnActor<AActor>(SplashEffectClass, OtherActor->GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
			}
		}		
	}

	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
