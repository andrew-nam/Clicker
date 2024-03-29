// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(FName("BackgroundSprite"));
	SpriteComponent->SetupAttachment(RootComponent);

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
			TEnumAsByte<ETileState::Type> TileState = ETileState::ETS_Empty;

			CreateTile(BaseTile.TileClass, BaseTile.TileMaterial, SpawnLocation, GridAddress, TileState);
		}
	}
}

ATile* AGrid::CreateTile(TSubclassOf<class ATile> TileToSpawn, class UMaterialInstanceConstant* TileMaterial, FVector SpawnLocation, int32 SpawnGridAddress, TEnumAsByte<ETileState::Type> TileState)
{
	// If we have set something to spawn:
	if (TileToSpawn)
	{
		//checkSlow(TileLibrary.IsValidIndex(TileTypeID));
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
			NewTile->TileState = TileState;
			NewTile->SetTileMaterial(TileMaterial);
			NewTile->SetGridAddress(SpawnGridAddress);
			MapTiles[SpawnGridAddress] = NewTile;
			return NewTile;
		}
	}
	return nullptr;
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
	OutLocation.Z += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;

	return OutLocation;
}

FVector AGrid::GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const
{
	FVector OutLocation = GetLocationFromGridAddress(GridAddress);
	OutLocation.X += TileSize.X * (float)(XOffsetInTiles);
	OutLocation.Z += TileSize.Y * (float)(YOffsetInTiles);
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

void AGrid::OnTileWasSelected(ATile* NewSelectedTile)
{
	UE_LOG(LogTemp, Warning, TEXT("Tile Selected"));
	NewSelectedTile->TileState = ETileState::ETS_Invalid;
	NewSelectedTile->onStateChange();
}

void AGrid::OnTileWasEntered(ATile* EnteredTile)
{

}

void AGrid::OnTileWasLeft()
{
	if (ObjectTiles.IsValidIndex(0))
	{
		for (ATile* ObjectTile : ObjectTiles)
		{
			ObjectTile->TempTileState = ObjectTile->TileState;
		}
	}
}

void AGrid::IsObjectAreaValid(TArray<ATile*> ObjectTileArray)
{
	bool isValid = true;
	for (ATile* ObjectTile : ObjectTileArray)
	{
		isValid = (ObjectTile->TileState == ETileState::ETS_Valid || ObjectTile->TileState == ETileState::ETS_Empty);
		if (!isValid)
		{
			/*for (ATile* ObjectTile : ObjectTileArray)
			{
				ObjectTile->TempTileState = ETileState::ETS_Invalid;
				ObjectTile->onStateChange();
			}*/
			break;
		}
		ObjectTile->TempTileState = ETileState::ETS_Valid;
		ObjectTile->onStateChange();
	}
}

FIntPoint AGrid::GetObjectCenter(int GridAddress, FIntPoint ObjectDimensions)
{
	FIntPoint Center;

	//if (ObjectDimensions.X % 2 == 0)
	//	if (GetLocationFromGridAddress(GridAddress).X /*> cursorLocation*/ )
	//		//Center.X = math.floor(ObjectDimensions.X / 2);
	//	else
	//		/*Center.X = math.ceiling(ObjectDimensions.X / 2)*/;
	//else


		


	Center.X = ObjectDimensions.X / 2;
	Center.Y = ObjectDimensions.Y / 2;
	return Center;
}