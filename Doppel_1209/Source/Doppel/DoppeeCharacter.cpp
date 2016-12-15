// Fill out your copyright notice in the Description page of Project Settings.

#include "Doppel.h"
#include "DoppeeCharacter.h"
#include "DoppelWorld.h"
#include "Tile.h"


// Sets default values
ADoppeeCharacter::ADoppeeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UAnimationAsset>
	//	idle_asset(TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonIdle.ThirdPersonIdle'"));

	//static ConstructorHelpers::FObjectFinder<UAnimationAsset>
	//	walk_asset(TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonIdle.ThirdPersonWalk_Start'"));

	//idle_anim = idle_asset.Object;
	//walk_anim = walk_asset.Object;
}

// Called when the game starts or when spawned
void ADoppeeCharacter::BeginPlay()
{
	Super::BeginPlay();

	//GetMesh()->PlayAnimation(idle_anim, true);
}

// Called every frame
void ADoppeeCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

// Called to bind functionality to input
void ADoppeeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ADoppeeCharacter::setParentWorld(ADoppelWorld* world)
{
	parent_world = world;
}

bool ADoppeeCharacter::canMoveDir(POINT dir)
{
	POINT target_porition = { curr_position.x + dir.x, curr_position.y + dir.y };

	return canMoveNext(target_porition);
}

bool ADoppeeCharacter::canMoveNext()
{
	return canMoveNext(next_position);
}

bool ADoppeeCharacter::isEmptyGoundDir(POINT dir)
{
	POINT target_porition = { curr_position.x + dir.x, curr_position.y + dir.y };

	return isEmptyGound(target_porition);
}

bool ADoppeeCharacter::canMoveNext(POINT target_position)
{
	if (!isBoundery(target_position)) return false;

	auto &tile_map = parent_world->getTileMap();
	auto &t = target_position;

	switch (tile_map[t.y][t.x]->getFloor())
	{
	case ATile::e_floors::T_GROUND:
		return true;
	default:
		return false;
	}
}

bool ADoppeeCharacter::isEmptyGound(POINT target_position)
{
	auto &t = target_position;
	auto &doppees = parent_world->getDoppees();

	for (auto &doppee : doppees) {
		auto &curr_pos = doppee->getCurrPosition();
		auto &next_pos = doppee->getNextPosition();

		if ((next_pos.x != t.x || next_pos.y != t.y)) {
			// keep loop
		}
		else {
			return false;
		}
	}

	return true;
}

bool ADoppeeCharacter::isBoundery(POINT target_position)
{
	auto &width = parent_world->width;
	auto &height = parent_world->height;

	if (0 <= target_position.x && target_position.x < width &&
		0 <= target_position.y && target_position.y < height) {
		return true;
	}

	return false;
}

void ADoppeeCharacter::swapPosition()
{
	FVector SpawnLocation = parent_world->GetRandomPointInWorld();
	auto Box = parent_world->getBox();
	auto &tile_map = parent_world->getTileMap();
	auto width = parent_world->width;
	auto height = parent_world->height;

	if (canMoveNext(next_position)) {
		FVector Origin = Box->Bounds.Origin;
		FVector Extent = Box->Bounds.BoxExtent;
		SpawnLocation.X = Origin.X + 2 * Extent.X * next_position.x / width - Extent.X;
		SpawnLocation.Y = Origin.Y + 2 * Extent.Y * next_position.y / height - Extent.Y;
		SpawnLocation.Z = Origin.Z;

		targetLocation = SpawnLocation;

		Move();
		curr_position = next_position;
	}
	else {
		FVector Origin = Box->Bounds.Origin;
		FVector Extent = Box->Bounds.BoxExtent;
		SpawnLocation.X = Origin.X + 2 * Extent.X * next_position.x / width - Extent.X;
		SpawnLocation.Y = Origin.Y + 2 * Extent.Y * next_position.y / height - Extent.Y;
		SpawnLocation.Z = Origin.Z;

		targetLocation = SpawnLocation;

		Turn();
	}
	
	//FRotator rot = this->GetControlRotation();
	//const FVector dir = FRotationMatrix(rot).GetScaledAxis(EAxis::X);

	//AddMovementInput({ 1.0, 0.0, 0.0 }, 1.0);

}

