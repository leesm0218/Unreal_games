// Fill out your copyright notice in the Description page of Project Settings.

#include "Doppel.h"
#include "DoppelWorld.h"
#include "DoppeeCharacter.h"
#include "Tile.h"
#include "Public/StageInfo.h"

#include "kismet/KismetMathLibrary.h"

#include <algorithm>


// Sets default values
ADoppelWorld::ADoppelWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	stage_infos.Add(new StageInfo(10, 10, 10, {
		{ 1, 2, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 2, 1, 1, 1, 1, 1, 2, 1, 1 },
		{ 1, 2, 2, 2, 1, 1, 1, 2, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 2, 1, 1 },
		{ 1, 2, 1, 1, 2, 2, 2, 2, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 2, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 2, 1, 1, 1 },
		{ 1, 2, 2, 2, 2, 2, 2, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 2, 1, 1, 1, 1, 1, 1, 3, 4 },
	}));
}

// Called when the game starts or when spawned
void ADoppelWorld::BeginPlay()
{
	Super::BeginPlay();
	
	init();
}

void ADoppelWorld::init()
{
	auto &now_stage = stage_infos[now_stage_i];

	width = now_stage->width;
	height = now_stage->height;
	num_doppees = now_stage->num_doppees;

	settingTileMap();
	settingDoppees();
	update();
}

void ADoppelWorld::settingTileMap()
{
	for (int i = 0; i < height; i++) {
		tile_map.Add(TArray<ATile*>());

		for (int j = 0; j < width; j++) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnLocation;
			FVector Origin = Box->Bounds.Origin;
			FVector Extent = Box->Bounds.BoxExtent;
			SpawnLocation.X = Origin.X + 2 * Extent.X * j / width - Extent.X;
			SpawnLocation.Y = Origin.Y + 2 * Extent.Y * i / height - Extent.Y;
			SpawnLocation.Z = Origin.Z / 2;

			FRotator SpawnRotation;
			SpawnRotation.Yaw = 0.0f;
			SpawnRotation.Pitch = 0.0f;
			SpawnRotation.Roll = 0.0f;

			ATile* tile = GetWorld()->SpawnActor<ATile>(Tile_BP, SpawnLocation, SpawnRotation, SpawnParams);
			tile->setParentWorld(this);
			tile->setFloor(ATile::e_floors(stage_infos[now_stage_i]->stage_info[j][i]));
			/*if (stage1_tile[i][j] == 2) {
			tile->setFloor(ATile::e_floors(stage1_tile[i][j]));
			}
			else {
			tile->setFloor(ATile::e_floors::T_GROUND);
			}*/

			tile_map[i].Add(tile);
		}
	}
}

void ADoppelWorld::settingDoppees()
{
	for (int i = 0; i < num_doppees; i++) {
		POINT rand_position;

		while (1) {
			bool finded = false;
			rand_position = { FMath::Rand() % width, FMath::Rand() % height };
			if (tile_map[rand_position.y][rand_position.x]->getFloor() == ATile::e_floors::T_GROUND) {
				break;
			}
		}

		FVector Origin = Box->Bounds.Origin;
		FVector Extent = Box->Bounds.BoxExtent;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FRotator SpawnRotation;
		SpawnRotation.Yaw = FMath::FRand() * 360.0f;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;
		float dir[] = { FMath::Cos(SpawnRotation.Yaw), FMath::Sin(SpawnRotation.Yaw) };
		dir[0] = FMath::Sign(dir[0]);
		dir[1] = FMath::Sign(dir[1]);
		FVector SpawnLocation = GetRandomPointInWorld();
		//SpawnLocation.X = Origin.X + 2 * Extent.X * (rand_position.x + dir[0] / 3.0f) / width - Extent.X;
		//SpawnLocation.Y = Origin.Y + 2 * Extent.Y * (rand_position.y + dir[1] / 3.0f) / height - Extent.Y;
		SpawnLocation.X = Origin.X + 2 * Extent.X * rand_position.x / width - Extent.X;
		SpawnLocation.Y = Origin.Y + 2 * Extent.Y * rand_position.y / height - Extent.Y;
		SpawnLocation.Z = Origin.Z;


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
	if (!isNotInput) {
		my_doppee->moveNext(dir);
		startMove();

		for (auto doppee : doppees) {
			if (doppee != my_doppee) {
				int num_arr[] = { 0, 1, 2, 3 };

				std::random_shuffle(num_arr, num_arr + 4);

				for (int i = 0; i < e_dirs::COUNT; i++) {
					POINT rand_dir = dirs[num_arr[i]];

					if (doppee->canMoveDir(rand_dir) && doppee->isEmptyGoundDir(rand_dir)) {
						doppee->moveNext(rand_dir);
						startMove();
						break;
					}
				}
			}
		}

		update();
	}
}


bool ADoppelWorld::canMovePos(POINT target_position)
{
	if (!isBoundery(target_position)) return false;

	auto &t = target_position;

	switch (tile_map[t.y][t.x]->getFloor())
	{
	case ATile::e_floors::T_GROUND:
		return true;
	case ATile::e_floors::T_THORNS:
		return true;
	case ATile::e_floors::T_EXIT:
		return true;
	default:
		return false;
	}
}

bool ADoppelWorld::isEmptyGound(POINT target_position)
{
	auto &t = target_position;

	for (auto &doppee : doppees) {
		auto &curr_pos = doppee->getCurrPosition();
		auto &next_pos = doppee->getNextPosition();

		if (isBoundery(next_pos) && canMovePos(next_pos)) {
			if (next_pos.x == t.x && next_pos.y == t.y) {
				return false;
			}
		}
		else if (curr_pos.x == t.x && curr_pos.y == t.y) {
			return false;
		}
	}

	return true;
}

bool ADoppelWorld::isBoundery(POINT target_position)
{
	if (0 <= target_position.x && target_position.x < width &&
		0 <= target_position.y && target_position.y < height) {
		return true;
	}

	return false;
}