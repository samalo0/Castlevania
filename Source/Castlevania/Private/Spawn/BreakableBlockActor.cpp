//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Spawn/BreakableBlockActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaPawn.h"
#include "PaperSpriteComponent.h"
#include "SplashEffectActor.h"
#include "Weapon/WeaponActor.h"

ABreakableBlockActor::ABreakableBlockActor()
{
	BoxComponent->SetCollisionProfileName("BreakableBlock");
	
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(GetRootComponent());
	SpriteComponent->SetCollisionProfileName("NoCollision");
	SpriteComponent->SetGenerateOverlapEvents(false);
}

void ABreakableBlockActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	AWeaponActor* Weapon = Cast<AWeaponActor>(OtherActor);
	if(IsValid(Pawn) || IsValid(Weapon))
	{
		UWorld* World = GetWorld();
		if(!IsValid(World))
		{
			return;
		}

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.ObjectFlags = RF_Transient;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		for(int32 Index = 0; Index < 4; Index++)
		{
			World->SpawnActor<ASplashEffectActor>(BreakEffectClass, GetActorTransform(), SpawnParameters);
		}
	
		SpawnPowerup();

		Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

		Destroy();
	}
}
