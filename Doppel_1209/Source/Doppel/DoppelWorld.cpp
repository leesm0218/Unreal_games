// Fill out your copyright notice in the Description page of Project Settings.

#include "Doppel.h"
#include "DoppelWorld.h"
#include "DoppeeCharacter.h"

#include "kismet/KismetMathLibrary.h"


// Sets default values
ADoppelWorld::ADoppelWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	num_doppees = 40;

	width = ceil(sqrt((double)num_doppees)*2);//제곱근 *2를 올림. 40->6.x->12.x->13
	height = width;
	/*
	for (int i = 0; i < height; i++) {
		TArray<e_tiles> tile_row;
		TArray<e_walls> wall_row;

		tile_row.Init(e_tiles::T_GROUND, width);
		tile_map.Add(tile_row);

		wall_row.Init(e_walls::W_EMPTY, width);
		wall_map.Add(wall_row);
	}*/
}

// Called when the game starts or when spawned
void ADoppelWorld::BeginPlay()
{
	Super::BeginPlay();
	/*
	for (int i = 0; i < height; i++) {
		for (int j = 0; i < width; i++) {
			switch (tile_map[i][j]) {
			case e_tiles::T_EMPTY:
				break;
			case e_tiles::T_GROUND:
				break;
			default:
				break;
			}
		}
	}*/
	for (int i = 0; i < width; i++)
	{
		disable_point.Add({ i, 7 });
	}


	for (int i = 0; i < num_doppees; i++) {
		POINT rand_position;

		while (1) {
			bool finded = false;
			rand_position = { FMath::Rand() % width, FMath::Rand() % height };

			for (auto doppee : doppees) {
				auto doppee_position = doppee->getPosition();

				if (doppee_position.x == rand_position.x && doppee_position.y == rand_position.y) {
					finded = true;
					break;
				}
			}

			if (!finded) {
				break;
			}
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation = GetRandomPointInWorld();
		FVector Origin = Box->Bounds.Origin;
		FVector Extent = Box->Bounds.BoxExtent;
		SpawnLocation.X = Origin.X + 2 * Extent.X * rand_position.x / width - Extent.X;
		SpawnLocation.Y = Origin.Y + 2 * Extent.Y * rand_position.y / height - Extent.Y;
		SpawnLocation.Z = Origin.Z;

		FRotator SpawnRotation;
		SpawnRotation.Yaw = FMath::FRand() * 360.0f;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;

		ADoppeeCharacter* doppee =
			GetWorld()->SpawnActor<ADoppeeCharacter>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		doppee->setParentWorld(this);
		doppee->setNextPosition(rand_position);
		doppee->swapPosition();
		doppee->setColor();

		doppees.AddUnique(doppee);

		if (my_doppee == nullptr) {
			my_doppee = doppee;
		}
	}

	//InputComponent->BindAction("Jump", IE_Pressed, this, &ADoppelWorld::moveUp);
	/*InputComponent->BindAction("ArrowDown", IE_Pressed, this, &ADoppelWorld::moveArrowDown);
	InputComponent->BindAction("ArrowLeft", IE_Pressed, this, &ADoppelWorld::moveArrowLeft);
	InputComponent->BindAction("ArrowRight", IE_Pressed, this, &ADoppelWorld::moveArrowRight);*/
}

// Called every frame
void ADoppelWorld::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ADoppelWorld::update()
{
	for (auto doppee : doppees) {
		doppee->swapPosition();
	}
}

FVector ADoppelWorld::GetRandomPointInWorld()
{
	auto ori = Box->Bounds.Origin;
	auto ext = Box->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(ori, ext);
}

void ADoppelWorld::moveUp()
{
	move(dirs[e_dirs::UP]);
}

void ADoppelWorld::moveDown()
{
	move(dirs[e_dirs::DOWN]);
}

void ADoppelWorld::moveLeft()
{
	move(dirs[e_dirs::LEFT]);
}

void ADoppelWorld::moveRight()
{
	move(dirs[e_dirs::RIGHT]);
}

void ADoppelWorld::move(POINT dir)
{
	if(Beforedirs.x != dir.x || Beforedirs.y != dir.y)
	{
		for (auto doppee : doppees) {
			if (doppee != my_doppee) {
				POINT Rdir = dirs[FMath::Rand() % e_dirs::COUNT];
				doppee->moveNext(Rdir);
			}
			else
			{
				doppee->moveNext(dir);
			}

			for (POINT p : disable_point)
			{
				if (doppee->getNextPosition().x == p.x && doppee->getNextPosition().y == p.y)
				{
					doppee->setNextPosition(doppee->getPosition());
				}
			}
		}

		Beforedirs.x = dir.x * -1;
		Beforedirs.y = dir.y * -1;
	}
	update();
}
