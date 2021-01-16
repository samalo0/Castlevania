//----------------------------------------------------------------------------------------------------------------------
// Copyright 2021 Stephen Maloney
// Owner: Stephen Maloney
// Created: 1/16/2021
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CastlevaniaPlayerStart.generated.h"

class UPaperSpriteComponent;

/**
 * Child player start to add some game specific functionality. 
 */
UCLASS()
class CASTLEVANIA_API ACastlevaniaPlayerStart : public APlayerStart
{

	GENERATED_BODY()

public:

	ACastlevaniaPlayerStart(const FObjectInitializer& ObjectInitializer);
	
protected:

#if WITH_EDITORONLY_DATA
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* SpriteComponent;

#endif
	
};
