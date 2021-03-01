// Copyright Epic Games, Inc. All Rights Reserved.


#include "ClickerGameModeBase.h"
#include "ClickerPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

AClickerGameModeBase::AClickerGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = APawn::StaticClass();
	PlayerControllerClass = AClickerPlayerController::StaticClass();
}

void AClickerGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}