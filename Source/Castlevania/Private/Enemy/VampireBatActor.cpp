//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/31/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/VampireBatActor.h"


#include "CastlevaniaFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"

AVampireBatActor::AVampireBatActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxComponent"));
	TriggerBoxComponent->SetupAttachment(GetRootComponent());
	TriggerBoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
}

void AVampireBatActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	if(!bStartHanging)
	{
		TriggerBoxComponent->DestroyComponent();
		SetMovementBasedOnPlayerLocation();
		SetActorTickEnabled(true);
	}
	else
	{
		TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVampireBatActor::OnTriggerBoxOverlap);
	}
}

void AVampireBatActor::HitWithWeapon(const int32 Damage, const bool bPlaySound)
{
	SetActorTickEnabled(false);

	Super::HitWithWeapon(Damage, bPlaySound);
}

void AVampireBatActor::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		TriggerBoxComponent->DestroyComponent();
		
		FlipbookComponent->SetFlipbook(FlyingFlipbook);
		SetMovementBasedOnPlayerLocation();
		SetActorTickEnabled(true);
	}
}

void AVampireBatActor::SetMovementBasedOnPlayerLocation()
{
	if(IsValid(GameMode))
	{
		ACastlevaniaPawn* Pawn = GameMode->GetPlayerPawn();
		if(IsValid(Pawn))
		{
			if(Pawn->GetActorLocation().X < GetActorLocation().X)
			{
				FlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
				MovementSpeed = InitialMovementSpeed * -1.0f;
			}
			else
			{
				MovementSpeed = InitialMovementSpeed;
			}
	
			InitialZ = GetActorLocation().Z;
		}
	}
}

void AVampireBatActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Accumulator += DeltaSeconds;

	LocationFloat.X += MovementSpeed * DeltaSeconds;
	LocationFloat.Z = InitialZ + Amplitude * FMath::Sin(2 * PI * Frequency * Accumulator);

	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);	
	}
}

void AVampireBatActor::TimeStop(const bool bEnable)
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
