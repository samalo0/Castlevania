//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/23/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Core/CastlevaniaGameInstance.h"

#include "Pawn/CastlevaniaPawn.h"
#include "Pawn/CastlevaniaUserWidget.h"
#include "Kismet/GameplayStatics.h"

#pragma region HUD

void UCastlevaniaGameInstance::AddHearts(const int32 Number)
{
	Hearts = FMath::Clamp(Hearts + Number, 0, 99);

	if(IsValid(HudWidget))
	{
		HudWidget->SetHearts(Hearts);
	}
}

void UCastlevaniaGameInstance::AddPlayerHealth(const int32 Number)
{
	PlayerHealth = FMath::Clamp(PlayerHealth + Number, 0, 16);

	if(IsValid(HudWidget))
	{
		HudWidget->SetPlayerHealth(PlayerHealth);
	}
}

void UCastlevaniaGameInstance::AddPlayerLives(const int32 Number)
{
	PlayerLives += Number;

	if(IsValid(HudWidget))
	{
		HudWidget->SetPlayerLives(PlayerLives);
	}
}

void UCastlevaniaGameInstance::AddScore(const int32 Number)
{
	Score += Number;

	if(IsValid(HudWidget))
	{
		HudWidget->SetScore(Score);
	}
}

void UCastlevaniaGameInstance::InitializeHudWidget(UCastlevaniaUserWidget* Widget, ACastlevaniaPawn* Pawn)
{
	HudWidget = Widget;

	if(IsValid(HudWidget))
	{
		HudWidget->SetEnemyHealth(EnemyHealth);
		HudWidget->SetHearts(Hearts);
		HudWidget->SetPlayerHealth(PlayerHealth);
		HudWidget->SetPlayerLives(PlayerLives);
		HudWidget->SetScore(Score);
		HudWidget->SetStage(Stage + 1);
		HudWidget->SetWeaponType(WeaponType);
	}
	
	if(IsValid(Pawn))
	{
		Pawn->SetWeapon(WeaponType);
		Pawn->SetWhipType(WhipType);
	}
}

void UCastlevaniaGameInstance::SetEnemyHealth(const int32 Number)
{
	EnemyHealth = Number;

	if(IsValid(HudWidget))
	{
		HudWidget->SetEnemyHealth(EnemyHealth);
	}
}

void UCastlevaniaGameInstance::SetShot(const int32 Count)
{
	Shot = Count;

	if(IsValid(HudWidget))
	{
		HudWidget->SetShot(Shot);
	}
}

void UCastlevaniaGameInstance::SetWeapon(const EWeaponType Type)
{
	WeaponType = Type;

	if(IsValid(HudWidget))
	{
		HudWidget->SetWeaponType(WeaponType);
	}
}

#pragma endregion

#pragma region Stage

void UCastlevaniaGameInstance::GetCameraSettingsForStage(float& Min, float& Max, FVector& Location)
{
	Min = Stages[Stage].CameraMinAndMaxX.X;
	Max = Stages[Stage].CameraMinAndMaxX.Y;
	Location = Stages[Stage].CameraStartingLocation;
}

void UCastlevaniaGameInstance::LoadNextStage()
{
	Stage++;
	if(Stage >= Stages.Num())
	{
		Stage = 0;
	}

	UGameplayStatics::OpenLevel(this, Stages[Stage].LevelName);
}

void UCastlevaniaGameInstance::ReloadCurrentStage()
{
	PlayerHealth = InitialPlayerHealth;
	EnemyHealth = InitialEnemyHealth;
	WeaponType = EWeaponType::None;
	WhipType = EWhipType::Leather;
	Shot = 1;
	Hearts = 5;
	
	UGameplayStatics::OpenLevel(this, Stages[Stage].LevelName);
}

void UCastlevaniaGameInstance::SetStage(const int32 NewStage)
{
	Stage = NewStage;
	if(IsValid(HudWidget))
	{
		HudWidget->SetStage(Stage + 1);
	}
}

#pragma endregion
