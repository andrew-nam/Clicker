// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClickerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CLICKER_API AClickerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AClickerPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetClickCount();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetClickCount(int32 NewClickCount);

	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetMoney();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetMoney(int32 NewMoney);

	UFUNCTION(BlueprintCallable, Category = "Grid Interaction")
	void AClickerPlayerController::HighlightGridUnderCursor();

protected:
	UPROPERTY()
	int32 Money;

	UPROPERTY()
	int32 ClickCount;

	AGridV2* GridReference;
};
