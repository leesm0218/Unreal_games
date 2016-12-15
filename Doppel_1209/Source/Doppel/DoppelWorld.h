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

	void init();
	void settingTileMap();
	void settingDoppees();
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void update();

	UFUNCTION(BlueprintPure, Category = "Doppel")
		FVector GetRandomPointInWorld();

	UFUNCTION(BlueprintCallable, Category = "Doppel")
		void startMove() { isNotInput++; }

	UFUNCTION(BlueprintCallable, Category = "Doppel")
		void endMove() { if (isNotInput > 0) isNotInput--; }

	inline class UBoxComponent* getBox() { return Box; }
	inline TArray<class ADoppeeCharacter*>& getDoppees() { return doppees; }
	inline TArray<TArray<class ATile*>>& getTileMap() { return tile_map; }


	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void move(POINT dir);

	bool canMovePos(POINT target_position);
	bool isEmptyGound(POINT target_position);
	bool isBoundery(POINT target_position);

protected:
	UPROPERTY(EditAnywhere, Category = "Doppel")
		TSubclassOf<class ADoppeeCharacter> WhatToSpawn;

	UPROPERTY(EditAnywhere, Category = "Doppel")
		TSubclassOf<class ATile> Tile_BP;

public:
	/*UPROPERTY(EditAnywhere, Category = "Doppel")
	int width;

	UPROPERTY(EditAnywhere, Category = "Doppel")
	int height;

	UPROPERTY(EditAnywhere, Category = "Doppel")
	int num_doppees;*/

	int width;
	int height;
	int num_doppees;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Doppel", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Box;

	TArray<class ADoppeeCharacter*> doppees;
	class ADoppeeCharacter* my_doppee = nullptr;

	TArray<TArray<class ATile*>> tile_map;

	TArray<class StageInfo*> stage_infos;
	int now_stage_i = 0;

	int isNotInput = 0;
};
