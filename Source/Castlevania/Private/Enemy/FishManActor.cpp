//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/2/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/FishManActor.h"

#include "CastlevaniaFunctionLibrary.h"
#include "CastlevaniaGameModeBase.h"
#include "CastlevaniaPawn.h"
#include "FireBallActor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AFishManActor::AFishManActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AFishManActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		GameMode = Cast<ACastlevaniaGameModeBase>(World->GetAuthGameMode());
		if(IsValid(GameMode))
		{
			Pawn = GameMode->GetPlayerPawn();
			if(IsValid(Pawn))
			{
				const bool bOnRight = GetActorLocation().X > Pawn->GetActorLocation().X; 
				if(bOnRight)
				{
					FlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
				}
				else
				{
					FlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
				}
			}
		}
	}
	
	Velocity = FVector(0.0f, 0.0f, InitialVelocityZ);

	WalkingFlipbook = FlipbookComponent->GetFlipbook();
	
	SetActorTickEnabled(true);
}

void AFishManActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
{
	SetActorTickEnabled(false);

	Super::HitWithWeapon(Damage, bPlaySound, WeaponLocation);
}

void AFishManActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	switch(State)
	{
	case EFishManState::JumpingOutOfWater:
		{
			Velocity.Z += GravityAcceleration * DeltaSeconds;

			LocationFloat += Velocity * DeltaSeconds;
			const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
			if(!GetActorLocation().Equals(LocationInteger, 0.99f))
			{
				SetActorLocation(LocationInteger);	
			}

			if(Velocity.Z < 0.0f)
			{
				State = EFishManState::FallingToLand;

				BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			}
		}
		break;
	case EFishManState::FallingToLand:
		{
			Velocity.Z += GravityAcceleration * DeltaSeconds;

			LocationFloat += Velocity * DeltaSeconds;
			const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
			if(!GetActorLocation().Equals(LocationInteger, 0.99f))
			{
				FHitResult OutSweepHitResult;
				if(!SetActorLocation(LocationInteger, true, &OutSweepHitResult))
				{
					LocationFloat = GetActorLocation();
					
					State = EFishManState::Walking;

					UGameplayStatics::PlaySound2D(this, LandSound);

					if(IsValid(Pawn))
					{
						bool bOnRight = GetActorLocation().X > Pawn->GetActorLocation().X; 
						if(bOnRight)
						{
							FlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
						}
						else
						{
							FlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
						}
					}
				
					Velocity.Z = 0.0f;
					Velocity.X = WalkSpeed * FlipbookComponent->GetComponentScale().X;
				
					AttackTime = FMath::FRandRange(MinimumAttackTime, MaximumAttackTime);
				}
			}
		}
		break;
	case EFishManState::Walking:
		{
			LocationFloat.X += Velocity.X * DeltaSeconds;
			const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
			if(!GetActorLocation().Equals(LocationInteger, 0.99f))
			{
				SetActorLocation(LocationInteger);
			}
			
			RaycastAccumulator += DeltaSeconds;
			if(RaycastAccumulator >= RaycastInterval)
			{
				RaycastAccumulator = 0.0f;
				
				UWorld* World = GetWorld();
				if(IsValid(World))
				{
					FHitResult OutHit;
					
					FCollisionObjectQueryParams ObjectQueryParams;
					ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
					
					FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, -BoxComponent->GetScaledBoxExtent().Z);
					FVector End = Start + FVector(0.0f, 0.0f, -8.0f);
					
					if(!World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams))
					{
						State = EFishManState::JumpingIntoWater;
						BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
						break;
					}
				}
			}
			
			Accumulator += DeltaSeconds;
			if(Accumulator >= AttackTime)
			{
				Accumulator = 0.0f;
				State = EFishManState::Attacking;
			}
		}  
		break;
	case EFishManState::Attacking:
		{
			FlipbookComponent->SetFlipbook(AttackingFlipbook);

			UWorld* World = GetWorld();
			if(IsValid(World))
			{
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = this;
				SpawnParameters.ObjectFlags = RF_Transient;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FTransform Transform = FlipbookComponent->GetComponentTransform();
				FVector Location = Transform.GetLocation();
				if(Transform.GetScale3D().X == 1.0f)
				{
					Location += FireballSpawnOffset;
					Transform.SetLocation(Location);
				}
				else
				{
					Location += FVector(FireballSpawnOffset.X * -1.0f, FireballSpawnOffset.Y, FireballSpawnOffset.Z);
					Transform.SetLocation(Location);
				}
				
				World->SpawnActor<AFireBallActor>(FireBallClass, Transform, SpawnParameters);
			}

			State = EFishManState::RecoveringFromAttack;
		}
		break;
	case EFishManState::RecoveringFromAttack:
		Accumulator += DeltaSeconds;
		if(Accumulator >= AttackRecoveryTime)
		{
			Accumulator = 0.0f;
			State = EFishManState::Walking;
			FlipbookComponent->SetFlipbook(WalkingFlipbook);
		}
		break;
	case EFishManState::JumpingIntoWater:
		{
			Velocity.Z += GravityAcceleration * DeltaSeconds;

			LocationFloat += Velocity * DeltaSeconds;
			const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
			if(!GetActorLocation().Equals(LocationInteger, 0.99f))
			{
				SetActorLocation(LocationInteger);	
			}
		}
		break;
	}
}

void AFishManActor::TimeStop(const bool bEnable)
{
	if(bEnable)
	{
		SetActorTickEnabled(false);
		FlipbookComponent->Stop();
	}
	else
	{
		SetActorTickEnabled(true);
		FlipbookComponent->Play();
	}
}
