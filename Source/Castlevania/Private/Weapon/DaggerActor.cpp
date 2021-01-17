//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/24/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Weapon/DaggerActor.h"

#include "CastlevaniaCameraActor.h"
#include "CastlevaniaFunctionLibrary.h"
#include "EnemyActor.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"

ADaggerActor::ADaggerActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SetRootComponent(SpriteComponent);
	SpriteComponent->SetCollisionProfileName("Weapon");
}

void ADaggerActor::BeginPlay()
{
	Super::BeginPlay();

	SpriteComponent->OnComponentEndOverlap.AddDynamic(this, &ADaggerActor::OnEndOverlap);

	const FVector Scale = GetActorScale();
	InterpolationSpeed = Scale.X > 0? TravelSpeed : TravelSpeed * -1;
	SetActorTickEnabled(true);

	UGameplayStatics::PlaySound2D(this, SpawnSound);

	FloatLocation = GetActorLocation();
}

void ADaggerActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		Destroy();
	}
}

void ADaggerActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FloatLocation.X += DeltaSeconds * InterpolationSpeed;
	IntegerLocation = UCastlevaniaFunctionLibrary::RoundVectorToInt(FloatLocation);
	
	if(!GetActorLocation().Equals(IntegerLocation, 0.99f))
	{
		SetActorLocation(IntegerLocation);	
	}
}


