// Fill out your copyright notice in the Description page of Project Settings.

#include "Doppel.h"
#include "DoppeeCharacter.h"
#include "DoppelWorld.h"


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

void ADoppeeCharacter::swapPosition()
{
	FVector SpawnLocation = parent_world->GetRandomPointInWorld();
	auto Box = parent_world->getBox();
	auto width = parent_world->width;
	auto height = parent_world->height;

	if (curr_position.x != next_position.x || curr_position.y != next_position.y)
	{
		if (0 <= next_position.x && next_position.x < width &&
			0 <= next_position.y && next_position.y < height) {
			FVector Origin = Box->Bounds.Origin;
			FVector Extent = Box->Bounds.BoxExtent;
			SpawnLocation.X = Origin.X + 2 * Extent.X * next_position.x / width - Extent.X;
			SpawnLocation.Y = Origin.Y + 2 * Extent.Y * next_position.y / height - Extent.Y;
			SpawnLocation.Z = Origin.Z;

			targetLocation = SpawnLocation;
		

			Move();
			curr_position = next_position;
		}
		//GetMesh()->PlayAnimation(walk_anim, true);

		//SetActorLocation(SpawnLocation);

	}
	
	//FRotator rot = this->GetControlRotation();
	//const FVector dir = FRotationMatrix(rot).GetScaledAxis(EAxis::X);

	//AddMovementInput({ 1.0, 0.0, 0.0 }, 1.0);

}

