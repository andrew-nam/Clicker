// Fill out your copyright notice in the Description page of Project Settings.


#include "GridDataComponent.h"

// Sets default values for this component's properties
UGridDataComponent::UGridDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGridDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGridDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGridDataComponent::Init(int32 Rows, int32 Columns)
{
	NumRows = Rows;
	NumColumns = Columns;
	GridSize = NumRows * NumColumns;
	PlacedObjects.Init(nullptr, GridSize);
}

FVector2D UGridDataComponent::IndexToTile(int32 Index)
{
	FVector2D Tile;
	Tile.X = Index % NumColumns;
	Tile.Y = Index / NumColumns;

	return Tile;
}

int32 UGridDataComponent::TileToIndex(FVector2D Tile)
{
	return Tile.X + Tile.Y * NumColumns;
}

APlaceableObject* UGridDataComponent::GetObjectAtIndex(int32 Index)
{
	if (PlacedObjects.IsValidIndex(Index))
	{
		return PlacedObjects[Index];
	}
	return nullptr;
}

bool UGridDataComponent::TryPlaceObject(APlaceableObject* ObjectToPlace)
{
	check(IsValid(ObjectToPlace));

	int32 index = 0;
	bool result = false;
	for (APlaceableObject* PlacedObject : PlacedObjects)
	{
		if (IsRoomAvailable(ObjectToPlace, index))
		{
			PlaceObjectAtIndex(ObjectToPlace, index);
			result = true;
		}
		index++;
	}
	return result;
}

bool UGridDataComponent::IsRoomAvailable(APlaceableObject* ObjectToPlace, int32 TopLeftIndex)
{
	check(IsValid(ObjectToPlace));


	FVector2D TopLeftTile = IndexToTile(TopLeftIndex);
	FIntPoint PlacedObjectDimensions = ObjectToPlace->GetDimensions();
	for (int32 x = TopLeftTile.X; x < TopLeftTile.X + PlacedObjectDimensions.X - 1; x++)
	{
		for (int32 y = TopLeftTile.Y; y < TopLeftTile.Y + PlacedObjectDimensions.Y - 1; y++)
		{
			if (!IsTileValid(x, y))
			{
				return false;
			}

			int32 CurrentIndex = TileToIndex(FVector2D(x, y));
			if (!PlacedObjects.IsValidIndex(CurrentIndex))
			{
				return false;
			}

			if (IsValid(GetObjectAtIndex(CurrentIndex)))
			{
				return false;
			}
		}
	}
	return true;
}

bool UGridDataComponent::PlaceObjectAtIndex(APlaceableObject* ObjectToPlace, int32 TopLeftIndex)
{
	check(IsValid(ObjectToPlace));
	bool result = false;

	FVector2D TopLeftTile = IndexToTile(TopLeftIndex);
	FIntPoint PlacedObjectDimensions = ObjectToPlace->GetDimensions();
	for (int32 x = TopLeftTile.X; x < TopLeftTile.X + PlacedObjectDimensions.X - 1; x++)
	{
		for (int32 y = TopLeftTile.Y; y < TopLeftTile.Y + PlacedObjectDimensions.Y - 1; y++)
		{
			PlacedObjects[TileToIndex(FVector2D(x, y))] = ObjectToPlace;
		}
	}
	result = true;
	IsDirty = true;
	return result;
}

bool UGridDataComponent::IsTileValid(int32 Row, int32 Column)
{
	return Row >= 0 && Row < NumRows && Column >= 0 && Column < NumColumns;
}