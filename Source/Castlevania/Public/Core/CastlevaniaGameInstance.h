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

USTRUCT()
struct FStage
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MaximumX;
	
	UPROPERTY(EditAnywhere)
	float MinimumX;
	
	UPROPERTY(EditAnywhere)
	int32 StageNumber;

	UPROPERTY(EditAnywhere)
	USoundBase* StageMusic;
	
	UPROPERTY(EditAnywhere)
	FName StageName;
	
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
	
	int32 Stage = 0;

	UPROPERTY(EditDefaultsOnly)
	TArray<FStage> Stages;
	
	EWeaponType WeaponType = EWeaponType::None;
	
	EWhipType WhipType = EWhipType::Leather;
	
#pragma endregion

#pragma region Stage

public:

	FORCEINLINE int32 GetStage() const { return Stage;}
	
	void GetStageMaximumAndMinimumX(float& Min, float& Max);
	
	USoundBase* GetStageMusic() { return Stages[Stage].StageMusic; }
	
	void LoadNextStage();

	void ReloadCurrentStage();

	// Set the stage variable and update the HUD widget; used for intermediate stages as you go through doors.
	void SetStage(const int32 NewStage);
	
#pragma endregion
	
};
