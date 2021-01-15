//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/3/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Enemy/EnemyProjectileActor.h"

#include "Components/BoxComponent.h"
#include "CastlevaniaCameraActor.h"
#include "PaperSpriteComponent.h"
#include "WeaponActor.h"
#include "Kismet/GameplayStatics.h"

AEnemyProjectileActor::AEnemyProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SetRootComponent(SpriteComponent);
	SpriteComponent->SetCollisionProfileName("Enemy");

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetGenerateOverlapEvents(false);
	FlipbookComponent->SetLooping(false);
	FlipbookComponent->SetVisibility(false);
}

void AEnemyProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	MovementSpeed = InitialMovementSpeed * GetActorScale().X;

	SpriteComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectileActor::OnBeginOverlap);
	SpriteComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyProjectileActor::OnEndOverlap);
	
	SetActorTickEnabled(true);
}

void AEnemyProjectileActor::BurnOutWithFlipbook()
{
	UGameplayStatics::PlaySound2D(this, DestroySound);

	SetActorTickEnabled(false);

	SpriteComponent->SetVisibility(false);
	SpriteComponent->SetGenerateOverlapEvents(false);

	FlipbookComponent->SetVisibility(true);
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AEnemyProjectileActor::OnFinishedPlaying);
	FlipbookComponent->PlayFromStart();
}

void AEnemyProjectileActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACastlevaniaPawn* Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	UBoxComponent* PawnBoxComponent = Cast<UBoxComponent>(OtherComp);
	if(IsValid(Pawn) && IsValid(PawnBoxComponent))
	{
		if(PawnBoxComponent->ComponentHasTag(FName(TEXT("Whip"))))
		{
			BurnOutWithFlipbook();	
		}
		else if(PawnBoxComponent->ComponentHasTag(FName(TEXT("Character"))))
		{
			Pawn->TakeDamage(DamageToPlayer, FDamageEvent(), nullptr, this);			
		}
	}

	AWeaponActor* Weapon = Cast<AWeaponActor>(OtherActor);
	if(IsValid(Weapon))
	{
		BurnOutWithFlipbook();
	}
}

void AEnemyProjectileActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACastlevaniaCameraActor* Camera = Cast<ACastlevaniaCameraActor>(OtherActor);
	if(IsValid(Camera))
	{
		Destroy();
	}
}

void AEnemyProjectileActor::OnFinishedPlaying()
{
	Destroy();
}

void AEnemyProjectileActor::Tick(const float DeltaSeconds)
{
	const FVector NewLocation = GetActorLocation() + FVector(MovementSpeed * DeltaSeconds, 0.0f, 0.0f);
	SetActorLocation(NewLocation);
}
