// Fill out your copyright notice in the Description page of Project Settings.


#include "ClickerPlayerController.h"

AClickerPlayerController::AClickerPlayerController(const FObjectInitializer& ObjectInitializer)
{
	bShowMouseCursor = true;

	bEnableTouchEvents = bEnableClickEvents = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;

	Money = 0;
	ClickCount = 0;
}

int32 AClickerPlayerController::GetClickCount()
{
	return ClickCount;
}

void AClickerPlayerController::SetClickCount(int32 NewClickCount)
{
	ClickCount = NewClickCount;
}

int32 AClickerPlayerController::GetMoney()
{
	return Money;
}

void AClickerPlayerController::SetMoney(int32 NewMoney)
{
	Money = NewMoney;
}
