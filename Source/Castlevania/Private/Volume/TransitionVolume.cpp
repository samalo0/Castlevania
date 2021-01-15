//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/1/2021
//----------------------------------------------------------------------------------------------------------------------

#include "Volume/TransitionVolume.h"

#include "Components/BrushComponent.h"
#include "Core/CastlevaniaCameraActor.h"
#include "Pawn/CastlevaniaPawn.h"

ATransitionVolume::ATransitionVolume()
{
	GetBrushComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	GetBrushComponent()->SetMobility(EComponentMobility::Static);
}

void ATransitionVolume::BeginPlay()
{
	Super::BeginPlay();

	GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATransitionVolume::OnBeginOverlap);
}

void ATransitionVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Pawn = Cast<ACastlevaniaPawn>(OtherActor);
	if(IsValid(Pawn) && Pawn->MovementComponent->bIsOnStairs && IsValid(CameraReference))
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			APlayerController* Controller = World->GetFirstPlayerController();
			if(IsValid(Controller))
			{
				// Fade out camera
				Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0f, 1.0f), CameraFadeTime, false, true);		
			}

			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, this, &ATransitionVolume::OnBeginOverlapAfterDelay, CameraFadeTime, false);
		}
	}
}

void ATransitionVolume::OnBeginOverlapAfterDelay() const
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if(IsValid(Controller))
		{
			Pawn->SetActorLocation(NewPawnLocation);
			CameraReference->SetActorLocation(NewCameraLocation);
			CameraReference->SetMinimumAndMaximumX(NewCameraMinimumX, NewCameraMaximumX);

			// Fade in camera
			Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0f, 0.0f), CameraFadeTime);		
		}
	}
}

