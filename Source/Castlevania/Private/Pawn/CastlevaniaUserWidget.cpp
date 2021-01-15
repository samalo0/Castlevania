//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pawn/CastlevaniaUserWidget.h"

#include "Button.h"
#include "Pawn/CastlevaniaPawn.h"
#include "Core/CastlevaniaStatics.h"
#include "Image.h"
#include "ProgressBar.h"
#include "TextBlock.h"

void UCastlevaniaUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AButton->OnPressed.AddDynamic(this, &UCastlevaniaUserWidget::AButtonPressed);
	BButton->OnPressed.AddDynamic(this, &UCastlevaniaUserWidget::BButtonPressed);

	DownButton->OnPressed.AddDynamic(this, &UCastlevaniaUserWidget::DownButtonPressed);
	DownButton->OnReleased.AddDynamic(this, &UCastlevaniaUserWidget::DownButtonReleased);
	
	LeftButton->OnPressed.AddDynamic(this, &UCastlevaniaUserWidget::LeftButtonPressed);
	LeftButton->OnReleased.AddDynamic(this, &UCastlevaniaUserWidget::LeftButtonReleased);

	RightButton->OnPressed.AddDynamic(this, &UCastlevaniaUserWidget::RightButtonPressed);
	RightButton->OnReleased.AddDynamic(this, &UCastlevaniaUserWidget::RightButtonReleased);

	UpButton->OnPressed.AddDynamic(this, &UCastlevaniaUserWidget::UpButtonPressed);
	UpButton->OnReleased.AddDynamic(this, &UCastlevaniaUserWidget::UpButtonReleased);
}

void UCastlevaniaUserWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(IsValid(PlayerHealthBar) && !FMath::IsNearlyEqual(CurrentPlayerHealth, DesiredPlayerHealth, .01f))
	{
		CurrentPlayerHealth = FMath::FInterpConstantTo(CurrentPlayerHealth, DesiredPlayerHealth, InDeltaTime, HealthInterpolationSpeed);
		PlayerHealthBar->SetPercent(CurrentPlayerHealth);
	}

	if(IsValid(EnemyHealthBar) && !FMath::IsNearlyEqual(CurrentEnemyHealth, DesiredEnemyHealth, .01f))
	{
		CurrentEnemyHealth = FMath::FInterpConstantTo(CurrentEnemyHealth, DesiredEnemyHealth, InDeltaTime, HealthInterpolationSpeed);
		EnemyHealthBar->SetPercent(CurrentEnemyHealth);
	}
}

void UCastlevaniaUserWidget::SetEnemyHealth(const int32 Health)
{
	DesiredEnemyHealth = Health / 16.0f;
}

void UCastlevaniaUserWidget::SetHearts(const int32 Hearts) const
{
	if(IsValid(HeartsText))
	{
		const FString HeartsString = FString::Printf(TEXT("%02d"), Hearts);
		HeartsText->SetText(FText::AsCultureInvariant(HeartsString));
	}
}

void UCastlevaniaUserWidget::SetPlayerHealth(const int32 Health)
{
	DesiredPlayerHealth = Health / 16.0f;
}

void UCastlevaniaUserWidget::SetPlayerLives(const int32 Lives) const
{
	if(IsValid(PlayerLivesText))
	{
		const FString LivesString = FString::Printf(TEXT("%02d"), Lives);
		PlayerLivesText->SetText(FText::AsCultureInvariant(LivesString));
	}
}

void UCastlevaniaUserWidget::SetScore(const int32 Score) const
{
	if(IsValid(ScoreText))
	{
		const FString ScoreString = FString::Printf(TEXT("%06d"), Score);
		ScoreText->SetText(FText::AsCultureInvariant(ScoreString));
	}
}

void UCastlevaniaUserWidget::SetShot(const int32 Shot) const
{
	if(!IsValid(ShotDoubleImage) || !IsValid(ShotTripleImage))
	{
		return;
	}
	
	if(Shot <= 1)
	{
		ShotDoubleImage->SetVisibility(ESlateVisibility::Hidden);
		ShotTripleImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else if(Shot == 2)
	{
		ShotDoubleImage->SetVisibility(ESlateVisibility::Visible);
		ShotTripleImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ShotDoubleImage->SetVisibility(ESlateVisibility::Hidden);
		ShotTripleImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCastlevaniaUserWidget::SetStage(const int32 Stage) const
{
	if(IsValid(StageText))
	{
		const FString StageString = FString::Printf(TEXT("%02d"), Stage);
		StageText->SetText(FText::AsCultureInvariant(StageString));
	}
}

void UCastlevaniaUserWidget::SetTime(const int32 Time) const
{
	if(IsValid(TimeText))
	{
		const FString TimeString = FString::Printf(TEXT("%04d"), Time);
		TimeText->SetText(FText::AsCultureInvariant(TimeString));
	}
}

void UCastlevaniaUserWidget::SetWeaponType(const EWeaponType Type)
{
	AxeImage->SetVisibility(ESlateVisibility::Hidden);
	BoomerangImage->SetVisibility(ESlateVisibility::Hidden);
	ClockImage->SetVisibility(ESlateVisibility::Hidden);
	DaggerImage->SetVisibility(ESlateVisibility::Hidden);
	HolyWaterImage->SetVisibility(ESlateVisibility::Hidden);
	
	switch(Type)
	{
	case EWeaponType::Axe:
		AxeImage->SetVisibility(ESlateVisibility::Visible);
		break;
	case EWeaponType::Boomerang:
		BoomerangImage->SetVisibility(ESlateVisibility::Visible);
		break;
	case EWeaponType::Clock:
		ClockImage->SetVisibility(ESlateVisibility::Visible);
		break;
	case EWeaponType::Dagger:
		DaggerImage->SetVisibility(ESlateVisibility::Visible);
		break;
	case EWeaponType::HolyWater:
		HolyWaterImage->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}

#pragma region Control

void UCastlevaniaUserWidget::AButtonPressed()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Jump();
	}
}

void UCastlevaniaUserWidget::BButtonPressed()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Attack();
	}
}

void UCastlevaniaUserWidget::DownButtonPressed()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Down(true);		
	}
}

void UCastlevaniaUserWidget::DownButtonReleased()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Down(false);
	}
}

void UCastlevaniaUserWidget::LeftButtonPressed()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Left(true);		
	}
}

void UCastlevaniaUserWidget::LeftButtonReleased()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Left(false);
	}
}

void UCastlevaniaUserWidget::RightButtonPressed()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Right(true);		
	}
}

void UCastlevaniaUserWidget::RightButtonReleased()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Right(false);
	}
}

void UCastlevaniaUserWidget::UpButtonPressed()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Up(true);		
	}
}

void UCastlevaniaUserWidget::UpButtonReleased()
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(GetOwningPlayerPawn());
	if(IsValid(Pawn))
	{
		Pawn->Up(false);
	}
}

#pragma endregion
