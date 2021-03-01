// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hamster.generated.h"

class HamsterState
{
public:
	virtual ~HamsterState() {}
	virtual void HandleInput(AHamster& Hamster);
	virtual void Update(AHamster& Hamster);
	virtual void Enter(AHamster& Hamster);
};

class IdleState : public HamsterState
{
public:
	IdleState() {}
	virtual void Update(AHamster& Hamster);
	virtual void Enter(AHamster& Hamster);
};

class HungryState : public HamsterState
{
public:
	HungryState() {}
	virtual void Update(AHamster& Hamster);
	virtual void Enter(AHamster& Hamster);
};

class StarvingState : public HamsterState
{
public:
	StarvingState() {}
	virtual void Update(AHamster& Hamster);
	virtual void Enter(AHamster& Hamster);
};

class EatingState : public HamsterState
{
public:
	EatingState() {}
	virtual void HandleInput(AHamster& Hamster);
	virtual void Update(AHamster& Hamster);
	virtual void Enter(AHamster& Hamster);
};

class DeathState : public HamsterState
{
public:
	DeathState() {}
	virtual void HandleInput(AHamster& Hamster);
	virtual void Enter(AHamster& Hamster);
};

UCLASS()
class CLICKER_API AHamster : public APawn
{
	GENERATED_BODY()

	//UPROPERTY(Category = "Collision", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//class USphereComponent * SphereCollision;

public:
	// Sets default values for this pawn's properties
	AHamster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, Category = "Hamster Stats")
	int32 Health;

	UPROPERTY(BlueprintReadWrite, Category = "Hamster Stats")
	int32 Happiness;

	UPROPERTY(BlueprintReadWrite, Category = "Hamster Stats")
	int32 Hunger;

	UPROPERTY(BlueprintReadWrite, Category = "Hamster Stats")
	int32 HealthDeteriorationRate;

	UPROPERTY(BlueprintReadWrite, Category = "Hamster Stats")
	int32 HappinessDeteriorationRate;

	UPROPERTY(BlueprintReadWrite, Category = "Hamster Stats")
	int32 HungerDeteriorationRate;

	UPROPERTY(Category = "Sprites", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* FlipbookComponent;

	class HamsterState* State_ = new IdleState();

	UFUNCTION(BlueprintCallable)
	void Clicked(UPrimitiveComponent* TouchedComponent, FKey Key);

	class UPaperFlipbook* IdleFlipbook;
	class UPaperFlipbook* EatingFlipbook;
	class UPaperFlipbook* DeathFlipbook;
	class UPaperFlipbook* StarvingFlipbook;
	class UPaperFlipbook* HungryFlipbook;

private:
	class UClickerPlayerController* ClickerPlayerControllerRef;

};
