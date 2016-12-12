// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DoppelWorld.generated.h"

UCLASS()
class DOPPEL_API ADoppelWorld : public AActor
{
	GENERATED_BODY()

public:
	enum e_dirs {
		UP, DOWN, LEFT, RIGHT,
		COUNT
	};

	enum e_tiles {
		T_EMPTY,
		T_GROUND,
		T_COUNT
	};

	enum e_walls {
		W_EMPTY,
		W_UP = 0x1,
		W_RIGHT = 0x2,
		W_DOWN = 0x4,
		W_LEFT = 0x8
	};

	const POINT dirs[4] = {
		{ 1, 0 },
		{ -1, 0 },
		{ 0, -1 },
		{ 0, 1 }
	};

public:	
	// Sets default values for this actor's properties
	ADoppelWorld();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void update();

	UFUNCTION(BlueprintPure, Category = "Doppel")
	FVector GetRandomPointInWorld();
	inline class UBoxComponent* getBox() { return Box; }


	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void move(POINT dir);

protected:
	UPROPERTY(EditAnywhere, Category = "Doppel")
	TSubclassOf<class ADoppeeCharacter> WhatToSpawn;

	UPROPERTY(EditAnywhere, Category = "Doppel")
	TSubclassOf<class AActor> Wall;

	UPROPERTY(EditAnywhere, Category = "Doppel")
	TSubclassOf<class AActor> Tile;

public:
	UPROPERTY(EditAnywhere, Category = "Doppel")
	int width;

	UPROPERTY(EditAnywhere, Category = "Doppel")
	int height;

	UPROPERTY(EditAnywhere, Category = "Doppel")
	int num_doppees;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Doppel", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Box;

	TArray<class ADoppeeCharacter*> doppees;
	class ADoppeeCharacter* my_doppee = nullptr;

	TArray<TArray<int>> tile_map;
	TArray<TArray<int>> wall_map;
};
