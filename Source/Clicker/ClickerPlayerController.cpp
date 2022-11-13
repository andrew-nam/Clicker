// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "ClickerGameModeBase.h"
#include "ClickerPlayerController.h"

AClickerPlayerController::AClickerPlayerController(const FObjectInitializer& ObjectInitializer)
{
	bShowMouseCursor = true;

	bEnableTouchEvents = bEnableClickEvents = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;

	Money = 0;
	ClickCount = 0;
	AClickerGameModeBase* GameMode = Cast<AClickerGameModeBase>(UGameplayStatics::GetGameMode(this));
	GridReference = GameMode->GetGrid();
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

void AClickerPlayerController::HighlightGridUnderCursor()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult))
	{
		int Row;
		int Column;
		GridReference->WorldLocationToTile(HitResult.Location, Row, Column);
		GridReference->SetSelectedTile(Row, Column);
	}
	else
	{
		GridReference->SetSelectedTile(-1, -1);
	}
}