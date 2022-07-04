// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaceableObject.h"
#include "GridDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLICKER_API UGridDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	// TODO: Make GridDataComponent not type specific, would need different types of objects in index and types of tiles
	UGridDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init(int32 Rows, int32 Columns);

	bool CheckTileValidity();

	bool TryPlaceObject(APlaceableObject* ObjectToPlace);

	bool IsRoomAvailable(APlaceableObject* ObjectToPlace, int32 TopLeftIndex);

	bool PlaceObjectAtTile(APlaceableObject* ObjectToPlace, int32 TopLeftIndex);

	APlaceableObject* GetObjectAtTile(FVector2D);



private: 
	int NumColumns;

	int NumRows;

	int GridSize;

	bool IsDirty;
	// TODO: Objects of greater than a single tile should have references to connecting tiles
	TArray<APlaceableObject*> PlacedObjects;
	
	FVector2D IndexToTile(int32 Index);

	int32 TileToIndex(FVector2D Tile);

	APlaceableObject* GetObjectAtIndex(int32 Index);

	bool PlaceObjectAtIndex(APlaceableObject* ObjectToPlace, int32 Index);

	bool IsTileValid(int32 Row, int32 Column);



		
};
