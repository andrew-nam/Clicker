// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "PaperSpriteComponent.h"
#include "Tile.generated.h"

/**
 * 
 */

UENUM()
namespace ETileState
{
	enum Type
	{
		ETS_Empty,
		ETS_Invalid,
		ETS_Valid
	};
}

UCLASS()
class CLICKER_API ATile : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	//UFUNCTION()
	//void TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	//UFUNCTION()
	//void TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION()
	void TilePress_Mouse(UPrimitiveComponent* TouchedComponent, FKey ButtonClicked);

	UFUNCTION()
	void TileEnter_Mouse(AActor* MousedOverActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Placement Events")
	void SetPlacement();

	UFUNCTION(BlueprintNativeEvent, Category = "Special Game Events")
	void SetTileMaterial(class UMaterialInstanceConstant* TileMaterial);
	virtual void SetTileMaterial_Implementation(class UMaterialInstanceConstant* TileMaterial);

	//UFUNCTION(BlueprintNativeEvent, Category = "Placement Events")
	//void OnPlaced();
	//virtual void OnPlaced_Implementation();

	UPROPERTY(Category = "Sprites", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* SpriteComponent;

	ATile();

	void BeginPlay() override;

	void SetGridAddress(int32 NewLocation);
	int32 GetGridAddress() const;
	
	UPROPERTY(BlueprintReadOnly)
	int32 TileTypeID;

	UPROPERTY()
	TEnumAsByte<ETileState::Type> TileState;

	UPROPERTY(EditAnywhere)
	class UMaterialInstanceConstant* InvalidMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInstanceConstant* ValidMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInstanceConstant* EmptyMaterial;

	void onStateChange();

protected:
	FVector PlacementLocation;
	FVector TileLocation;

	/** Location on the grid as a 1D key/value. To find neighbors, ask the grid. */
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 GridAddress;

	int32 PlacementGridAddress;

	/** The grid that owns this tile. Currently this is set by casting the object that spawned the tile. */
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	class AGrid* Grid;

	
};
