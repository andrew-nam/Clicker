// Fill out your copyright notice in the Description page of Project Settings.


#include "GridV2.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Vector.h"

// Sets default values
AGridV2::AGridV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* SelectionMaterialInstance = CreateMaterialInstance(SelectionColor, SelectionOpacity);

	GridDrawingComponent = CreateDefaultSubobject<UProceduralMeshComponent>("GridDrawingComponent");

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

	GridDrawingComponent->CreateMeshSection(0, LineVertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
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

bool isTileValid(int32 Row, int32 Column)
{
	return true;
}