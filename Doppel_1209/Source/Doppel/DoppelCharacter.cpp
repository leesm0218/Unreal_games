// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Doppel.h"
#include "DoppelCharacter.h"
#include "DoppelWorld.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

ADoppelCharacter::ADoppelCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

										  // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADoppelCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoppelWorld::StaticClass(), FoundActors);

	for (auto Actor : FoundActors) {
		auto* doppel_world = Cast<ADoppelWorld>(Actor);

		if (doppel_world) {
			doppel_worlds.AddUnique(doppel_world);
		}
	}
}

void ADoppelCharacter::Tick(float DeltaSeconds)
{
	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params;
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void ADoppelCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAction("Up", IE_Pressed, this, &ADoppelCharacter::ArrowUp);
	InputComponent->BindAction("Down", IE_Pressed, this, &ADoppelCharacter::ArrowDown);
	InputComponent->BindAction("Left", IE_Pressed, this, &ADoppelCharacter::ArrowLeft);
	InputComponent->BindAction("Right", IE_Pressed, this, &ADoppelCharacter::ArrowRight);
	InputComponent->BindAction("Restart", IE_Pressed, this, &ADoppelCharacter::Restart);
}

void ADoppelCharacter::ArrowUp()
{
	for (auto doppel_world : doppel_worlds) {
		if (doppel_world) {
			doppel_world->moveUp();
		}
	}
}

void ADoppelCharacter::ArrowDown()
{
	for (auto doppel_world : doppel_worlds) {
		if (doppel_world) {
			doppel_world->moveDown();
		}
	}
}

void ADoppelCharacter::ArrowLeft()
{
	for (auto doppel_world : doppel_worlds) {
		if (doppel_world) {
			doppel_world->moveLeft();
		}
	}
}

void ADoppelCharacter::ArrowRight()
{
	for (auto doppel_world : doppel_worlds) {
		if (doppel_world) {
			doppel_world->moveRight();
		}
	}
}

void ADoppelCharacter::Restart()
{
	for (auto doppel_world : doppel_worlds) {
		if (doppel_world) {
			doppel_world->init();
		}
	}
}
