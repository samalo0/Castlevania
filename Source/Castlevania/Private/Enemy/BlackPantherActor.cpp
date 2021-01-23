//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/BlackPantherActor.h"

#include "CastlevaniaFunctionLibrary.h"
#include "CastlevaniaPawn.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"

ABlackPantherActor::ABlackPantherActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetCollisionProfileName("NoCollision");
	SpriteComponent->SetupAttachment(GetRootComponent());
	SpriteComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxComponent"));
	TriggerBoxComponent->SetupAttachment(GetRootComponent());
	TriggerBoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
	TriggerBoxComponent->SetRelativeLocation(FVector(-48.0f, 0.0f, 0.0f));
	TriggerBoxComponent->SetBoxExtent(FVector(16.0f, 10.0f, 150.0f));
	
	FlipbookComponent->SetVisibility(false);
	FlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));

	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

void ABlackPantherActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackPantherActor::OnTriggerBoxBeginOverlap);
}

void ABlackPantherActor::HitWithWeapon(const int32 Damage, const bool bPlaySound, const FVector WeaponLocation)
{
	// Disable movement.
	State = EBlackPantherState::Burning;
	SetActorTickEnabled(false);

	SpriteComponent->SetVisibility(false);
	FlipbookComponent->SetVisibility(true);
	
	Super::HitWithWeapon(Damage, bPlaySound, WeaponLocation);
}

void ABlackPantherActor::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn) && !bIsTimeStopped)
	{
		State = EBlackPantherState::Run;

		SpriteComponent->SetVisibility(false);
		
		FlipbookComponent->SetVisibility(true);
		MovementSpeed = InitialMovementSpeed * FlipbookComponent->GetComponentScale().X;

		TriggerBoxComponent->DestroyComponent();
		
		SetActorTickEnabled(true);
	}
}

void ABlackPantherActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch(State)
	{
	case EBlackPantherState::Run:
		{
			LocationFloat.X += MovementSpeed * DeltaSeconds;
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
					const FVector Start = GetActorLocation() + FVector(BoxComponent->GetScaledBoxExtent().X * FlipbookComponent->GetComponentScale().X, 0.0f, -BoxComponent->GetScaledBoxExtent().Z);
					const FVector End = Start + FVector(0.0f, 0.0f, -8.0f);

					if(!World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams))
					{
						State = EBlackPantherState::Jump;

						FlipbookComponent->SetVisibility(false);

						SpriteComponent->SetSprite(JumpSprite);
						SpriteComponent->SetVisibility(true);

						JumpVelocity = InitialJumpVelocity * FlipbookComponent->GetComponentScale();
					}
				}
			}	
		}
		break;
	case EBlackPantherState::Jump:
		{
			JumpVelocity.Z += GravityAcceleration * DeltaSeconds;
			LocationFloat += JumpVelocity * DeltaSeconds;
			const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
			if(!GetActorLocation().Equals(LocationInteger, 0.99f))
			{
				FHitResult OutSweepHitResult;
				SetActorLocation(LocationInteger, true, &OutSweepHitResult);
				if(OutSweepHitResult.bBlockingHit)
				{
					LocationFloat = GetActorLocation();
				
					// Set to running.
					State = EBlackPantherState::Run;

					// Flip around.
					FVector NewScale = FlipbookComponent->GetComponentScale();
					NewScale.X *= -1;
					FlipbookComponent->SetWorldScale3D(NewScale);
					SpriteComponent->SetWorldScale3D(NewScale);

					// Setup run speed/direction.
					MovementSpeed = InitialMovementSpeed * FlipbookComponent->GetComponentScale().X;
			
					// Show the flipbook and hide the sprite.
					FlipbookComponent->SetVisibility(true);

					SpriteComponent->SetVisibility(false);
				}	
			}	
		}
		break;
	default:
		SetActorTickEnabled(false);
		break;
	}
}

void ABlackPantherActor::TimeStop(const bool bEnable)
{
	Super::TimeStop(bEnable);

	if(State == EBlackPantherState::Burning || State == EBlackPantherState::Idle)
	{
		return;
	}
	
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
