//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/EnemyActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaCameraActor.h"
#include "CastlevaniaGameInstance.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "ShotPickupActor.h"
#include "WeaponActor.h"
#include "WhipUpgradeActor.h"

AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName("Enemy");

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetGenerateOverlapEvents(false);
}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyActor::OnBoxEndOverlap);

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(GameMode))
		{
			Camera = GameMode->GetCamera();
			GameMode->OnClockTimeStop.AddDynamic(this, &AEnemyActor::TimeStop);
		}
	}
}

void AEnemyActor::HitWithWeapon(const int32 Damage, const bool bPlaySound)
{
	BoxComponent->OnComponentEndOverlap.Clear();
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
		if(IsValid(GameInstance))
		{
			GameInstance->AddScore(Score);
		}
	}

	if(bPlaySound)
	{
		UGameplayStatics::PlaySound2D(this, HitSound);	
	}
	
	FlipbookComponent->SetLooping(false);
	FlipbookComponent->SetFlipbook(BurnOutFlipbook);
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AEnemyActor::OnFinishedPlaying);
	FlipbookComponent->PlayFromStart();
}

void AEnemyActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	UBoxComponent* PawnBoxComponent = Cast<UBoxComponent>(OtherComp);
	if(IsValid(Pawn) && IsValid(PawnBoxComponent))
	{
		if(PawnBoxComponent->ComponentHasTag(FName(TEXT("Whip"))))
		{
			HitWithWeapon(Pawn->GetWhipDamage(), true);

			bWasHitWithWhip = true;
		}
		else if(PawnBoxComponent->ComponentHasTag(FName(TEXT("Character"))))
		{
			// Trigger damage on the pawn.
			Pawn->TakeDamage(DamageToPlayer, FDamageEvent(), nullptr, this);
		}
		return;
	}

	AWeaponActor* Weapon = Cast<AWeaponActor>(OtherActor);
	if(IsValid(Weapon))
	{
		HitWithWeapon(Weapon->GetWeaponDamage(), true);
		Weapon->Hit();

		bWasHitWithWhip = false;
	}
}

void AEnemyActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Camera == OtherActor)
	{
		Destroy();
	}
}

void AEnemyActor::OnFinishedPlaying()
{
	SpawnDrop();
	Destroy();
}

void AEnemyActor::RoundFlipbookLocation() const
{
	FVector RoundedLocation = GetActorLocation();
	RoundedLocation.X = static_cast<float>(FMath::RoundToInt(RoundedLocation.X));
	RoundedLocation.Z = static_cast<float>(FMath::RoundToInt(RoundedLocation.Z));
	FlipbookComponent->SetWorldLocation(RoundedLocation);
}

void AEnemyActor::SpawnDrop()
{
	const float Probability = FMath::FRandRange(0.0f, 1.0f);
	if(Probability > ProbabilityOfDrop)
	{
		return;
	}
	
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
		
	if(GameInstance->GetWhipType() != EWhipType::Long && !IsValid(UGameplayStatics::GetActorOfClass(this, AWhipUpgradeActor::StaticClass())))
	{
		World->SpawnActor<AWhipUpgradeActor>(WhipUpgradeClass, GetActorLocation(), GetActorRotation(), SpawnParameters);
		return;
	}

	const float ShotProbability = FMath::FRandRange(0.0f, 1.0f);
	if(ShotProbability < ProbabilityOfShotDrop && !bWasHitWithWhip && GameInstance->GetShot() < 3 && !IsValid(UGameplayStatics::GetActorOfClass(this, AShotPickupActor::StaticClass())))
	{
		AShotPickupActor* ShotActor = World->SpawnActorDeferred<AShotPickupActor>(ShotUpgradeClass, GetActorTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if(IsValid(ShotActor))
		{
			ShotActor->SetIsTripleShot(GameInstance->GetShot() == 2);
			UGameplayStatics::FinishSpawningActor(ShotActor, GetActorTransform());
			return;
		}
	}

	if(ClassesToSpawnOnDestroy.Num() > 0)
	{
		const int RandomSpawn = FMath::RandRange(0, ClassesToSpawnOnDestroy.Num() - 1);
		World->SpawnActor<AActor>(ClassesToSpawnOnDestroy[RandomSpawn], GetActorLocation(), GetActorRotation(), SpawnParameters);
	}
}

