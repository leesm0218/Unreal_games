// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class DOPPEL_API ATile : public AActor
{
	GENERATED_BODY()
	
public:
	enum e_floors {
		T_EMPTY,
		T_GROUND,
		T_PILLAR,
		T_COUNT
	};

	/*enum e_walls {
		W_EMPTY,
		W_UP = 0x1,
		W_RIGHT = 0x2,
		W_DOWN = 0x4,
		W_LEFT = 0x8
	};*/

protected:
	UPROPERTY(EditAnywhere, Category = "Doppel")
		TSubclassOf<class AActor> Floor_BP;

	/*UPROPERTY(EditAnywhere, Category = "Doppel")
		TSubclassOf<class AActor> Wall_BP;*/

public:	
	// Sets default values for this actor's properties
	ATile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void setParentWorld(class ADoppelWorld* world);

	inline void setFloor(e_floors floor) { floor_info = floor; }

	inline e_floors getFloor() { return floor_info; }
private:
	class ADoppelWorld* parent_world;

	e_floors floor_info = e_floors::T_GROUND;
	//e_walls wall_info = e_walls::W_UP;

	TArray<AActor*> objs;
};
