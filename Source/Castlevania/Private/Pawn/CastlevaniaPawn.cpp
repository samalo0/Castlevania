//----------------------------------------------------------------------------------------------------------------------
// Copyright 2019 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/22/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Pawn/CastlevaniaPawn.h"


#include "CastlevaniaFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Core/CastlevaniaGameInstance.h"
#include "Core/CastlevaniaGameModeBase.h"
#include "Pawn/CastlevaniaUserWidget.h"
#include "Enemy/EnemyActor.h"
#include "Enemy/EnemyProjectileActor.h"
#include "Kismet/GameplayStatics.h"
#include "Effect/HitEffectActor.h"
#include "Components/InputComponent.h"
#include "PaperFlipbookComponent.h"
#include "Spawn/PowerupSpawnActor.h"
#include "Weapon/WeaponActor.h"

ACastlevaniaPawn::ACastlevaniaPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CharacterBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(CharacterBoxComponent);
	CharacterBoxComponent->SetCollisionProfileName("Pawn");
	CharacterBoxComponent->ComponentTags.Add(FName(TEXT("Character")));
	
	OffsetComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OffsetComponent"));
	OffsetComponent->SetupAttachment(GetRootComponent());
	
	CharacterFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbookComponent"));
	CharacterFlipbookComponent->SetupAttachment(OffsetComponent);
	CharacterFlipbookComponent->SetCollisionProfileName(TEXT("NoCollision"));
	CharacterFlipbookComponent->SetGenerateOverlapEvents(false);

	WhipFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WeaponFlipbookComponent"));
	WhipFlipbookComponent->SetupAttachment(CharacterFlipbookComponent);
	WhipFlipbookComponent->SetCollisionProfileName(TEXT("NoCollision"));
	WhipFlipbookComponent->SetLooping(false);
	WhipFlipbookComponent->SetVisibility(false);
	WhipFlipbookComponent->SetGenerateOverlapEvents(false);

	WhipBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxComponent"));
	WhipBoxComponent->SetupAttachment(WhipFlipbookComponent);
	WhipBoxComponent->SetCollisionProfileName("Weapon");
	WhipBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WhipBoxComponent->ComponentTags.Add(FName(TEXT("Whip")));
	
	MovementComponent = CreateDefaultSubobject<UCastlevaniaPawnMovementComponent>(TEXT("MovementComponent"));
}

void ACastlevaniaPawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		GameInstance = Cast<UCastlevaniaGameInstance>(World->GetGameInstance());
	}
	
	DynamicMaterial = CharacterFlipbookComponent->CreateDynamicMaterialInstance(0);
	
	CharacterFlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ACastlevaniaPawn::OnFlipbookFinishedPlaying);

	WhipBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACastlevaniaPawn::OnWhipBeginOverlap);
	
	UpdateWhipFlipbookAndCollision();
}

void ACastlevaniaPawn::InstantDeath()
{
	if(IsValid(GameInstance))
	{
		GameInstance->AddPlayerHealth(static_cast<int32>(-16));
		GameInstance->AddPlayerLives(-1);

		Destroy();
	}
}

float ACastlevaniaPawn::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{
	if(MovementComponent->bIsInvincible)
	{
		return 0.0f;
	}

	if(!IsValid(GameInstance))
	{
		return 0.0f;
	}

	GameInstance->AddPlayerHealth(static_cast<int32>(-1 * DamageAmount));

	// Cancel attacking.
	if(MovementComponent->bIsAttacking)
	{
		EndAttacking();
	}
	
	MovementComponent->Hurt(DamageCauser);
	UGameplayStatics::PlaySound2D(this, HurtSound);

	if(GameInstance->GetPlayerHealth() <= 0)
	{
		MovementComponent->bIsDead = true;
		if(IsValid(GameMode))
		{
			GameMode->PlayDeathMusic();
		}

		GameInstance->AddPlayerLives(-1);
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACastlevaniaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACastlevaniaPawn::Jump);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &ACastlevaniaPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACastlevaniaPawn::MoveUp);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACastlevaniaPawn::Attack);
}

