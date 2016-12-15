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

void ADoppeeCharacter::moveNext(POINT dir)
{
	next_position = { curr_position.x + dir.x, curr_position.y + dir.y };
}

bool ADoppeeCharacter::canMoveDir(POINT dir)
{
	POINT target_porition = { curr_position.x + dir.x, curr_position.y + dir.y };

	return parent_world->canMovePos(target_porition);
}

bool ADoppeeCharacter::canMoveNext()
{
	return parent_world->canMovePos(next_position);
}

bool ADoppeeCharacter::isEmptyGoundDir(POINT dir)
{
	POINT target_porition = { curr_position.x + dir.x, curr_position.y + dir.y };

	return parent_world->isEmptyGound(target_porition);
}

void ADoppeeCharacter::swapPosition()
{
	FVector SpawnLocation = parent_world->GetRandomPointInWorld();
	auto Box = parent_world->getBox();
	auto &tile_map = parent_world->getTileMap();
	auto width = parent_world->width;
	auto height = parent_world->height;
	auto dir = POINT{ next_position.x - curr_position.x, next_position.y - curr_position.y };
	dir.x = FMath::Sign(dir.x);
	dir.y = FMath::Sign(dir.y);
	FVector Origin = Box->Bounds.Origin;
	FVector Extent = Box->Bounds.BoxExtent;
	FVector doppee_extent = this->GetComponentsBoundingBox().GetExtent();
	SpawnLocation.X = Origin.X + 2 * Extent.X * (next_position.x) / width - Extent.X + dir.x * doppee_extent.X / 2;
	SpawnLocation.Y = Origin.Y + 2 * Extent.Y * (next_position.y) / height - Extent.Y + dir.y * doppee_extent.Y / 2;
	//SpawnLocation.X = Origin.X + 2 * Extent.X * next_position.x / width - Extent.X;
	//SpawnLocation.Y = Origin.Y + 2 * Extent.Y * next_position.y / height - Extent.Y;
	SpawnLocation.Z = Origin.Z;

	targetLocation = SpawnLocation;

	if (parent_world->canMovePos(next_position)) {
		Move();
		curr_position = next_position;
	}
	else {
		Turn();
	}

	//FRotator rot = this->GetControlRotation();
	//const FVector dir = FRotationMatrix(rot).GetScaledAxis(EAxis::X);

	//AddMovementInput({ 1.0, 0.0, 0.0 }, 1.0);
}

