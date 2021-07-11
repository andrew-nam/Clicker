// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::InitGrid()
{
	MapTiles.Empty(GridWidth * GridHeight);
	MapTiles.AddZeroed(MapTiles.Max());
	FVector SpawnLocation;
	for (int32 Column = 0; Column < GridWidth; ++Column)
	{
		for (int32 Row = 0; Row < GridHeight; ++Row)
		{
			int32 GridAddress;
			GetGridAddressWithOffset(0, Column, Row, GridAddress);
			SpawnLocation = GetLocationFromGridAddress(GridAddress);

			CreateTile(TileLibrary[0].TileClass, TileLibrary[0].TileMaterial, SpawnLocation, GridAddress, 0);
		}
	}
}

ATile* AGrid::CreateTile(TSubclassOf<class ATile> TileToSpawn, class UMaterialInstanceConstant* TileMaterial, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID)
{
	// If we have set something to spawn:
	if (TileToSpawn)
	{
		checkSlow(TileLibrary.IsValidIndex(TileTypeID));
		// Check for a valid World:
		UWorld* const World = GetWorld();
		if (World)
		{
			// Set the spawn parameters.
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// Tiles never rotate
			FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
			// Spawn the tile.
			ATile* const NewTile = World->SpawnActor<ATile>(TileToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
			NewTile->TileTypeID = TileTypeID;
			NewTile->SetTileMaterial(TileMaterial);
			NewTile->SetGridAddress(SpawnGridAddress);
			MapTiles[SpawnGridAddress] = NewTile;
			return NewTile;
		}
	}
}

ATile* AGrid::GetTileFromGridAddress(int32 GridAddress) const
{
	if (MapTiles.IsValidIndex(GridAddress))
	{
		return MapTiles[GridAddress];
	}
	return nullptr;
}

FVector AGrid::GetLocationFromGridAddress(int32 GridAddress) const
{
	FVector Center = GetActorLocation();
	FVector OutLocation = FVector(-(GridWidth * 0.5f) * TileSize.X + (TileSize.X * 0.5f), 0.0f, -(GridHeight * 0.5f) * TileSize.Y + (TileSize.Y * 0.5f));
	check(GridWidth > 0);
	OutLocation.X += TileSize.X * (float)(GridAddress % GridWidth);
	OutLocation.Y += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;

	return OutLocation;
}

FVector AGrid::GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const
{
	FVector OutLocation = GetLocationFromGridAddress(GridAddress);
	OutLocation.X += TileSize.X * (float)(XOffsetInTiles);
	OutLocation.Y += TileSize.Y * (float)(YOffsetInTiles);
	return OutLocation;
}

bool AGrid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const
{
	int32 NewAxisValue;
	
	// Initialize to an invalid address.
	ReturnGridAddress = -1;

	// Check for going off the map in the X direction.
	check(GridWidth > 0);
	NewAxisValue = (InitialGridAddress % GridWidth) + XOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridWidth - 1)))
	{
		return false;
	}

	// Check for going off the map in the Y direction.
	NewAxisValue = (InitialGridAddress / GridWidth) + YOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridHeight - 1)))
	{
		return false;
	}

	ReturnGridAddress = (InitialGridAddress + XOffset + (YOffset * GridWidth));
	check(ReturnGridAddress >= 0);
	check(ReturnGridAddress < (GridWidth* GridHeight));
	return true;
}

bool AGrid::AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const
{
	if ((FMath::Min(GridAddressA, GridAddressB) >= 0) && (FMath::Max(GridAddressA, GridAddressB) < (GridWidth * GridHeight)))
	{
		int32 GridAddressOffset = FMath::Abs(GridAddressA - GridAddressB);
		return ((GridAddressOffset == 1 && FMath::Min(GridAddressA, GridAddressB) % GridWidth != GridWidth - 1) || (GridAddressOffset == GridWidth));
	}
	return false;
}