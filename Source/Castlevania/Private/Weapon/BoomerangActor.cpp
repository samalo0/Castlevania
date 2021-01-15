//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Weapon/BoomerangActor.h"

#include "CastlevaniaCameraActor.h"
#include "CastlevaniaGameModeBase.h"
#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ABoomerangActor::ABoomerangActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName("Weapon");
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetGenerateOverlapEvents(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetRootComponent());
	AudioComponent->SetAutoActivate(false);

	WeaponDamage = 2;
}

void ABoomerangActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(GameMode))
		{
			Camera = GameMode->GetCamera();
		}
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABoomerangActor::OnBeginOverlap);

	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABoomerangActor::OnEndOverlap);

	const FVector CurrentLocation = GetActorLocation();
	if(GetActorScale().X == 1.0f)
	{
		State = EBoomerangState::StartGoingRight;	
		DesiredX = CurrentLocation.X + Range;
		InterpolationSpeed = InitialInterpolationSpeed;
	}
	else
	{
		State = EBoomerangState::StartGoingLeft;
		DesiredX = CurrentLocation.X - Range;
		InterpolationSpeed = -1.0f * InitialInterpolationSpeed;
	}

	AudioComponent->Activate();
	
	SetActorTickEnabled(true);
}

void ABoomerangActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		Destroy();
	}
}

void ABoomerangActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == Camera)
	{
		Destroy();
	}
}

void ABoomerangActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector CameraViewport = Camera->GetCameraViewportExtent();
	const float CameraMaxX = Camera->GetActorLocation().X + CameraViewport.X - 32.0f;
	const float CameraMinX = Camera->GetActorLocation().X - CameraViewport.X + 32.0f;
	
	switch(State)
	{
	case EBoomerangState::StartGoingRight:
		{
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
			if(NewLocation.X >= DesiredX || NewLocation.X >= CameraMaxX)
			{
				State = EBoomerangState::SlowingDownRight;
			}
		}
		break;
	case EBoomerangState::SlowingDownRight:
		{
			InterpolationSpeed += TurnAroundAcceleration * DeltaSeconds;
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
			if(InterpolationSpeed <= 0.0f)
			{
				SetActorScale3D(FVector(-1.0f, 1.0f, 1.0f));
				State = EBoomerangState::SpeedingUpLeft;
			}
		}
		break;
	case EBoomerangState::SpeedingUpLeft:
		{
			InterpolationSpeed += TurnAroundAcceleration * DeltaSeconds;
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
			if(InterpolationSpeed <= (InitialInterpolationSpeed * -1.0f))
			{
				InterpolationSpeed = InitialInterpolationSpeed * -1.0f;
				State = EBoomerangState::FinalGoingLeft;
			}
		}
		break;
	case EBoomerangState::FinalGoingLeft:
		{
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
		}
		break;
	case EBoomerangState::StartGoingLeft:
		{
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
			if(NewLocation.X <= DesiredX || NewLocation.X <= CameraMinX)
			{
				State = EBoomerangState::SlowingDownLeft;
			}
		}
		break;
	case EBoomerangState::SlowingDownLeft:
		{
			InterpolationSpeed -= TurnAroundAcceleration * DeltaSeconds;
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
			if(InterpolationSpeed >= 0.0f)
			{
				SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
				State = EBoomerangState::SpeedingUpRight;
			}
		}
		break;
	case EBoomerangState::SpeedingUpRight:
		{
			InterpolationSpeed -= TurnAroundAcceleration * DeltaSeconds;
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
			if(InterpolationSpeed >= InitialInterpolationSpeed)
			{
				InterpolationSpeed = InitialInterpolationSpeed;
				State = EBoomerangState::FinalGoingRight;
			}
		}
		break;		
	case EBoomerangState::FinalGoingRight:
		{
			const FVector NewLocation = GetActorLocation() + FVector(InterpolationSpeed * DeltaSeconds, 0.0f, 0.0f);
			SetActorLocation(NewLocation);
		}		
		break;
	default:
		SetActorTickEnabled(false);
		break;
	}	

	
	
	// Round flipbook location to the nearest pixel, for authenticity.
	FVector RoundLocation = GetActorLocation();
	RoundLocation.X = static_cast<float>(FMath::RoundToInt(RoundLocation.X));
	RoundLocation.Z = static_cast<float>(FMath::RoundToInt(RoundLocation.Z));
	FlipbookComponent->SetWorldLocation(RoundLocation);
}
