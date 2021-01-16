//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CastlevaniaPawnMovementComponent.h"
#include "CastlevaniaStatics.h"
#include "CastlevaniaPawn.generated.h"

class ACastlevaniaGameModeBase;
class AHitEffectActor;
class AWeaponActor;

class UBoxComponent;
class UCastlevaniaGameInstance;
class UCastlevaniaUserWidget;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class UUserWidget;

USTRUCT(BlueprintType)
struct FFlipbookWithOffSetAndExtent
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Extent = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* Flipbook = nullptr;

	UPROPERTY(EditAnywhere)
	FVector Offset = FVector(0.0f, 0.0f, 0.0f);
	
};

UCLASS(HideCategories = (Tick, Replication, Rendering, Actor, Input, LOD, Cooking))
class CASTLEVANIA_API ACastlevaniaPawn : public APawn
{

	GENERATED_BODY()

public:

	ACastlevaniaPawn();

	virtual UPawnMovementComponent* GetMovementComponent() const override { return MovementComponent; }

	void InstantDeath();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION(CallInEditor)
	void TestDeath() { TakeDamage(16, FDamageEvent(), nullptr, nullptr); }
	
	virtual void Tick(float DeltaSeconds) override;

#pragma region Animation

public:

	void EndAttacking();
	
	bool GetFacingRightDirection() const;
	
	void SetFacingDirection(float Forward) const;
	
protected:

	UFUNCTION()
    void OnFlipbookFinishedPlaying();

	void SetFlipbook(const FFlipbookWithOffSetAndExtent& FlipbookWithOffsetAndExtent);
	
	void UpdateAnimationStateMachine();

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent CrouchFlipbook;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent DeathFlipbook;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent HurtFlipbook;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent IdleFlipbook;
	
	// Velocity threshold for showing the crouch while jumping. Must be <= this to show crouch.
	UPROPERTY(EditAnywhere, Category = "CastlevaniaPawn | Animation")
	float JumpVelocityForCrouchFrame = 125.0f;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent StairsDownFlipbook;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent StairsInBetweenFlipbook;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent StairsUpFlipbook;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent WalkFlipbook;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent WhipCrouchFlipbook;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent WhipStandFlipbook;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent WhipStairsDownFlipbook;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Animation")
	FFlipbookWithOffSetAndExtent WhipStairsUpFlipbook;
	
#pragma endregion

#pragma region Audio

public:

	void PlayFellSound() const;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Audio")
	USoundBase* FellSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Audio")
	USoundBase* HurtSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Audio")
	USoundBase* WhipSwingSound;
	
#pragma endregion

#pragma region Clock

protected:

	void ActivateClock();

	void OnClockDurationTick();

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	USoundBase* ClockSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	int32 ClockTicks = 3;

	int32 ClockTicksLeft = 0;
	
	FTimerHandle ClockTimerHandle;
	
	UPROPERTY(VisibleInstanceOnly, Category = "CastlevaniaPawn | Weapon")
	bool bIsClockActive = false;
	
#pragma endregion
	
#pragma region Components

public:
	
	// Movement component that allows moving the character in the level.
	UPROPERTY(VisibleAnywhere)
	UCastlevaniaPawnMovementComponent* MovementComponent;

protected:

	// Character collision.
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CharacterBoxComponent;
	
	// Sprite flip book for displaying character visuals.
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* CharacterFlipbookComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* OffsetComponent;
	
	// Collision for the whip.
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WhipBoxComponent;
	
	// Sprite flip book for displaying the whip.
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* WhipFlipbookComponent;
	
#pragma endregion

#pragma region Control

public:

    void Attack();

	void Down(bool bPressed);
	
    void Jump();

	void Left(bool bPressed);

	void Right(bool bPressed);

	void Up(bool bPressed);
	
	void SetCanMoveToStairs(bool bAllow, bool bDownNotUp = false, bool bAscendsFromLeftToRight = true, FVector LeftWorldLocation = FVector::ZeroVector, FVector RightWorldLocation = FVector::ZeroVector);

	void SetForceMove(bool bEnableNotDisable, bool bRightNotLeft) const;
	
protected:

	void MoveUp(float Input);

	void MoveRight(float Input);

	void UpdateControlFromButtons();
	
	bool bAttackThrowsWeapon = false;
	
	bool bIsHoldingDown = false;

	bool bIsHoldingLeft = false;
	
	bool bIsHoldingRight = false;
	
	bool bIsHoldingUp = false;

	bool bIsWhipping = false;
	
#pragma endregion

#pragma region Material

public:

