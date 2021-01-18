//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CastlevaniaGameModeBase.generated.h"

class ACastlevaniaCameraActor;
class ACastlevaniaPawn;
class UCastlevaniaUserWidget;
class ADirectionalLight;
class UCastlevaniaGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClockTimeStopSignature, bool, bIsActive);

UCLASS()
class CASTLEVANIA_API ACastlevaniaGameModeBase : public AGameModeBase
{

	GENERATED_BODY()

public:

	ACastlevaniaGameModeBase();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	void SetClockActivated(const bool bActive) const { OnClockTimeStop.Broadcast(bActive); }

	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	
	void StartLevelTransition(bool bNextStage);

	UFUNCTION()
	void StartLevelTransitionAfterDelay(bool bNextStage) const;
	
	virtual void Tick(float DeltaSeconds) override;

	FClockTimeStopSignature OnClockTimeStop;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode | General")
	float CameraFadeTime = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameMode | General")
	float StageSwitchDelay = 0.5f;

	FTimerHandle LevelCompletionTimerHandle;
	
	float LevelTimeFloat = 300.0f;

	int32 LevelTimeSeconds = 0;

#pragma region Audio

public:

	void PauseMusic(bool bPause) const;
	
	void PlayBossMusic() const;
	
	void PlayDeathMusic() const;
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode | Audio")
	USoundBase* BossMusic;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameMode | Audio")
	USoundBase* DeathMusic;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode | Audio")
	USoundBase* HeartCountdownSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameMode | Audio")
	USoundBase* LevelCompletionMusic;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode | Audio")
	USoundBase* SecondCountdownSound;
	
#pragma endregion
	
#pragma region HUD

public:

	FORCEINLINE UCastlevaniaUserWidget* GetHudWidget() const { return HudWidget; }
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "GameMode | HUD")
	TSubclassOf<UCastlevaniaUserWidget> HudWidgetClass;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "GameMode | HUD")
	UCastlevaniaUserWidget* HudWidget;
	
#pragma endregion

#pragma region LevelCompletion

public:
	
	void StartLevelCompletion();

protected:
	
	UFUNCTION()
    void StartLevelCompletion_AfterMusic();
	
	void StartLevelCompletion_TimeCountdown();

	void StartLevelCompletion_HeartsCountdown();

	UPROPERTY(EditDefaultsOnly, Category = "GameMode | LevelCompletion")
	float HeartCountdownDelay = 0.05;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameMode | LevelCompletion")
	float SecondCountdownDelay = 0.05;
	
#pragma endregion
	
#pragma region Reference

public:

	ACastlevaniaCameraActor* GetCamera();

	ADirectionalLight* GetDirectionalLight();
	
	ACastlevaniaPawn* GetPlayerPawn();
	
protected:

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "GameMode | Reference")
	ACastlevaniaCameraActor* Camera;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "GameMode | Reference")
	ADirectionalLight* DirectionalLight;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "GameMode | Reference")
	UCastlevaniaGameInstance* GameInstance;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "GameMode | Reference")
	ACastlevaniaPawn* Pawn;
	
#pragma endregion
	
};
