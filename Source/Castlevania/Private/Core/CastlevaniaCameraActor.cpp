//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Core/CastlevaniaCameraActor.h"


#include "CastlevaniaFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "CastlevaniaGameInstance.h"
#include "CastlevaniaGameModeBase.h"

ACastlevaniaCameraActor::ACastlevaniaCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
	GetCameraComponent()->SetOrthoWidth(256.0f);
	GetCameraComponent()->SetAspectRatio(256.0f/240.0f);
	GetCameraComponent()->SetOrthoFarClipPlane(500.0f);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetCameraComponent());
	BoxComponent->SetBoxExtent(FVector(128.0f, 200.0f, 120.0f), false);
	BoxComponent->SetCollisionProfileName(TEXT("CameraVisibility"));
	BoxComponent->SetUsingAbsoluteRotation(true);
}

void ACastlevaniaCameraActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
		if(IsValid(GameMode) && IsValid(GameInstance))
		{
			Pawn = GameMode->GetPlayerPawn();
			if(IsValid(Pawn))
			{
				FVector Location;
				GameInstance->GetCameraSettingsForStage(MinimumX, MaximumX, Location);
				SetActorLocation(Location);

				SetActorTickEnabled(true);
			}
		}
	}
}

FVector ACastlevaniaCameraActor::GetCameraViewportExtent() const
{
	return BoxComponent->GetScaledBoxExtent();
}

bool ACastlevaniaCameraActor::IsLocationInViewport(const FVector Location) const
{
	const FVector ViewportExtent = BoxComponent->GetScaledBoxExtent();
	const FVector ViewportLocation = BoxComponent->GetComponentLocation();

	return (Location.X <= ViewportLocation.X + ViewportExtent.X && Location.X >= ViewportLocation.X - ViewportExtent.X &&
		Location.Z <= ViewportLocation.Z + ViewportExtent.Z && Location.Z >= ViewportLocation.Z - ViewportExtent.Z);
}

void ACastlevaniaCameraActor::SetMinimumAndMaximumX(const float MinX, const float MaxX)
{
	MinimumX = MinX;
	MaximumX = MaxX;
}

void ACastlevaniaCameraActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsValid(Pawn))
	{
		SetActorTickEnabled(false);
		return;
	}
	
	const FVector PawnLocation = Pawn->GetActorLocation();

	if(PawnLocation.X >= MinimumX && PawnLocation.X <= MaximumX || bIgnoreMinAndMaxAndTrackPlayer)
	{
		const FVector NewLocation = UCastlevaniaFunctionLibrary::RoundVectorToInt(FVector(PawnLocation.X, GetActorLocation().Y, GetActorLocation().Z));
		if(!GetActorLocation().Equals(NewLocation, 0.99f))
		{
			SetActorLocation(NewLocation);	
		}
	}
}