void ACastlevaniaPawn::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateControlFromButtons();
	
	UpdateAnimationStateMachine();

	UpdateMaterial(DeltaSeconds);

	FVector RoundedLocation = OffsetComponent->GetComponentLocation();
	RoundedLocation.X = static_cast<float>(FMath::RoundToInt(RoundedLocation.X));
	RoundedLocation.Z = static_cast<float>(FMath::RoundToInt(RoundedLocation.Z));
	CharacterFlipbookComponent->SetWorldLocation(RoundedLocation);
}

void ACastlevaniaPawn::SetFacingDirection(const float Forward) const
{
	if(Forward > 0.f)
	{
		CharacterFlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
	else if(Forward < 0.0f)
	{
		CharacterFlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));	
	}
}

#pragma region Animation

void ACastlevaniaPawn::EndAttacking()
{
	MovementComponent->bIsAttacking = false;
	bIsWhipping = false;
	bWeaponSpawned = false;

	WhipFlipbookComponent->SetVisibility(false);
	WhipBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	CharacterFlipbookComponent->SetLooping(true);
	UpdateAnimationStateMachine();
	CharacterFlipbookComponent->PlayFromStart();
}

bool ACastlevaniaPawn::GetFacingRightDirection() const
{
	return CharacterFlipbookComponent->GetComponentScale().X == 1.0f;
}

void ACastlevaniaPawn::OnFlipbookFinishedPlaying()
{
	if(MovementComponent->bIsDead)
	{
		if(IsValid(GameMode))
		{
			GameMode->StartLevelTransition(false);
		}
		return;	
	}
	
	if(MovementComponent->bIsAttacking)
	{
		EndAttacking();
	}
}

void ACastlevaniaPawn::SetFlipbook(const FFlipbookWithOffSetAndExtent& FlipbookWithOffsetAndExtent)
{
	if(CharacterFlipbookComponent->GetFlipbook() == FlipbookWithOffsetAndExtent.Flipbook)
	{
		return;
	}
	
	CharacterFlipbookComponent->SetFlipbook(FlipbookWithOffsetAndExtent.Flipbook);

	const FVector CurrentExtent = CharacterBoxComponent->GetScaledBoxExtent();
	CharacterBoxComponent->SetBoxExtent(FlipbookWithOffsetAndExtent.Extent);

	SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, (FlipbookWithOffsetAndExtent.Extent.Z - CurrentExtent.Z)));
	
	OffsetComponent->SetRelativeLocation(FlipbookWithOffsetAndExtent.Offset);
}

