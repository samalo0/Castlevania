//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/18/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/CrusherActor.h"

#include "CastlevaniaFunctionLibrary.h"
#include "CastlevaniaGameInstance.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "PaperSpriteComponent.h"

ACrusherActor::ACrusherActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
	RootSceneComponent->SetMobility(EComponentMobility::Static);
	
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(GetRootComponent());
	SpriteComponent->SetMobility(EComponentMobility::Movable);
	SpriteComponent->SetCollisionProfileName("OverlapOnlyPawn");
}

void ACrusherActor::BeginPlay()
{
	Super::BeginPlay();

	LocationFloat = SpriteComponent->GetRelativeLocation();

	SpriteComponent->OnComponentBeginOverlap.AddDynamic(this, &ACrusherActor::OnBeginOverlap);

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("%s unable to get world."), *GetNameSafe(this));
		return;
	}
	
	ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
	if(!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("%s unable to get game mode."), *GetNameSafe(this));
		return;
	}

	UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
	if(!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("%s unable to get game instance."), *GetNameSafe(this));
		return;
	}
	
	GameMode->OnClockTimeStop.AddDynamic(this, &ACrusherActor::TimeStop);

	// Enables the crusher if you restart the level at it's stage.
	if(ActiveStage == GameInstance->GetStage())
	{
		SetActorTickEnabled(true);
	}

	GameInstance->OnStageChanged.AddUObject(this, &ACrusherActor::OnStageChanged);
}

void ACrusherActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		Pawn->TakeDamage(16.0f, FDamageEvent(), nullptr, this);
	}
}

void ACrusherActor::OnStageChanged(int32& Stage)
{
	SetActorTickEnabled(ActiveStage == Stage);
}

void ACrusherActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bMovingDown)
	{
		LocationFloat.Z += LowerVelocity * DeltaSeconds;		
	}
	else
	{
		LocationFloat.Z += RaiseVelocity * DeltaSeconds;
	}
	
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!SpriteComponent->GetRelativeLocation().Equals(LocationInteger))
	{
		SpriteComponent->SetRelativeLocation(LocationInteger);

		if((bMovingDown && LocationInteger.Z <= StopRelativeLocation.Z) ||
		   (!bMovingDown && LocationInteger.Z >= 0.0f))
		{
			bMovingDown = !bMovingDown;
		}
	}
}

void ACrusherActor::TimeStop(const bool bIsActive)
{
	SetActorTickEnabled(!bIsActive);
}
