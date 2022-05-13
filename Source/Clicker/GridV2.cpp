// Fill out your copyright notice in the Description page of Project Settings.


#include "GridV2.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Vector.h"

// Sets default values
AGridV2::AGridV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	GridLineDrawingComponent = CreateDefaultSubobject<UProceduralMeshComponent>("GridLineDrawingComponent");
	GridSelectionDrawingComponent = CreateDefaultSubobject<UProceduralMeshComponent>("GridSelectionDrawingComponent");
	FloorDrawingComponent = CreateDefaultSubobject<UProceduralMeshComponent>("FloorDrawingComponent");
}

void AGridV2::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GridSize = NumRows * NumColumns;

	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* SelectionMaterialInstance = CreateMaterialInstance(SelectionColor, SelectionOpacity);
	UMaterialInstanceDynamic* FloorMaterialInstance = CreateMaterialInstance(FLinearColor::White, 1);

	float LineStart;
	float LineEnd;
	TArray<FVector> LineVertices;
	TArray<int32> Triangles;
	for (int i = 0; i <= NumRows; i++)
	{
		LineStart = i * TileSize;
		LineEnd = GetGridWidth();
		CreateLine(FVector(LineStart, 0, 0), FVector(LineStart, LineEnd, 0), LineThickness, LineVertices, Triangles);
	}

	for (int i = 0; i <= NumColumns; i++)
	{
		LineStart = i * TileSize;
		LineEnd = GetGridHeight();
		CreateLine(FVector(0, LineStart, 0), FVector(LineEnd, LineStart, 0), LineThickness, LineVertices, Triangles);
	}
	
	// Create the procedural mesh for the lines
	GridLineDrawingComponent->CreateMeshSection(0, LineVertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	GridLineDrawingComponent->SetMaterial(0, LineMaterialInstance);

	TArray<FVector> FloorVertices;
	TArray<int32> FloorTriangles;
	CreateFloorMesh(GetGridWidth(), GetGridHeight(), FloorVertices, FloorTriangles);
	FloorDrawingComponent->CreateMeshSection(0, FloorVertices, FloorTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	FloorDrawingComponent->SetMaterial(0, FloorMaterialInstance);

	TArray<FVector> SelectionVertices;
	TArray<int32> SelectionTriangles;
	CreateLine(FVector(0, TileSize / 2, 0), FVector(TileSize, TileSize / 2, 0), TileSize, SelectionVertices, SelectionTriangles);
	GridSelectionDrawingComponent->CreateMeshSection(0, SelectionVertices, SelectionTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	GridSelectionDrawingComponent->SetMaterial(0, SelectionMaterialInstance);
	GridSelectionDrawingComponent->SetVisibility(false);
	// Need to set Trunslucent sort priority to avoid selection component from disappearing over the floor component.
	GridSelectionDrawingComponent->SetTranslucentSortPriority(1);

	PlacedObjects.Init(nullptr, GridSize);

}

// Called when the game starts or when spawned
void AGridV2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGridV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AGridV2::GetGridWidth()
{
	return NumColumns * TileSize;
}

float AGridV2::GetGridHeight()
{
	return NumRows * TileSize;
}

bool AGridV2::WorldLocationToTile(FVector WorldLocation, int32& OutRow, int32& OutColumn)
{
	OutRow = floor((WorldLocation.X - GetActorLocation().X) / GetGridWidth() * NumRows);
	OutColumn = floor((WorldLocation.Y - GetActorLocation().Y) / GetGridHeight() * NumColumns);
	return IsTileValid(OutRow, OutColumn);
	
}

bool AGridV2::TileToGridLocation(int32 Row, int32 Column, bool isCenter, FVector2D& OutGridLocation)
{
	OutGridLocation.X = Row * TileSize + GetActorLocation().X;
	OutGridLocation.Y = Column * TileSize + GetActorLocation().Y;
	if (isCenter)
	{
		OutGridLocation.X += TileSize / 2;
		OutGridLocation.Y += TileSize / 2;
	}
	return IsTileValid(Row, Column);
}

void AGridV2::SetSelectedTile(int32 Row, int32 Column)
{
	FVector2D GridLocation;
	if (TileToGridLocation(Row, Column, false, GridLocation))
	{
		GridSelectionDrawingComponent->SetVisibility(true);
		GridSelectionDrawingComponent->SetWorldLocation(FVector(GridLocation.X, GridLocation.Y, GetActorLocation().Z));
	}
	else
	{
		GridSelectionDrawingComponent->SetVisibility(false);
	}

}

FTileV2 AGridV2::IndexToTile(int32 Index)
{
	FTileV2 Tile;
	Tile.X = Index % NumColumns;
	Tile.Y = Index / NumColumns;

	return Tile;
}

int32 AGridV2::TileToIndex(FTileV2 Tile)
{
	return Tile.X + Tile.Y * NumColumns;
}

APlaceableObject* AGridV2::GetObjectAtIndex(int32 Index)
{
	if (PlacedObjects.IsValidIndex(Index))
	{
		return PlacedObjects[Index];
	}
	return nullptr;
}

bool AGridV2::TryPlaceObject(APlaceableObject* ObjectToPlace)
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

bool AGridV2::IsRoomAvailable(APlaceableObject* ObjectToPlace, int32 TopLeftIndex)
{
	check(IsValid(ObjectToPlace));


	FTileV2 TopLeftTile = IndexToTile(TopLeftIndex);
	FIntPoint PlacedObjectDimensions = ObjectToPlace->GetDimensions();
	for (int32 x = TopLeftTile.X; x < TopLeftTile.X + PlacedObjectDimensions.X - 1; x++)
	{
		for (int32 y = TopLeftTile.Y; y < TopLeftTile.Y + PlacedObjectDimensions.Y - 1; y++)
		{
			if (!IsTileValid(x, y))
			{
				return false;
			}

			int32 CurrentIndex = TileToIndex(FTileV2(x, y));
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

bool AGridV2::PlaceObjectAtIndex(APlaceableObject* ObjectToPlace, int32 TopLeftIndex)
{
	check(IsValid(ObjectToPlace));
	bool result = false;

	FTileV2 TopLeftTile = IndexToTile(TopLeftIndex);
	FIntPoint PlacedObjectDimensions = ObjectToPlace->GetDimensions();
	for (int32 x = TopLeftTile.X; x < TopLeftTile.X + PlacedObjectDimensions.X - 1; x++)
	{
		for (int32 y = TopLeftTile.Y; y < TopLeftTile.Y + PlacedObjectDimensions.Y - 1; y++)
		{
			PlacedObjects[TileToIndex(FTileV2(x, y))] = ObjectToPlace;
		}
	}
	result = true;
	IsDirty = true;
	return result;
}

UMaterialInstanceDynamic* AGridV2::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	if (IsValid(GridMaterial))
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GridMaterial, this);
		DynamicMaterial->SetVectorParameterValue("Color", Color);
		DynamicMaterial->SetScalarParameterValue("Opacity", Opacity);

		return DynamicMaterial;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Material not set"));
		return nullptr;
	}
}


void AGridV2::CreateFloorMesh(float GridWidth, float GridHeight, TArray<FVector>& OutVertices, TArray<int32>& OutTriangles)
{
	OutTriangles.Add(OutVertices.Num() + 2);
	OutTriangles.Add(OutVertices.Num() + 1);
	OutTriangles.Add(OutVertices.Num() + 0);
	OutTriangles.Add(OutVertices.Num() + 2);
	OutTriangles.Add(OutVertices.Num() + 3);
	OutTriangles.Add(OutVertices.Num() + 1);

	OutVertices.Add(FVector(0, 0, 0));
	OutVertices.Add(FVector(GridWidth, 0, 0));
	OutVertices.Add(FVector(0, GridHeight, 0));
	OutVertices.Add(FVector(GridWidth, GridHeight, 0));
}

void AGridV2::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &OutVertices, TArray<int32> &OutTriangles)
{
	float HalfThickness = Thickness / 2;
	FVector ThicknessDirection = FVector::CrossProduct((End-Start).GetSafeNormal(), FVector(0, 0, 1));
	// Adds the six indices for the two triangles drawn counterclockwise to make the rectangle: Triangle(2, 1, 0), Triangle(2, 3, 1)
	OutTriangles.Add(OutVertices.Num() + 2);
	OutTriangles.Add(OutVertices.Num() + 1);
	OutTriangles.Add(OutVertices.Num() + 0);
	OutTriangles.Add(OutVertices.Num() + 2);
	OutTriangles.Add(OutVertices.Num() + 3);
	OutTriangles.Add(OutVertices.Num() + 1);

	// The vertices are calculated for the actual worldspace from the triangle corners (0, 1, 2, 3)
	OutVertices.Add(Start + (ThicknessDirection * HalfThickness));
	OutVertices.Add(End + (ThicknessDirection * HalfThickness));
	OutVertices.Add(Start - (ThicknessDirection * HalfThickness));
	OutVertices.Add(End - (ThicknessDirection * HalfThickness));
}

bool AGridV2::IsTileValid(int32 Row, int32 Column)
{
	return Row >= 0 && Row < NumRows && Column >= 0 && Column < NumColumns;
}