void ACastlevaniaPawn::UpdateAnimationStateMachine()
{
	// While hurt, show the hurt flipbook as you fly through the air uncontrollably.
	if(MovementComponent->bIsHurt)
	{
		SetFlipbook(HurtFlipbook);
		return;
	}

	if(MovementComponent->bIsDead)
	{
		CharacterFlipbookComponent->SetLooping(false);
		SetFlipbook(DeathFlipbook);
		return;
	}
	
	// Crouch while recovering.
	if(MovementComponent->bIsRecovering && !MovementComponent->bIsOnStairs)
	{
		SetFlipbook(CrouchFlipbook);	
		return;
	}
	
	// Don't update movement flipbook while whipping
	if(MovementComponent->bIsAttacking)
	{
		if(!bIsWhipping && !bWeaponSpawned)
		{
			const int32 Frame = CharacterFlipbookComponent->GetPlaybackPositionInFrames();
			switch(WeaponType)
			{
			case EWeaponType::Axe:	
			case EWeaponType::Boomerang:
			case EWeaponType::Dagger:
			case EWeaponType::HolyWater:
				if(Frame >= WeaponSpawnFrame)
				{
					bWeaponSpawned = true;
					SpawnWeapon();	
				}
				break;
			default:
				break;
			}
			return;
		}
		
		if(bIsWhipping && WhipFlipbookComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
		{
			const int32 Frame = WhipFlipbookComponent->GetPlaybackPositionInFrames();
			switch(WhipType)
			{
			case EWhipType::Leather:
				if(Frame >= LeatherWhipActivateCollisionFrame)
				{
					WhipBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				}
				break;
			case EWhipType::Short:
				if(Frame >= ShortWhipActivateCollisionFrame)
				{
					WhipBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				}
				break;
			case EWhipType::Long:
				if(Frame >= LongWhipActivateCollisionFrame)
				{
					WhipBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				}
				break;
			}
		}	
		return;
	}

	if(MovementComponent->bIsOnStairs)
	{
		const int GridLocation = FMath::RoundToInt(GetActorLocation().X) % 8;
		if(GridLocation > 1 && GridLocation < 7)
		{
			SetFlipbook(StairsInBetweenFlipbook);
		}
		else
		{
			if(MovementComponent->bStairsAscendFromLeftToRight)
			{
				if(GetFacingRightDirection())
				{
					SetFlipbook(StairsUpFlipbook);
				}
				else
				{
					SetFlipbook(StairsDownFlipbook);
				}
			}
			else
			{
				if(GetFacingRightDirection())
				{
					SetFlipbook(StairsDownFlipbook);	
				}
				else
				{
					SetFlipbook(StairsUpFlipbook);
				}
			}
		}
		return;
	}
	
	const FVector Velocity = GetVelocity();

	if(MovementComponent->bIsOnGround)
	{
		if(FMath::IsNearlyEqual(Velocity.X, 0.0f))
		{
			if(MovementComponent->bIsCrouched)
			{
				SetFlipbook(CrouchFlipbook);	
			}
			else
			{
				SetFlipbook(IdleFlipbook);
			}	
		}
		else
		{
			SetFlipbook(WalkFlipbook);
		}
	}
	else
	{
		if(MovementComponent->bIsJumping && FMath::Abs(Velocity.Z) <= JumpVelocityForCrouchFrame)
		{
			SetFlipbook(CrouchFlipbook);			
		}
		else
		{
			SetFlipbook(IdleFlipbook);
		}
	}
}

#pragma endregion

#pragma region Audio

void ACastlevaniaPawn::PlayFellSound() const
{
	UGameplayStatics::PlaySound2D(this, FellSound);
}

#pragma endregion

#pragma region Clock

void ACastlevaniaPawn::ActivateClock()
{
	UWorld* World = GetWorld();
	if(IsValid(World) && IsValid(GameMode))
	{
		bIsClockActive = true;

		GameMode->PauseMusic(true);
		GameMode->SetClockActivated(true);
		
		ClockTicksLeft = ClockTicks;
		UGameplayStatics::PlaySound2D(this, ClockSound);
		World->GetTimerManager().SetTimer(ClockTimerHandle, this, &ACastlevaniaPawn::OnClockDurationTick, 1.0f, true);
	}
}

void ACastlevaniaPawn::OnClockDurationTick()
{
	ClockTicksLeft--;
	if(ClockTicksLeft > 0)
	{
		UGameplayStatics::PlaySound2D(this, ClockSound);
	}
	else
	{
		UWorld* World = GetWorld();
		if(IsValid(World) && IsValid(GameMode))
		{
			bIsClockActive = false;
			
			GameMode->PauseMusic(false);
			GameMode->SetClockActivated(false);

			World->GetTimerManager().ClearTimer(ClockTimerHandle);
		}	
	}
}

#pragma endregion

#pragma region Control

void ACastlevaniaPawn::Attack()
{
	if(!InputEnabled())
	{
		return;
	}
	
	// Block attacking or using a weapon if unable.
	if(MovementComponent->bIsAttacking || MovementComponent->bIsHurt || MovementComponent->bIsRecovering || MovementComponent->bIsDead)
	{
		return;
	}

	// Check for clock usage.
	if(bAttackThrowsWeapon && WeaponType == EWeaponType::Clock && !bIsClockActive && ConsumeHeartsToUseWeapon())
	{
		ActivateClock();
		return;
	}

	// Check for projectile weapon throw or whipping.
	MovementComponent->bIsAttacking = true;
	CharacterFlipbookComponent->SetLooping(false);

	if(bAttackThrowsWeapon && WeaponType != EWeaponType::None && CheckWeaponInstances() && ConsumeHeartsToUseWeapon())
	{
		if(MovementComponent->bIsOnStairs)
		{
			if(MovementComponent->bStairsAscendFromLeftToRight)
			{
				if(GetFacingRightDirection())
				{
					SetFlipbook(WhipStairsUpFlipbook);
				}
				else
				{
					SetFlipbook(WhipStairsDownFlipbook);
				}
			}
			else
			{
				if(GetFacingRightDirection())
				{
					SetFlipbook(WhipStairsDownFlipbook);	
				}
				else
				{
					SetFlipbook(WhipStairsUpFlipbook);
				}
			}
		}
		else
		{
			SetFlipbook(WhipStandFlipbook);
		}
	}
	else
	{
		bIsWhipping = true;

		UGameplayStatics::PlaySound2D(this, WhipSwingSound);
		
		if(MovementComponent->bIsCrouched)
		{
			SetFlipbook(WhipCrouchFlipbook);
			WhipFlipbookComponent->SetRelativeLocation(CrouchWhipFlipbookRelativeLocation);
		}
		else
		{
			if(MovementComponent->bIsOnStairs)
			{
				if(MovementComponent->bStairsAscendFromLeftToRight)
				{
					if(GetFacingRightDirection())
					{
						SetFlipbook(WhipStairsUpFlipbook);	
					}
					else
					{
						SetFlipbook(WhipStairsDownFlipbook);
					}
				}
				else
				{
					if(GetFacingRightDirection())
					{
						SetFlipbook(WhipStairsDownFlipbook);	
					}
					else
					{
						SetFlipbook(WhipStairsUpFlipbook);
					}
				}
			}
			else
			{
				SetFlipbook(WhipStandFlipbook);
				WhipFlipbookComponent->SetRelativeLocation(StandWeaponFlipbookRelativeLocation);
			}
		}

		WhipFlipbookComponent->SetVisibility(true);
		WhipFlipbookComponent->PlayFromStart();			
	}
}

void ACastlevaniaPawn::Down(const bool bPressed)
{
	bIsHoldingDown = bPressed;
}

void ACastlevaniaPawn::Jump()
{
	if(!InputEnabled())
	{
		return;
	}
	
	MovementComponent->Jump();
}

void ACastlevaniaPawn::Left(const bool bPressed)
{
	bIsHoldingLeft = bPressed;
}

void ACastlevaniaPawn::MoveUp(const float Input)
{
	if(!FMath::IsNearlyEqual(Input, 0.0f))
	{
		MovementComponent->AddInputVector(FVector(0.0f, 0.0f, Input));
	}

	bAttackThrowsWeapon = Input == 1.0f;
}

void ACastlevaniaPawn::MoveRight(const float Input)
{
	if(!FMath::IsNearlyEqual(Input, 0.0f))
	{
		MovementComponent->AddInputVector(FVector(Input, 0.0f, 0.0f));
	}
}

void ACastlevaniaPawn::Right(const bool bPressed)
{
	bIsHoldingRight = bPressed;
}

void ACastlevaniaPawn::SetCanMoveToStairs(const bool bAllow, const bool bDownNotUp, const bool bAscendsFromLeftToRight, const FVector LeftWorldLocation, const FVector RightWorldLocation)
{
	if(!bAllow)
	{
		MovementComponent->bCanMoveToAscendStairs = false;
		MovementComponent->bCanMoveToDescendStairs = false;
		return;
	}
	
	if(bDownNotUp)
	{
		MovementComponent->bCanMoveToDescendStairs = true;
		MovementComponent->bCanMoveToAscendStairs = false;
	}
	else
	{
		MovementComponent->bCanMoveToAscendStairs = true;
		MovementComponent->bCanMoveToDescendStairs = false;
	}

	MovementComponent->bStairsAscendFromLeftToRight = bAscendsFromLeftToRight;
	
	MovementComponent->StairsLeftWorldLocation = LeftWorldLocation;
	MovementComponent->StairsRightWorldLocation = RightWorldLocation;
}

void ACastlevaniaPawn::SetForceMove(const bool bEnableNotDisable, const bool bRightNotLeft) const
{
	MovementComponent->bForceMove = bEnableNotDisable;
	MovementComponent->bForceMoveRight = bRightNotLeft;
}

void ACastlevaniaPawn::Up(const bool bPressed)
{
	bIsHoldingUp = bPressed;
}

void ACastlevaniaPawn::UpdateControlFromButtons()
{
	if(!InputEnabled())
	{
		return;
	}
	
	if(bIsHoldingDown)
	{
		MoveUp(-1.0f);
	}

	if(bIsHoldingLeft)
	{
		MoveRight(-1.0f);
	}

	if(bIsHoldingRight)
	{
		MoveRight(1.0f);
	}
	
	if(bIsHoldingUp)
	{
		MoveUp(1.0f);
	}
}

#pragma endregion

#pragma region Material

void ACastlevaniaPawn::SetInvincibleTimer(const float Duration)
{
	InvincibilityTime = Duration;
	DynamicMaterial->SetScalarParameterValue(OpacityMultiplierName, InvincibilityOpacityMultiplier);
}

void ACastlevaniaPawn::StartWhipUpgradeFlash()
{
	CurrentFlashFrame = 1;
	FlashTime = InitialFlashTime;
	FlashFrameTime = InitialFlashFrameTime;
	DynamicMaterial->SetScalarParameterValue(FlashFrameName, static_cast<float>(CurrentFlashFrame));	
}

void ACastlevaniaPawn::UpdateMaterial(const float DeltaSeconds)
{
	if(!IsValid(DynamicMaterial))
	{
		return;
	}

	if(FlashTime > 0.0f)
	{
		FlashTime -= DeltaSeconds;
		FlashFrameTime -= DeltaSeconds;
	
		if(FlashTime <= 0.0f)
		{
			FlashTime = 0.0f;
			DynamicMaterial->SetScalarParameterValue(FlashFrameName, 0.0f);
			return;
		}

		if(FlashFrameTime <= 0.0f)
		{
			FlashFrameTime = InitialFlashFrameTime;

			CurrentFlashFrame++;
			if(CurrentFlashFrame > MaxFlashFrame)
			{
				CurrentFlashFrame = 0;
			}

			DynamicMaterial->SetScalarParameterValue(FlashFrameName, static_cast<float>(CurrentFlashFrame));
		}		
	}

	if(InvincibilityTime > 0.0f)
	{
		InvincibilityTime -= DeltaSeconds;
		if(InvincibilityTime <= 0.0f)
		{
			InvincibilityTime = 0.0f;
			DynamicMaterial->SetScalarParameterValue(OpacityMultiplierName, 1.0f);
			MovementComponent->bIsInvincible = false;

			// Check for anything overlapping that does damage when invincibility ends.
			TArray<AActor*> OverlappingActors;
			CharacterBoxComponent->GetOverlappingActors(OverlappingActors);
			for(AActor* Actor : OverlappingActors)
			{
				AEnemyActor* Enemy = Cast<AEnemyActor>(Actor);
				if(IsValid(Enemy))
				{
					TakeDamage(Enemy->GetDamageToPlayer(), FDamageEvent(), nullptr, Enemy);
					break;
				}

				AEnemyProjectileActor* Projectile = Cast<AEnemyProjectileActor>(Actor);
				if(IsValid(Projectile))
				{
					TakeDamage(Projectile->GetDamageToPlayer(), FDamageEvent(), nullptr, Enemy);
					break;
				}
			}
		}
	}
}

#pragma endregion

#pragma region Weapon

bool ACastlevaniaPawn::ConsumeHeartsToUseWeapon() const
{
	if(!IsValid(GameInstance))
	{
		return false;
	}

	const int32 Hearts = GameInstance->GetHearts();
	
	switch(WeaponType)
	{
	case EWeaponType::Axe:
	case EWeaponType::Boomerang:
	case EWeaponType::Dagger:
	case EWeaponType::HolyWater:
		if(Hearts >= 1)
		{
			GameInstance->AddHearts(-1);
			return true;
		}
		break;
	case EWeaponType::Clock:
		if(Hearts >= 5)
		{
			GameInstance->AddHearts(-5);
			return true;
		}
		break;
	default:
		return false;
	}

	return false;
}

bool ACastlevaniaPawn::CheckWeaponInstances()
{
	// Remove old instances.
	for(int32 Index = SpawnedWeapons.Num() - 1; Index >= 0; Index--)
	{
		if(!IsValid(SpawnedWeapons[Index]))
		{
			SpawnedWeapons.RemoveAt(Index, 1);
		}
	}

	return SpawnedWeapons.Num() < MaximumWeaponInstances;
}

void ACastlevaniaPawn::AddMaximumWeaponInstance()
{
	MaximumWeaponInstances++;
	if(MaximumWeaponInstances > 3)
	{
		MaximumWeaponInstances = 3;
	}
}

void ACastlevaniaPawn::SetWeapon(const EWeaponType Type)
{
	WeaponType = Type;

	if(MaximumWeaponInstances > 1)
	{
		MaximumWeaponInstances = 1;
		
		if(IsValid(GameInstance))
		{
			GameInstance->SetShot(1);
		}
	}
}

void ACastlevaniaPawn::SpawnWeapon()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	UClass* Class = nullptr;

	FTransform Transform = CharacterFlipbookComponent->GetComponentTransform();
	FVector Offset = FVector::ZeroVector;
	
	switch(WeaponType)
	{
	case EWeaponType::Axe:
		Class = AxeClass;
		Offset = AxeSpawnOffset;
		break;
	case EWeaponType::Boomerang:
		Class = BoomerangClass;
		Offset = BoomerangSpawnOffset;
		break;
	case EWeaponType::Dagger:
		Class = DaggerClass;
		Offset = DaggerSpawnOffset;
		break;
	case EWeaponType::HolyWater:
		Class = HolyWaterClass;
		Offset = HolyWaterSpawnOffset;
		break;
	default:
		break;
	}

	if(!IsValid(Class))
	{
		return;
	}

	if(Transform.GetScale3D().X == -1.0f)
	{
		Offset *= FVector(-1.0f, 1.0f, 1.0f);
	}

	Transform.SetLocation(Transform.GetLocation() + Offset);
	
	// Spawn weapon.
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = this;
	SpawnParameters.ObjectFlags = RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeaponActor* NewWeapon = World->SpawnActor<AWeaponActor>(Class, Transform, SpawnParameters);
	if(IsValid(NewWeapon))
	{
		SpawnedWeapons.Add(NewWeapon);
	}
}