	void SetInvincibleTimer(float Duration);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Material")
	float InitialInvincibilityTime = 1.5f;
	
protected:

	void StartWhipUpgradeFlash();

	void UpdateMaterial(float DeltaSeconds);

	int32 CurrentFlashFrame = 0;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "CastlevaniaPawn | Material")
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Material")
	FName FlashFrameName = FName(TEXT("FlashFrame"));
	
	float FlashFrameTime = 0.0f;
	
	float FlashTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Material")
	float InitialFlashTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Material")
	float InitialFlashFrameTime = 1.0f/60;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Material")
	float InvincibilityOpacityMultiplier = 0.5f;

	float InvincibilityTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Material")
	int32 MaxFlashFrame = 4;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Material")
	FName OpacityMultiplierName = FName(TEXT("OpacityMultiplier"));
	
#pragma endregion

#pragma region Reference

protected:

	UPROPERTY(Transient)
	UCastlevaniaGameInstance* GameInstance;
	
	UPROPERTY(Transient)
	ACastlevaniaGameModeBase* GameMode;
	
#pragma endregion
	
#pragma region Weapon

public:

	void AddMaximumWeaponInstance();
	
	void SetWeapon(EWeaponType Type);
	
protected:
	
	bool ConsumeHeartsToUseWeapon() const;

	bool CheckWeaponInstances();
	
	void SpawnWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	TSubclassOf<AWeaponActor> AxeClass;

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	FVector AxeSpawnOffset = FVector(15.0f, 0.0f, 7.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	TSubclassOf<AWeaponActor> BoomerangClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	FVector BoomerangSpawnOffset = FVector(22.0f, 0.0f, 1.0f);
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	TSubclassOf<AWeaponActor> DaggerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	FVector DaggerSpawnOffset = FVector(22.0f, 0.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	TSubclassOf<AWeaponActor> HolyWaterClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	FVector HolyWaterSpawnOffset = FVector(22.0f, 0.0f, 1.0f);

	int32 MaximumWeaponInstances = 1;
	
	UPROPERTY(Transient, VisibleInstanceOnly)
	TArray<AWeaponActor*> SpawnedWeapons;
	
	bool bWeaponSpawned = false;

	UPROPERTY(VisibleDefaultsOnly, Category = "CastlevaniaPawn | Weapon")
	int32 WeaponSpawnFrame = 2;
	
	UPROPERTY(VisibleInstanceOnly, Category = "CastlevaniaPawn | Weapon")	
	EWeaponType WeaponType = EWeaponType::None;
	
#pragma endregion
	
#pragma region Whip

public:

	int32 GetWhipDamage() const;
	
	FORCEINLINE EWhipType GetWhipType() const { return WhipType; }
	
	void SetWhipType(EWhipType Type);
	
	void UpgradeWhip();
	
protected:
	
	UFUNCTION()
    void OnWhipBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void UpdateWhipFlipbookAndCollision() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector CrouchWhipFlipbookRelativeLocation = FVector(24.0f,0.01f,-7.0f);

	UPROPERTY(EditAnywhere, Category = "CastlevaniaPawn | Whip")
	UPaperFlipbook* FlipbookLeatherWhip;

	UPROPERTY(EditAnywhere, Category = "CastlevaniaPawn | Whip")
	UPaperFlipbook* FlipbookLongWhip;
	
	UPROPERTY(EditAnywhere, Category = "CastlevaniaPawn | Whip")
	UPaperFlipbook* FlipbookShortWhip;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	TSubclassOf<AHitEffectActor> HitEffectActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	int32 LeatherWhipActivateCollisionFrame = 2;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector LeatherWhipBoxExtent = FVector(11.0f, 10.0f, 4.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector LeatherWhipBoxRelativeLocation = FVector(1.0f,0.0f,2.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	int32 LongWhipActivateCollisionFrame = 16;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector LongWhipBoxExtent = FVector(19.5f,10.0f,3.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector LongWhipBoxRelativeLocation = FVector(8.5f,0.0f,1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	int32 ShortWhipActivateCollisionFrame = 2;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector ShortWhipBoxExtent = FVector(11.5f,10.0f,3.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector ShortWhipBoxRelativeLocation = FVector(0.5f,0.0f,0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "CastlevaniaPawn | Whip")
	FVector StandWeaponFlipbookRelativeLocation = FVector(25.0f,0.01f,-1.0f);

	UPROPERTY(VisibleInstanceOnly, Category = "CastlevaniaPawn | Whip")
	EWhipType WhipType = EWhipType::Leather;
	
#pragma endregion
	
};




