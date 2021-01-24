//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "CastlevaniaStatics.h"
#include "Engine/GameInstance.h"
#include "CastlevaniaGameInstance.generated.h"

class ACastlevaniaPawn;

DECLARE_MULTICAST_DELEGATE_OneParam(FCastlevaniaStageChangeSignature, int32&);

USTRUCT()
struct FStage
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector CameraStartingLocation;

	UPROPERTY(EditAnywhere)
	FVector2D CameraMinAndMaxX;

	UPROPERTY(EditAnywhere)
	FName LevelName;

	UPROPERTY(EditAnywhere)
	USoundBase* StageMusic;
		
};

class UCastlevaniaUserWidget;

UCLASS()
class CASTLEVANIA_API UCastlevaniaGameInstance : public UGameInstance
{

	GENERATED_BODY()
	
#pragma region HUD

public:

	void AddHearts(const int32 Number);

	void AddPlayerHealth(const int32 Number);

	void AddPlayerLives(const int32 Number);

	void AddScore(const int32 Number);

	FORCEINLINE int32 GetEnemyHealth() const { return EnemyHealth; }
	
	FORCEINLINE int32 GetHearts() const { return Hearts; }

	FORCEINLINE int32 GetPlayerHealth() const { return PlayerHealth; }

	FORCEINLINE int32 GetShot() const { return Shot; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	
	FORCEINLINE EWhipType GetWhipType() const { return WhipType; }

	void InitializeHudWidget(UCastlevaniaUserWidget* Widget, ACastlevaniaPawn* Pawn);

	void SetEnemyHealth(const int32 Number);
	
	void SetShot(int32 Count);
	
	void SetWeapon(EWeaponType Type);
	
	void SetWhipType(const EWhipType Type) { WhipType = Type; }

	// Delegate which fires when the stage has changed during a level.
	FCastlevaniaStageChangeSignature OnStageChanged;
	
protected:
	
	int32 EnemyHealth = 16;
	
	UPROPERTY(EditDefaultsOnly)
	int32 InitialEnemyHealth = 16;

	UPROPERTY(EditDefaultsOnly)
	int32 InitialHearts = 5;

	UPROPERTY(EditDefaultsOnly)
	int32 InitialPlayerHealth = 16;

	UPROPERTY(EditDefaultsOnly)
	int32 InitialPlayerLives = 3;
	
	int32 Hearts = 5;

	UPROPERTY(Transient)
	UCastlevaniaUserWidget* HudWidget;
	
	int32 PlayerHealth = 16;

	int32 PlayerLives = 3;
	
	int32 Score = 0;

	int32 Shot = 1;

	// The current stage; each level is broken up into stages, which are locations you can start at once you have died.
	UPROPERTY(EditDefaultsOnly)
	int32 Stage = 0;

	UPROPERTY(EditDefaultsOnly)
	TArray<FStage> Stages;
	
	EWeaponType WeaponType = EWeaponType::None;
	
	EWhipType WhipType = EWhipType::Leather;
	
#pragma endregion

#pragma region Stage

public:

	FORCEINLINE int32 GetStage() const { return Stage;}
	
	void GetCameraSettingsForStage(float& Min, float& Max, FVector& Location);
	
	USoundBase* GetStageMusic() { return Stages[Stage].StageMusic; }
	
	void LoadNextStage();

	void ReloadCurrentStage();

	// Set the stage variable and update the HUD widget; used for intermediate stages as you go through doors.
	void SetStage(const int32 NewStage);
	
#pragma endregion
	
};
