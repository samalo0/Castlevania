//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/1/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Effect/SplashEffectActor.h"

#include "CastlevaniaCameraActor.h"
#include "CastlevaniaFunctionLibrary.h"
#include "PaperSpriteComponent.h"

ASplashEffectActor::ASplashEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SetRootComponent(SpriteComponent);
	SpriteComponent->SetCollisionProfileName("Interactable");
}

void ASplashEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SpriteComponent->OnComponentEndOverlap.AddDynamic(this, &ASplashEffectActor::OnEndOverlap);
	
	const float X = FMath::FRandRange(-InitialVelocityX, InitialVelocityX);
	const float Z = FMath::FRandRange(InitialVelocityZ / 2.0f, InitialVelocityZ);
	Velocity = FVector(X, 0.0f, Z);

	LocationFloat = GetActorLocation();
	
	SetActorTickEnabled(true);
}

void ASplashEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		Destroy();
	}
}

void ASplashEffectActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Velocity.Z += GravityAcceleration * DeltaSeconds;

	LocationFloat += Velocity * DeltaSeconds;
	const FVector LocationInteger = UCastlevaniaFunctionLibrary::RoundVectorToInt(LocationFloat);
	if(!GetActorLocation().Equals(LocationInteger, 0.99f))
	{
		SetActorLocation(LocationInteger);	
	}
}

