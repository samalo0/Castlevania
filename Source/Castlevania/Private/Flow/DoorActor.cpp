//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/30/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Flow/DoorActor.h"

#include "Components/BoxComponent.h"
#include "Core/CastlevaniaCameraActor.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Core/CastlevaniaGameModeBase.h"
#include "Pawn/CastlevaniaPawn.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"

ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
	
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(GetRootComponent());
	SpriteComponent->SetCollisionProfileName("NoCollision");
	SpriteComponent->SetGenerateOverlapEvents(false);

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxComponent"));
	CollisionBoxComponent->SetupAttachment(GetRootComponent());
	CollisionBoxComponent->SetCollisionProfileName("BlockAll");
	CollisionBoxComponent->SetGenerateOverlapEvents(false);
	CollisionBoxComponent->SetRelativeLocation(FVector(-4.0f, 0.0f, 0.0f));
	CollisionBoxComponent->SetBoxExtent(FVector(4.0f, 25.0f, 24.0f));
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OnBoxBeginOverlap);
	
	ClosedSprite = SpriteComponent->GetSprite();
}

void ADoorActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	UWorld* World = GetWorld();
	if(IsValid(Pawn) && IsValid(World))
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		UCastlevaniaGameInstance* GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
		ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(Controller) && IsValid(GameInstance) && IsValid(GameMode))
		{
			// Pause timer.
			GameMode->SetActorTickEnabled(false);
			
			Pawn->DisableInput(Controller);
			CameraReference->SetActorTickEnabled(false);

			GameInstance->SetStage(NewStage);
			
			SetActorTickEnabled(true);
		}
	}
}

void ADoorActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch(State)
	{
	case EDoorState::Closed:
		{
			// Interpolate camera into position.
			const FVector CameraLocation = CameraReference->GetActorLocation();
			const float DesiredX = GetActorLocation().X;
			const float NewX = FMath::FInterpConstantTo(CameraLocation.X, DesiredX, DeltaSeconds, CameraInterpolationSpeed);
			CameraReference->SetActorLocation(FVector(NewX, CameraLocation.Y, CameraLocation.Z));

			if(FMath::IsNearlyEqual(NewX, DesiredX))
			{
				SpriteComponent->SetSprite(Open1Sprite);
				State = EDoorState::Opening;
				UGameplayStatics::PlaySound2D(this, DoorSound);
				FrameAccumulator = 0.0f;
			}
		}
		break;
	case EDoorState::Opening:
		FrameAccumulator += DeltaSeconds;
		if(FrameAccumulator > FrameTime)
		{
			SpriteComponent->SetSprite(Open2Sprite);
			State = EDoorState::Open;
			FrameAccumulator = 0.0f;

			// Move character through the open door.
			CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Pawn->SetForceMove(true, GetActorScale3D().X == 1.0f);
		}
		break;
	case EDoorState::Open:
		if((GetActorScale3D().X == 1.0f && Pawn->GetActorLocation().X >= NewPawnX) ||
			(GetActorScale3D().X == -1.0f && Pawn->GetActorLocation().X <= NewPawnX))
		{
			Pawn->SetForceMove(false, false);
			SpriteComponent->SetSprite(Open1Sprite);
			State = EDoorState::Closing;
		}
		break;
	case EDoorState::Closing:
		FrameAccumulator += DeltaSeconds;
		if(FrameAccumulator > FrameTime)
		{
			State = EDoorState::FinalClosed;

			SpriteComponent->SetSprite(ClosedSprite);
			UGameplayStatics::PlaySound2D(this, DoorSound);
			FrameAccumulator = 0.0f;
		}
		break;
	case EDoorState::FinalClosed:
		{
			// Interpolate camera into position.
			const FVector CameraLocation = CameraReference->GetActorLocation();
			const float NewX = FMath::FInterpConstantTo(CameraLocation.X, NewCameraX, DeltaSeconds, CameraInterpolationSpeed);
			CameraReference->SetActorLocation(FVector(NewX, CameraLocation.Y, CameraLocation.Z));

			if(FMath::IsNearlyEqual(NewX, NewCameraX))
			{
				CameraReference->SetMinimumAndMaximumX(NewCameraMinimumX, NewCameraMaximumX);
				CameraReference->SetActorTickEnabled(true);
				SetActorTickEnabled(false);

				UWorld* World = GetWorld();
				if(IsValid(World))
				{
					APlayerController* Controller = World->GetFirstPlayerController();
					ACastlevaniaGameModeBase* GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
					if(IsValid(Controller))
					{
						Pawn->EnableInput(Controller);

						CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

						// Continue timer.
						GameMode->SetActorTickEnabled(true);
					}
				}
			}
		}
		break;
	}
}
