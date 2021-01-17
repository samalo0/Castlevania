//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/28/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Weapon/HolyWaterActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaCameraActor.h"
#include "CastlevaniaFunctionLibrary.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"

AHolyWaterActor::AHolyWaterActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName("Weapon");
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetLooping(false);

	WeaponDamage = 2;
}

void AHolyWaterActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AHolyWaterActor::OnEndOverlap);

	Velocity = InitialVelocity;
	
	const FVector Scale = GetActorScale();
	if(Scale.X < 0)
	{
		Velocity.X *= -1.0f;
	}

	SetActorTickEnabled(true);
}

void AHolyWaterActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		Destroy();
	}
}

void AHolyWaterActor::OnFinishedPlaying()
{
	Destroy();
}

void AHolyWaterActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Velocity.Z += GravityAcceleration * DeltaSeconds;

	LocationFloat += Velocity * DeltaSeconds;
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);

	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		FHitResult OutSweepHitResult;
		if(!SetActorLocation(LocationInteger, true, &OutSweepHitResult))
		{
			BoxComponent->SetBoxExtent(FVector(8.0f, 10.0f, 8.0f));

			SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 4.0f));
		
			FlipbookComponent->SetFlipbook(BurnFlipbook);
			FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AHolyWaterActor::OnFinishedPlaying);
			FlipbookComponent->PlayFromStart();
			SetActorTickEnabled(false);

			UGameplayStatics::PlaySound2D(this, BurnSound);
		}	
	}
}
