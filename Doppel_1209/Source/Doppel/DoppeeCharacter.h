// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DoppeeCharacter.generated.h"

UCLASS()
class DOPPEL_API ADoppeeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoppeeCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void setParentWorld(class ADoppelWorld* world);

	inline POINT& getCurrPosition() { return curr_position; }
	inline POINT& getNextPosition() { return next_position; }

	inline void setNextPosition(POINT position) { next_position = position; }
	inline void moveNext(POINT dir) { next_position = { curr_position.x + dir.x, curr_position.y + dir.y }; }
	bool canMoveDir(POINT dir);
	bool canMoveNext();
	bool isEmptyGoundDir(POINT dir);

	bool canMoveNext(POINT target_position);
	bool isEmptyGound(POINT target_position);
	bool isBoundery(POINT target_position);

	void swapPosition();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Doppel", meta = (AllowPrivateAccess = "true"))
		FVector targetLocation;

	UFUNCTION(BlueprintImplementableEvent, Category = "Doppel")
		void setColor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Doppel")
		void Move();

	UFUNCTION(BlueprintImplementableEvent, Category = "Doppel")
		void Turn();
	
private:
	POINT curr_position = { -1, -1 };
	POINT next_position = { -1, -1 };

	UAnimationAsset *idle_anim, *walk_anim;
	class ADoppelWorld* parent_world;
};
