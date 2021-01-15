//----------------------------------------------------------------------------------------------------------------------
// Copyright 2020 Stephen Maloney
// Owner: Stephen Maloney
// Created: 12/25/2020
//----------------------------------------------------------------------------------------------------------------------

#include "Volume/EnterCastleVolume.h"

#include "Components/BrushComponent.h"
#include "Core/CastlevaniaCameraActor.h"
#include "CastlevaniaPawn.h"
#include "Kismet/GameplayStatics.h"

AEnterCastleVolume::AEnterCastleVolume()
{
	GetBrushComponent()->SetCollisionProfileName("OverlapOnlyPawn");
}

void AEnterCastleVolume::BeginPlay()
{
	Super::BeginPlay();

	GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnterCastleVolume::OnBeginOverlap);
	GetBrushComponent()->OnComponentEndOverlap.AddDynamic(this, &AEnterCastleVolume::OnEndOverlap);
}

void AEnterCastleVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn))
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			APlayerController* Controller = World->GetFirstPlayerController();
			if(IsValid(Controller))
			{
				Pawn->DisableInput(Controller);	
				Pawn->SetForceMoveRight(true);
				UGameplayStatics::PlaySound2D(this, EnterSound);
			}
		}
	}
}

void AEnterCastleVolume::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	UWorld* World = GetWorld();
	if(IsValid(Pawn) && IsValid(World))
	{
		APlayerController* Controller = World->GetFirstPlayerController();

		if(IsValid(Controller))
		{
			// Fade out camera
			Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0f, 1.0f), CameraFadeTime, false, true);

			// Delay, then fade in at new location.
			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, this, &AEnterCastleVolume::OnEndOverlapDelayed, SwitchDelay);
		}
	}
}

void AEnterCastleVolume::OnEndOverlapDelayed()
{
	UWorld * World = GetWorld();
	if(IsValid(World) && IsValid(Pawn))
	{
		APlayerController* Controller = World->GetFirstPlayerController();

		if(IsValid(Controller))
		{
			// Move and fade in camera.
			CameraReference->SetMinimumAndMaximumX(NewCameraMinimumX, NewCameraMaximumX);
			CameraReference->SetActorLocation(NewCameraLocation);
			Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0f, 0.0f), CameraFadeTime);

			// Enable control.
			Pawn->SetForceMoveRight(false);
			Pawn->SetActorLocation(NewPawnLocation);
			Pawn->EnableInput(Controller);
		}
	}
}