#pragma endregion

#pragma region Whip

int32 ACastlevaniaPawn::GetWhipDamage() const
{
	if(WhipType == EWhipType::Leather)
	{
		return 1;
	}

	return 2;
}

void ACastlevaniaPawn::OnWhipBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyActor* Enemy = Cast<AEnemyActor>(OtherActor);
	APowerupSpawnActor* Spawner = Cast<APowerupSpawnActor>(OtherActor);
	AEnemyProjectileActor* Projectile = Cast<AEnemyProjectileActor>(OtherActor);
	if(IsValid(Enemy) || IsValid(Spawner) || IsValid(Projectile))
	{
		const FVector AverageLocation = (WhipBoxComponent->GetComponentLocation() + OtherActor->GetActorLocation()) / 2.0f;
		const FVector SpawnLocation = UCastlevaniaFunctionLibrary::RoundVectorToInt(FVector(AverageLocation.X, WhipBoxComponent->GetComponentLocation().Y, AverageLocation.Z));

		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = this;
			SpawnParameters.Owner = this;
			SpawnParameters.ObjectFlags = RF_Transient;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			World->SpawnActor<AHitEffectActor>(HitEffectActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
		}
	}
}

void ACastlevaniaPawn::SetWhipType(const EWhipType Type)
{
	WhipType = Type;
	UpdateWhipFlipbookAndCollision();
}

