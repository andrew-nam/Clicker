// Fill out your copyright notice in the Description page of Project Settings.


#include "Hamster.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "ClickerPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperFlipbook.h"

const int kHungryThreshold = 500;
const int kStarvingThreshold = 0;
const int kHamsterMaxHealth = 500;
const int kHamsterMinHealth = 0;

IdleState HamsterState::Idle;
HungryState HamsterState::Hungry;
StarvingState HamsterState::Starving;
DeathState HamsterState::Dead;
EatingState HamsterState::Eating;

void HamsterState::HandleInput(AHamster& Hamster)
{

}

void HamsterState::Update(AHamster& Hamster)
{

}

void HamsterState::Enter(AHamster& Hamster)
{

}

void EatingState::HandleInput(AHamster& Hamster)
{

}

void EatingState::Enter(AHamster& Hamster)
{
	Hamster.Hunger += 10;
	Hamster.FlipbookComponent->SetFlipbook(Hamster.EatingFlipbook);
	Hamster.FlipbookComponent->SetLooping(0);
	Hamster.FlipbookComponent->PlayFromStart();
}

void EatingState::Update(AHamster& Hamster)
{
	if (!Hamster.FlipbookComponent->IsPlaying())
	{
		Hamster.State_ = &HamsterState::Idle;
		Hamster.State_->Enter(Hamster);
	}
}

void IdleState::Enter(AHamster& Hamster)
{
	if (Hamster.Hunger > kHungryThreshold)
	{
		Hamster.FlipbookComponent->SetFlipbook(Hamster.IdleFlipbook);
		Hamster.FlipbookComponent->SetLooping(1);
		Hamster.FlipbookComponent->PlayFromStart();
	}
	else
	{
		Hamster.State_ = &HamsterState::Hungry;
		Hamster.State_->Enter(Hamster);
	}
}

void IdleState::Update(AHamster& Hamster)
{
	if (Hamster.Health < kHamsterMaxHealth)
	{
		Hamster.Health += 1;
	}
	Hamster.Hunger -= 1;
	if (Hamster.Hunger < kHungryThreshold)
	{
		Hamster.State_ = &HamsterState::Hungry;
		Hamster.State_->Enter(Hamster);
	}
}

void HungryState::Enter(AHamster& Hamster)
{
	if (Hamster.Hunger > kStarvingThreshold && Hamster.Hunger <= kHungryThreshold)
	{
		Hamster.FlipbookComponent->SetFlipbook(Hamster.HungryFlipbook);
		Hamster.FlipbookComponent->SetLooping(1);
		Hamster.FlipbookComponent->PlayFromStart();
	}
	else
	{
		Hamster.State_ = &HamsterState::Starving;
		Hamster.State_->Enter(Hamster);
	}
}

void HungryState::Update(AHamster& Hamster)
{
	Hamster.Hunger -= 1;
	if (Hamster.Hunger <= kStarvingThreshold)
	{
		Hamster.State_ = &HamsterState::Starving;
		Hamster.State_->Enter(Hamster);
	}
}

void StarvingState::Enter(AHamster& Hamster)
{
	if (Hamster.Hunger <= kStarvingThreshold && Hamster.Health > kHamsterMinHealth)
	{
		Hamster.Hunger = kStarvingThreshold;
		Hamster.FlipbookComponent->SetFlipbook(Hamster.StarvingFlipbook);
		Hamster.FlipbookComponent->SetLooping(1);
		Hamster.FlipbookComponent->PlayFromStart();
	}
	else if(Hamster.Health <= kHamsterMinHealth)
	{
		Hamster.State_ = &HamsterState::Dead;
		Hamster.State_->Enter(Hamster);
	}
}

void StarvingState::Update(AHamster& Hamster)
{
	Hamster.Health -= 1;
	if (Hamster.Health <= kHamsterMaxHealth)
	{
		Hamster.State_ = &HamsterState::Dead;
		Hamster.State_->Enter(Hamster);
	}
}


void DeathState::Enter(AHamster& Hamster)
{
	if (Hamster.Health <= kHamsterMinHealth)
	{
		Hamster.FlipbookComponent->SetFlipbook(Hamster.DeathFlipbook);
		Hamster.FlipbookComponent->SetLooping(1);
		Hamster.FlipbookComponent->PlayFromStart();
	}
	else
	{
		Hamster.State_ = &HamsterState::Idle;
		Hamster.State_->Enter(Hamster);
	}
}

void DeathState::HandleInput(AHamster& Hamster)
{
	// TODO: Music notes
}

// Sets default values
AHamster::AHamster()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SphereCollision = CreateDefaultSubobject<USphereComponent>(FName("HamsterSphereComponent"));
	//RootComponent = SphereCollision;
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(FName("HamsterSprite"));
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookIdle(TEXT("/Game/Sprites/Hamsters/Hamster_Idle.Hamster_Idle"));
	IdleFlipbook = FlipbookIdle.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookEating(TEXT("/Game/Sprites/Hamsters/Hamster_Eating.Hamster_Eating"));
	EatingFlipbook = FlipbookEating.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookHungry(TEXT("/Game/Sprites/Hamsters/Hamster_Hungry.Hamster_Hungry"));
	HungryFlipbook = FlipbookHungry.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookStarving(TEXT("/Game/Sprites/Hamsters/Hamster_Starving.Hamster_Starving"));
	StarvingFlipbook = FlipbookStarving.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookDeath(TEXT("/Game/Sprites/Hamsters/Hamster_Death.Hamster_Death"));
	DeathFlipbook = FlipbookDeath.Object;
	
	FlipbookComponent->SetFlipbook(IdleFlipbook);
	FlipbookComponent->OnClicked.AddDynamic(this, &AHamster::Clicked);
	//FlipbookComponent->SetupAttachment(RootComponent);
	Health = 500;
	Happiness = 100;
	Hunger = 750;
}

// Called when the game starts or when spawned
void AHamster::BeginPlay()
{
	Super::BeginPlay();
	ClickerPlayerControllerRef = (UClickerPlayerController *)GetWorld()->GetFirstPlayerController();

}

// Called every frame
void AHamster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	State_->Update(*this);

}

// Called to bind functionality to input
//void AHamster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AHamster::Clicked(UPrimitiveComponent* TouchedComponent, FKey Key)
{
	if (Health <= 0)
	{
		State_->HandleInput(*this);
		return;
	}
	State_ = &HamsterState::Eating;
	State_->HandleInput(*this);
	State_->Enter(*this);
}

