//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Core/CastlevaniaGameModeBase.h"

#include "Components/AudioComponent.h"
#include "CastlevaniaCameraActor.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Pawn/CastlevaniaPawn.h"
#include "Pawn/CastlevaniaUserWidget.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"

ACastlevaniaGameModeBase::ACastlevaniaGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	SetRootComponent(AudioComponent);
	AudioComponent->SetAutoActivate(false);
}

void ACastlevaniaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid world on begin play."));
		return;
	}
	
	if(!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid game instance on begin play."));
		return;
	}

	AudioComponent->SetSound(GameInstance->GetStageMusic());
	AudioComponent->Activate();
	
	HudWidget = CreateWidget<UCastlevaniaUserWidget>(GameInstance, HudWidgetClass);
	if(IsValid(HudWidget))
	{
		HudWidget->AddToViewport(0);
		GameInstance->InitializeHudWidget(HudWidget, GetPlayerPawn());
	}

	APlayerController* Controller = World->GetFirstPlayerController();
	if(!IsValid(Controller))
	{
		return;
	}

	// Fade in camera.
	Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0f, 0.0f), 0.1f, true, false);
}

AActor* ACastlevaniaGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid world when choosing player start."));
		return Super::ChoosePlayerStart_Implementation(Player);
	}
	
	GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
	if(!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid game instance when choosing player start."));
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	const int32 Stage = GameInstance->GetStage();

	const FString StageString = FString::Printf(TEXT("Stage%d"), Stage);
	const FName PlayerStartTag = FName(*StageString);
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutActors);

	for(AActor* Actor : OutActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
		if(IsValid(Actor) && PlayerStart->PlayerStartTag == PlayerStartTag)
		{
			return Actor;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Found no player starts with tag %s"), *StageString);
	return Super::ChoosePlayerStart_Implementation(Player);
}

APawn* ACastlevaniaGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return nullptr;
	}
	
	FTransform SpawnTransform = StartSpot->GetTransform();

	bool bFaceLeft = false;
	if(SpawnTransform.GetScale3D().X == -1.0f)
	{
		bFaceLeft = true;
		SpawnTransform.SetScale3D(FVector::OneVector);
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
	if(!IsValid(ResultPawn))
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnFor_Implementation: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}

	if(bFaceLeft)
	{
		ACastlevaniaPawn* CastlevaniaPawn = Cast<ACastlevaniaPawn>(ResultPawn);
		if(IsValid(CastlevaniaPawn))
		{
			CastlevaniaPawn->SetFacingDirection(-1.0f);
		}
	}

	return ResultPawn;
}

void ACastlevaniaGameModeBase::StartLevelTransition(const bool bNextStage)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	APlayerController* Controller = World->GetFirstPlayerController();
	if(!IsValid(Controller))
	{
		return;
	}

	SetActorTickEnabled(false);
	
	// Fade out camera
	Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0f, 1.0f), CameraFadeTime, true, true);

	// Delay and load next stage.
	FTimerHandle TimerHandle;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StartLevelTransitionAfterDelay", bNextStage);
	World->GetTimerManager().SetTimer(TimerHandle, Delegate, StageSwitchDelay, false);
}

void ACastlevaniaGameModeBase::StartLevelTransitionAfterDelay(const bool bNextStage) const
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	if(!IsValid(GameInstance))
	{
		return;
	}

	if(bNextStage)
	{
		GameInstance->LoadNextStage();	
	}
	else
	{
		GameInstance->ReloadCurrentStage();
	}
}

void ACastlevaniaGameModeBase::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LevelTimeFloat -= DeltaSeconds;
	const int32 NewLevelTimeSeconds = FMath::RoundToInt(LevelTimeFloat);
	if(LevelTimeSeconds != NewLevelTimeSeconds)
	{
		LevelTimeSeconds = NewLevelTimeSeconds;
		if(IsValid(HudWidget))
		{
			HudWidget->SetTime(LevelTimeSeconds);
		}

		if(LevelTimeSeconds == 0)
		{
			Pawn->TakeDamage(16, FDamageEvent(), nullptr, this);
		}
	}
}

#pragma region Audio

void ACastlevaniaGameModeBase::PauseMusic(const bool bPause) const
{
	AudioComponent->SetPaused(bPause);
}

