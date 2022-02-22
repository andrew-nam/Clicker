// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "Math/Color.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridV2.generated.h"

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


	bool WorldLocationToTile(FVector WorldLocation, int32& OutRow, int32& OutColumn);

	bool TileToGridLocation(int32 Row, int32 Column, bool isCenter, FVector2D& OutGridLocation);

	void SetSelectedTile(int32 Row, int32 Column);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	class UProceduralMeshComponent* GridDrawingComponent;

	void CreateLine(FVector Start, FVector End, float LineThickness, TArray<FVector>& OutVertices, TArray<int32>& OutTriangles);

	float GetGridWidth();

	float GetGridHeight();

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
