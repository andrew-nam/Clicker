// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClickerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CLICKER_API AClickerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AClickerGameModeBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
};
