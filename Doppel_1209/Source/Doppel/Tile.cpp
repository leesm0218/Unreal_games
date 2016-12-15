// Fill out your copyright notice in the Description page of Project Settings.

#include "Doppel.h"
#include "DoppelWorld.h"
#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	auto Box = parent_world->getBox();
	auto Origin = Box->Bounds.Origin;
	auto Extent = Box->Bounds.BoxExtent;
	
	switch (floor_info) {
	case e_floors::T_GROUND:
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation = GetActorLocation();

		FRotator SpawnRotation;
		SpawnRotation.Yaw = 0.0f;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;

		AActor* floor =
			GetWorld()->SpawnActor<AActor>(Floor_BP, SpawnLocation, SpawnRotation, SpawnParams);
		objs.Add(floor);

		break;
	}
	case e_floors::T_PILLAR:
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation = GetActorLocation();

		FRotator SpawnRotation;
		SpawnRotation.Yaw = 0.0f;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;

		AActor* floor =
			GetWorld()->SpawnActor<AActor>(Floor_BP, SpawnLocation, SpawnRotation, SpawnParams);
		auto scale = floor->GetActorScale();
		scale.Z = 2;
		floor->SetActorScale3D(scale);
		objs.Add(floor);

		break;
	}
	case e_floors::T_THORNS:
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation = GetActorLocation();

		FRotator SpawnRotation;
		SpawnRotation.Yaw = 0.0f;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;

		AActor* floor =
			GetWorld()->SpawnActor<AActor>(THORNS_BP, SpawnLocation, SpawnRotation, SpawnParams);
		//auto scale = floor->GetActorScale();
		//scale.Z = 2;
		//floor->SetActorScale3D(scale);
		objs.Add(floor);

		break;
	}
	case e_floors::T_EXIT:
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 100;

		FRotator SpawnRotation;
		SpawnRotation.Yaw = 0.0f;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;

		AActor* floor =
			GetWorld()->SpawnActor<AActor>(EXIT_BP, SpawnLocation, SpawnRotation, SpawnParams);
		//auto scale = floor->GetActorScale();
		//scale.Z = 2;
		//floor->SetActorScale3D(scale);
		objs.Add(floor);

		break;
	}
	default:
		break;
	}

	/*if (wall_info != e_walls::W_EMPTY) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.X += (Box->Bounds.BoxExtent.X / parent_world->width);

		FRotator SpawnRotation;
		SpawnRotation.Yaw = 90.0f;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;

		if ((wall_info & e_walls::W_UP) == e_walls::W_UP) {
			AActor* wall =
				GetWorld()->SpawnActor<AActor>(Wall_BP, SpawnLocation, SpawnRotation, SpawnParams);
			objs.Add(wall);
		}
	}*/
}

// Called every frame
void ATile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATile::setParentWorld(ADoppelWorld * world)
{
	parent_world = world;
}

