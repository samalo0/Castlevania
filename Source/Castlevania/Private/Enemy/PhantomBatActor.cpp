//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/PhantomBatActor.h"

#include "CastlevaniaCameraActor.h"
#include "CastlevaniaGameInstance.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "HolyWaterActor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

APhantomBatActor::APhantomBatActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APhantomBatActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		if(IsValid(GameMode))
		{
			Pawn = GameMode->GetPlayerPawn();
		}
	}

	FlipbookComponent->SetComponentTickEnabled(false);
}

void APhantomBatActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AHolyWaterActor* HolyWater = Cast<AHolyWaterActor>(OtherActor);
	if(IsValid(HolyWater))
	{
		TimeStop(true);
	}
}

void APhantomBatActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex)
{
	AHolyWaterActor* HolyWater = Cast<AHolyWaterActor>(OtherActor);
	if(IsValid(HolyWater))
	{
		TimeStop(false);
	}
}

void APhantomBatActor::HitWithWeapon(const int32 Damage, const bool bPlaySound)
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
	
	if(World->GetTimeSeconds() < HitCooldown)
	{
		return;
	}

	HitCooldown = World->GetTimeSeconds() + HitCooldownTime;
	
	UGameplayStatics::PlaySound2D(this, HitSound);

	int32 EnemyHealth = GameInstance->GetEnemyHealth();
	EnemyHealth = FMath::Clamp(EnemyHealth - Damage, 0, 16);;

	GameInstance->SetEnemyHealth(EnemyHealth);
	
	if(EnemyHealth == 0)
	{
		BoxComponent->OnComponentEndOverlap.Clear();
		BoxComponent->OnComponentBeginOverlap.Clear();
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		State = EPhantomBatState::Death;
		SetActorTickEnabled(false);

		FlipbookComponent->SetLooping(false);
		FlipbookComponent->SetFlipbook(BurnOutFlipbook);
		FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &APhantomBatActor::OnFinishedPlaying);
		FlipbookComponent->PlayFromStart();
	}
}

void APhantomBatActor::OnFinishedPlaying()
{
	if(IsValid(GameMode))
	{
		GameMode->StartLevelCompletion();
	}

	Destroy();
}

void APhantomBatActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	switch(State)
	{
	case EPhantomBatState::InitialWait:
		State = EPhantomBatState::MoveToDestination;

		FlipbookComponent->SetComponentTickEnabled(true);
		FlipbookComponent->SetFlipbook(FlyingFlipbook);
		BoxComponent->SetBoxExtent(FlyingCollisionBoxExtent);

		MovementSpeed = SlowMovementSpeed;
	
		if(IsValid(Camera))
		{
			const FVector CameraLocation = Camera->GetActorLocation();
			Destination = FVector(CameraLocation.X, GetActorLocation().Y, CameraLocation.Z);
		}
		break;
	case EPhantomBatState::MoveToDestination:
		{
			const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), Destination, DeltaSeconds, MovementSpeed);
			SetActorLocation(NewLocation);

			if(NewLocation.Equals(Destination, 0.1f))
			{
				State = EPhantomBatState::WaitRandomTime;
				WaitTime = FMath::FRandRange(MinimumWaitTime, MaximumWaitTime);
			}
		}
		break;
	case EPhantomBatState::WaitRandomTime:
		WaitTime -= DeltaSeconds;
		if(WaitTime <= 0.0f)
		{
			// Allow twice in a row with a random chance.
			const float TwiceInARow = FMath::FRandRange(0.0f, 1.0f);
			if(TwiceInARow < 0.25f)
			{
				bLastTowardsPlayer = !bLastTowardsPlayer;
			}
			
			if(bLastTowardsPlayer)
			{
				bLastTowardsPlayer = false;
				State = EPhantomBatState::GetLocationAwayFromPlayer;
				MovementSpeed = SlowMovementSpeed;
			}
			else
			{
				bLastTowardsPlayer = true;
				State = EPhantomBatState::GetPlayerLocation;
				MovementSpeed = FastMovementSpeed;
			}
		}
		break;
	case EPhantomBatState::GetPlayerLocation:
		if(IsValid(Pawn))
		{
			const FVector PawnLocation = Pawn->GetActorLocation();
			Destination = FVector(PawnLocation.X, GetActorLocation().Y, PawnLocation.Z);
			MovementSpeed = FastMovementSpeed;
			
			State = EPhantomBatState::MoveToDestination;
		}
		break;
	case EPhantomBatState::GetLocationAwayFromPlayer:
		if(IsValid(Pawn) && IsValid(Camera))
		{
			const FVector CameraLocation = Camera->GetActorLocation();
			float AdderX = FMath::FRandRange(64.0f, 96.0f);
			if(Pawn->GetActorLocation().X > CameraLocation.X)
			{
				AdderX *= -1.0f;
			}
			const float AdderZ = FMath::FRandRange(-32.0f, 32.0f);

			Destination = FVector(CameraLocation.X + AdderX, GetActorLocation().Y, CameraLocation.Z + AdderZ);
			
			State = EPhantomBatState::MoveToDestination;
		}
		break;
	default:
		break;
	}

	RoundFlipbookLocation();
}

void APhantomBatActor::TimeStop(const bool bEnable)
{
	if(State != EPhantomBatState::InitialWait && State != EPhantomBatState::Death)
	{
		if(bEnable)
		{
			SetActorTickEnabled(false);
			FlipbookComponent->Stop();
		}
		else 
		{
			SetActorTickEnabled(true);
			FlipbookComponent->Play();
		}	
	}
}
