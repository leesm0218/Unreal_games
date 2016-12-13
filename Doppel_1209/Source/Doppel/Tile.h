// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class DOPPEL_API ATile : public AActor
{
	GENERATED_BODY()
	
public:
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

public:	
	// Sets default values for this actor's properties
	ATile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Doppel")
		TSubclassOf<class AActor> Wall;

	UPROPERTY(EditAnywhere, Category = "Doppel")
		TSubclassOf<class AActor> Tile;
	
private:
	TArray<TArray<e_tiles>> tile_map;
	TArray<TArray<e_walls>> wall_map;
};
