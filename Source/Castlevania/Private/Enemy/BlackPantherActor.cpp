//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/26/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/BlackPantherActor.h"

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

void ABlackPantherActor::HitWithWeapon(const int32 Damage, const bool bPlaySound)
{
	// Disable movement.
	SetActorTickEnabled(false);
	State = EBlackPantherState::Burning;

	SpriteComponent->SetVisibility(false);
	FlipbookComponent->SetVisibility(true);
	
	Super::HitWithWeapon(Damage, bPlaySound);
}

void ABlackPantherActor::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
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
			FVector NewLocation = GetActorLocation();
			NewLocation.X += MovementSpeed * DeltaSeconds;
			SetActorLocation(NewLocation);

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

			FHitResult OutSweepHitResult;
			SetActorLocation(GetActorLocation() + JumpVelocity * DeltaSeconds, true, &OutSweepHitResult);
			if(OutSweepHitResult.bBlockingHit)
			{
				FVector Location = GetActorLocation();
				Location.Z = static_cast<float>(FMath::RoundToInt(Location.Z));
				SetActorLocation(Location);
				
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
		break;
	default:
		SetActorTickEnabled(false);
		break;
	}

	RoundFlipbookLocation();
}

void ABlackPantherActor::TimeStop(const bool bEnable)
{
	if(bEnable)
	{
		SetActorTickEnabled(false);
		FlipbookComponent->Stop();
	}
	else if(State != EBlackPantherState::Idle)
	{
		SetActorTickEnabled(true);
		FlipbookComponent->Play();
	}
}