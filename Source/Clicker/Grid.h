// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "Tile.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FTileType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstanceConstant* TileMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor EffectColor = FLinearColor::White;
};

UCLASS()
class CLICKER_API AGrid : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGrid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATile*> MapTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileType> TileLibrary;

	/** THe size of a space on the grid. Does not include borders or spacing between tiles. */
	UPROPERTY(EditAnywhere, Category = Tile)
	FVector2D TileSize;

	/** The width of the grid. Needed to calculate tile positions and neighbors. */
	int32 GridWidth;

	/** The height of the grid. */
	int32 GridHeight;

	/** Spawn a tile and associate it with a specific grid address. */
	ATile* CreateTile(TSubclassOf<class ATile> TileToSpawn, class UMaterialInstanceConstant* TileMaterial, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeId);

	/**Get the pointer to the tile at the specified grid address. */
	ATile* GetTileFromGridAddress(int32 GridAddress) const;

	UFUNCTION(BlueprintCallable, Category = Initialization)
	void InitGrid();

	/** Get the world location for a given grid address. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	FVector GetLocationFromGridAddress(int32 GridAddress) const;
	/** Get the world location for a grid address relative to another grid address. Offset between addresses is measured in tiles. */
	FVector GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const;
	/** Get a grid address relative to another grid address. Offset between addresses is measured in tiles */
	UFUNCTION(BlueprintCallable, Category = Tile)
	bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32 &ReturnGridAddress) const;
	/** Determine if two grid addresses are valid and adjacent. */
	bool AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const;

	/** React to a tile being clicked. */
	void OnTileWasSelected(ATile* NewSelectedTile);

	ATile* GetCurrentlySelectedTile() const { return CurrentlySelectedTile; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
	ATile* CurrentlySelectedTile;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
