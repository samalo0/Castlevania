//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Spawn/PowerupSpawnActor.h"

#include "CastlevaniaGameInstance.h"
#include "CastlevaniaPawn.h"
#include "ShotPickupActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponActor.h"
#include "Pickup/WhipUpgradeActor.h"

APowerupSpawnActor::APowerupSpawnActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName("Interactable");
	BoxComponent->CanCharacterStepUpOn = ECB_No;
}

void APowerupSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APowerupSpawnActor::OnBoxBeginOverlap);
}

void APowerupSpawnActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	AWeaponActor* Weapon = Cast<AWeaponActor>(OtherActor);
	if(IsValid(Pawn) || IsValid(Weapon))
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UGameplayStatics::PlaySound2D(this, HitSound);

		if(IsValid(Weapon))
		{
			Weapon->Hit();
		}
	}
}

void APowerupSpawnActor::SpawnPowerup()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
	if(!IsValid(GameInstance))
	{
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.ObjectFlags = RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if(bSpawnWhipUpgradeIfNotFullyUpgraded)
	{
		if(GameInstance->GetWhipType() != EWhipType::Long && !IsValid(UGameplayStatics::GetActorOfClass(this, AWhipUpgradeActor::StaticClass())))
		{
			World->SpawnActor<AWhipUpgradeActor>(WhipUpgradeClass, GetActorLocation() + SpawnOffset, GetActorRotation(), SpawnParameters);
			return;
		}
	}
	
	if(bSpawnShotIfNotFullyUpgraded)
	{
		if (GameInstance->GetWeaponType() != EWeaponType::None && GameInstance->GetWeaponType() != EWeaponType::Clock &&
			GameInstance->GetShot() < 3 && !IsValid(UGameplayStatics::GetActorOfClass(this, AShotPickupActor::StaticClass())))
		{
			FTransform Transform = GetActorTransform();
			Transform.SetLocation(Transform.GetLocation() + SpawnOffset);
			AShotPickupActor* Shot = World->SpawnActorDeferred<AShotPickupActor>(ShotPickupClass,  Transform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if(IsValid(Shot))
			{
				Shot->SetIsTripleShot(GameInstance->GetShot() == 2);
				UGameplayStatics::FinishSpawningActor(Shot, Transform);
				return;
			}	
		}
	}
	
	World->SpawnActor<AActor>(ClassToSpawnOnDestroy, GetActorLocation() + SpawnOffset, GetActorRotation(), SpawnParameters);
}



