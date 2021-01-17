//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Weapon/BoomerangActor.h"

#include "CastlevaniaCameraActor.h"
#include "CastlevaniaFunctionLibrary.h"
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
		Velocity = InitialInterpolationSpeed;
	}
	else
	{
		State = EBoomerangState::StartGoingLeft;
		DesiredX = CurrentLocation.X - Range;
		Velocity = -1.0f * InitialInterpolationSpeed;
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
	
	UpdateState(DeltaSeconds);

	LocationFloat.X += Velocity * DeltaSeconds;
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);	
	}
}

void ABoomerangActor::UpdateState(const float DeltaSeconds)
{
	const FVector CameraViewport = Camera->GetCameraViewportExtent();
	const float CameraMaxX = Camera->GetActorLocation().X + CameraViewport.X - 32.0f;
	const float CameraMinX = Camera->GetActorLocation().X - CameraViewport.X + 32.0f;
	
	switch(State)
	{
	case EBoomerangState::StartGoingRight:
		if(LocationFloat.X >= DesiredX || LocationFloat.X >= CameraMaxX)
		{
			State = EBoomerangState::SlowingDownRight;
		}
		break;
	case EBoomerangState::SlowingDownRight:
		Velocity += TurnAroundAcceleration * DeltaSeconds;

		if(Velocity <= 0.0f)
		{
			SetActorScale3D(FVector(-1.0f, 1.0f, 1.0f));
			State = EBoomerangState::SpeedingUpLeft;
		}
		break;
	case EBoomerangState::SpeedingUpLeft:
		Velocity += TurnAroundAcceleration * DeltaSeconds;
		if(Velocity <= (InitialInterpolationSpeed * -1.0f))
		{
			Velocity = InitialInterpolationSpeed * -1.0f;
			State = EBoomerangState::FinalGoingLeft;
		}
		break;
	case EBoomerangState::FinalGoingLeft:
		break;
	case EBoomerangState::StartGoingLeft:
		if(LocationFloat.X <= DesiredX || LocationFloat.X <= CameraMinX)
		{
			State = EBoomerangState::SlowingDownLeft;
		}
		break;
	case EBoomerangState::SlowingDownLeft:
		Velocity -= TurnAroundAcceleration * DeltaSeconds;

		if(Velocity >= 0.0f)
		{
			SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
			State = EBoomerangState::SpeedingUpRight;
		}
		break;
	case EBoomerangState::SpeedingUpRight:
		Velocity -= TurnAroundAcceleration * DeltaSeconds;

		if(Velocity >= InitialInterpolationSpeed)
		{
			Velocity = InitialInterpolationSpeed;
			State = EBoomerangState::FinalGoingRight;
		}
		break;		
	case EBoomerangState::FinalGoingRight:
		break;
	default:
		SetActorTickEnabled(false);
		break;
	}
}