void ACastlevaniaPawn::UpdateWhipFlipbookAndCollision() const
{
	const float PlaybackPosition = WhipFlipbookComponent->GetPlaybackPosition();
	
	switch(WhipType)
	{
	case EWhipType::Leather:
		WhipFlipbookComponent->SetFlipbook(FlipbookLeatherWhip);
		WhipBoxComponent->SetBoxExtent(LeatherWhipBoxExtent);
		WhipBoxComponent->SetRelativeLocation(LeatherWhipBoxRelativeLocation);
		break;
	case EWhipType::Long:
		WhipFlipbookComponent->SetFlipbook(FlipbookLongWhip);
		WhipBoxComponent->SetBoxExtent(LongWhipBoxExtent);
		WhipBoxComponent->SetRelativeLocation(LongWhipBoxRelativeLocation);
		break;
	case EWhipType::Short:
		WhipFlipbookComponent->SetFlipbook(FlipbookShortWhip);
		WhipBoxComponent->SetBoxExtent(ShortWhipBoxExtent);
		WhipBoxComponent->SetRelativeLocation(ShortWhipBoxRelativeLocation);
		break;
	}

	WhipFlipbookComponent->SetPlaybackPosition(PlaybackPosition, false);
}

void ACastlevaniaPawn::UpgradeWhip()
{
	switch(WhipType)
	{
	case EWhipType::Leather:
		WhipType = EWhipType::Short;
		break;
	case EWhipType::Short:
		WhipType = EWhipType::Long;
		break;
	default:
		// Fully upgraded.
		break;
	}

	UpdateWhipFlipbookAndCollision();

	StartWhipUpgradeFlash();
}

#pragma endregion



