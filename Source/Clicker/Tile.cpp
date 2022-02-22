// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

	if (RootComponent)
	{
		RootComponent->SetMobility(EComponentMobility::Movable);
	}
	GetRenderComponent()->OnClicked.AddDynamic(this, &ATile::TilePress_Mouse);/*
	GetRenderComponent()->OnBeginCursorOver.AddDynamic(this, &ATile::TileEnter_Mouse);
	GetRenderComponent()->OnEndCursorOver.AddDynamic(this, &ATile::TileLeave_Mouse);*/
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	Grid = Cast<AGrid>(GetOwner());
}

// Called every frame

void ATile::SetTileMaterial_Implementation(class UMaterialInstanceConstant* MaterialToUse)
{
	GetRenderComponent()->SetMaterial(0, MaterialToUse);
}

void ATile::SetGridAddress(int32 NewLocation)
{
	GridAddress = NewLocation;
}

int32 ATile::GetGridAddress() const
{
	return GridAddress;
}

void ATile::TilePress_Mouse(UPrimitiveComponent* TouchedComponent, FKey ButtonClicked)
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked"));
	if (!UGameplayStatics::IsGamePaused(this) && Grid)
	{
		Grid->OnTileWasSelected(this);
	}
}

void ATile::TileEnter_Mouse(AActor* MousedOverActor)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		Grid->OnTileWasEntered((ATile*) MousedOverActor);
		//Grid->IsTileAreaValid(this);
		if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			
		}
	}
}

void ATile::TileLeave_Mouse()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		Grid->OnTileWasLeft();
	}
}

void ATile::onStateChange()
{
	UE_LOG(LogTemp, Warning, TEXT("Changing State"));
	switch (TempTileState)
	{
	case ETileState::ETS_Empty :
		SetTileMaterial_Implementation(EmptyMaterial);
		break;

	case ETileState::ETS_Valid :
		SetTileMaterial_Implementation(ValidMaterial);
		break;

	case ETileState::ETS_Invalid :
		SetTileMaterial_Implementation(InvalidMaterial);
		break;
	}
}