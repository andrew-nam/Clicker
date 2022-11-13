// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "GridDataComponent.h"
#include "Math/Color.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableObject.h"
#include "GridV2.generated.h"

USTRUCT(BlueprintType)
struct FTileV2
{
	GENERATED_USTRUCT_BODY();
	FTileV2() { X = 0; Y = 0; };

	FTileV2(int32 x, int32 y) { X = x; Y = y; };

	int32 X;
	int32 Y;
};

UCLASS()
class CLICKER_API AGridV2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridV2();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	int32 NumRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	int32 NumColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float LineThickness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	FLinearColor LineColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	FLinearColor SelectionColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float LineOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float SelectionOpacity;

	UPROPERTY(EditAnywhere)
	class UMaterial* GridMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* SelectionMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* FloorMaterial;

	UFUNCTION(BlueprintCallable, Category = "World/2D Transform")
	bool WorldLocationToTile(FVector WorldLocation, int32& OutRow, int32& OutColumn);

	UFUNCTION(BlueprintCallable, Category = "World/2D Transform")
	bool TileToGridLocation(int32 Row, int32 Column, bool isCenter, FVector2D& OutGridLocation);

	UFUNCTION(BlueprintCallable, Category = "World/2D Transform")
	void SetSelectedTile(int32 Row, int32 Column);

	UFUNCTION(BlueprintCallable, Category = "Object Placement")
	bool TryPlaceObjectAtWorldLocation(APlaceableObject* ObjectToPlace, FVector WorldLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	class UProceduralMeshComponent* GridLineDrawingComponent;

	class UProceduralMeshComponent* GridSelectionDrawingComponent;

	class UProceduralMeshComponent* FloorDrawingComponent;

	class UGridDataComponent* GridDataComponent;

	void CreateFloorMesh(float GridWidth, float GridHeight, TArray<FVector>& OutVertices, TArray<int32>& OutTriangles);

	void CreateLine(FVector Start, FVector End, float LineThickness, TArray<FVector>& OutVertices, TArray<int32>& OutTriangles);

	float GetGridWidth();

	float GetGridHeight();

	bool IsTileValid(int32 Row, int32 Column);

	UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor Color, float Opacity);


	/*
	ATile* GetTileFromGridAddress(int32 GridAddress) const;

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitGrid();


	UFUNCTION(BlueprintCallable, Category = Tile)
		FVector GetLocationFromGridAddress(int32 GridAddress) const;

	FVector GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const;

	UFUNCTION(BlueprintCallable, Category = Tile)
		bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const; */


};