void ACastlevaniaGameModeBase::PlayBossMusic() const
{
	AudioComponent->SetSound(BossMusic);
}

void ACastlevaniaGameModeBase::PlayDeathMusic() const
{
	AudioComponent->SetSound(DeathMusic);
}

#pragma endregion

#pragma region LevelCompletion

void ACastlevaniaGameModeBase::StartLevelCompletion()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	APlayerController* Controller = World->GetFirstPlayerController();
	if(IsValid(Controller) && IsValid(Pawn))
	{
		Pawn->DisableInput(Controller);
	}
	
	SetActorTickEnabled(false);

	GameInstance->AddPlayerHealth(16);
	GameInstance->SetEnemyHealth(16);
	
	AudioComponent->SetSound(LevelCompletionMusic);
	AudioComponent->OnAudioFinished.AddDynamic(this, &ACastlevaniaGameModeBase::StartLevelCompletion_AfterMusic);
	AudioComponent->Play();
}

void ACastlevaniaGameModeBase::StartLevelCompletion_AfterMusic()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	if(!IsValid(GameInstance))
	{
		return;
	}

	AudioComponent->Stop();

	if(LevelTimeSeconds > 0)
	{
		World->GetTimerManager().SetTimer(LevelCompletionTimerHandle, this, &ACastlevaniaGameModeBase::StartLevelCompletion_TimeCountdown, SecondCountdownDelay, true);
	}
	else if(GameInstance->GetHearts() > 0)
	{
		World->GetTimerManager().SetTimer(LevelCompletionTimerHandle, this, &ACastlevaniaGameModeBase::StartLevelCompletion_HeartsCountdown, HeartCountdownDelay, true);
	}
	else
	{
		StartLevelTransition(true);
	}
}

void ACastlevaniaGameModeBase::StartLevelCompletion_TimeCountdown()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	if(!IsValid(GameInstance))
	{
		return;
	}

	if(LevelTimeSeconds > 0)
	{
		UGameplayStatics::PlaySound2D(this, SecondCountdownSound);
		
		GameInstance->AddScore(10);
		
		LevelTimeSeconds--;
		HudWidget->SetTime(LevelTimeSeconds);
	}
	else if(GameInstance->GetHearts() > 0)
	{
		World->GetTimerManager().SetTimer(LevelCompletionTimerHandle, this, &ACastlevaniaGameModeBase::StartLevelCompletion_HeartsCountdown, HeartCountdownDelay, true);
	}
	else
	{
		World->GetTimerManager().ClearTimer(LevelCompletionTimerHandle);
		StartLevelTransition(true);
	}
}

void ACastlevaniaGameModeBase::StartLevelCompletion_HeartsCountdown()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	if(!IsValid(GameInstance))
	{
		return;
	}
	
	const int32 Hearts = GameInstance->GetHearts();
	if(Hearts > 0)
	{
		UGameplayStatics::PlaySound2D(this, HeartCountdownSound);
		
		GameInstance->AddScore(100);
		GameInstance->AddHearts(-1);
	}
	else
	{
		World->GetTimerManager().ClearTimer(LevelCompletionTimerHandle);
		StartLevelTransition(true);
	}
}

#pragma endregion

#pragma region Reference

ACastlevaniaCameraActor* ACastlevaniaGameModeBase::GetCamera()
{
	if(IsValid(Camera))
	{
		return Camera;
	}

	Camera = Cast<ACastlevaniaCameraActor>(UGameplayStatics::GetActorOfClass(this, ACastlevaniaCameraActor::StaticClass()));
	return Camera;
}

ADirectionalLight* ACastlevaniaGameModeBase::GetDirectionalLight()
{
	if(IsValid(DirectionalLight))
	{
		return DirectionalLight;
	}

	DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(this, ADirectionalLight::StaticClass()));
	return DirectionalLight;
}

ACastlevaniaPawn* ACastlevaniaGameModeBase::GetPlayerPawn()
{
	if(IsValid(Pawn))
	{
		return Pawn;
	}

	Pawn = Cast<ACastlevaniaPawn>(UGameplayStatics::GetActorOfClass(this, ACastlevaniaPawn::StaticClass()));
	return Pawn;
}

#pragma endregion