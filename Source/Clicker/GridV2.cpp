// Fill out your copyright notice in the Description page of Project Settings.


#include "GridV2.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Vector.h"

// Sets default values
AGridV2::AGridV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	FloorDrawingComponent = CreateDefaultSubobject<UProceduralMeshComponent>("FloorDrawingComponent");
	SetRootComponent(FloorDrawingComponent);
	GridLineDrawingComponent = CreateDefaultSubobject<UProceduralMeshComponent>("GridLineDrawingComponent");
	GridSelectionDrawingComponent = CreateDefaultSubobject<UProceduralMeshComponent>("GridSelectionDrawingComponent");
	GridDataComponent = CreateDefaultSubobject<UGridDataComponent>("GridDataComponent");
}

void AGridV2::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

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
	GridLineDrawingComponent->SetTranslucentSortPriority(1);

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
	// Need to set Translucent sort priority to avoid selection component from disappearing over the floor component.
	GridSelectionDrawingComponent->SetTranslucentSortPriority(1);


}

// Called when the game starts or when spawned
void AGridV2::BeginPlay()
{
	Super::BeginPlay();

	GridDataComponent->Init(NumRows, NumColumns);

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

bool AGridV2::TryPlaceObjectAtWorldLocation(APlaceableObject* ObjectToPlace, FVector WorldLocation)
{
	check(IsValid(ObjectToPlace));
	int32 Row;
	int32 Column;
	if (WorldLocationToTile(WorldLocation, Row, Column))
	{
		if (GridDataComponent->TryPlaceObjectAtLocation(ObjectToPlace, FVector2D(Row, Column)))
		{
			// TODO: Z is hard programmed, change this, also need to change APlaceableObject's world location setting to proper set for the upperleft tile
			ObjectToPlace->SetActorLocation(WorldLocation);
			return true;
		}
	}
	return false;
}