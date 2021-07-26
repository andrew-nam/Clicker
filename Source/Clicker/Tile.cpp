// Fill out your copyright notice in the Description page of Project Settings.


#include "Materials/MaterialInstanceConstant.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Tile.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

	if (RootComponent)
	{
		RootComponent->SetMobility(EComponentMobility::Movable);
	}
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
