//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CastlevaniaUserWidget.generated.h"

class UButton;
class UImage;
class UProgressBar;
class UTextBlock;
enum class EWeaponType : uint8;

UCLASS()
class CASTLEVANIA_API UCastlevaniaUserWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	
	void SetHearts(int32 Hearts) const;

	void SetEnemyHealth(int32 Health);
	
	void SetPlayerHealth(int32 Health);

	void SetPlayerLives(int32 Lives) const;
	
	void SetScore(int32 Score) const;

	void SetShot(int32 Shot) const;
	
	void SetStage(int32 Stage) const;
	
	void SetTime(int32 Time) const;

	void SetWeaponType(EWeaponType Type);
	
protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* AxeImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BoomerangImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* ClockImage;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* DaggerImage;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* EnemyHealthBar;

	UPROPERTY(EditAnywhere)
	float HealthInterpolationSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HeartsText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* HolyWaterImage;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* PlayerHealthBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerLivesText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* ShotDoubleImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* ShotTripleImage;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* StageText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TimeText;

private:

	float CurrentEnemyHealth = 0.98f;

	float DesiredEnemyHealth = 1.0f;
	
	float CurrentPlayerHealth = 0.98f;

	float DesiredPlayerHealth = 1.0f;

#pragma region Control

public:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* AButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BButton;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* DownButton;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LeftButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RightButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* UpButton;

protected:

	UFUNCTION()
    void AButtonPressed();
	
	UFUNCTION()
	void BButtonPressed();

	UFUNCTION()
    void DownButtonPressed();

	UFUNCTION()
    void DownButtonReleased();
	
	UFUNCTION()
	void LeftButtonPressed();

	UFUNCTION()
    void LeftButtonReleased();

	UFUNCTION()
    void RightButtonPressed();

	UFUNCTION()
    void RightButtonReleased();

	UFUNCTION()
    void UpButtonPressed();

	UFUNCTION()
    void UpButtonReleased();
	
#pragma endregion
	
};

