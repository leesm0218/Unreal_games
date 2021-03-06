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
	void moveNext(POINT dir);
	bool canMoveDir(POINT dir);
	bool canMoveNext();
	bool isEmptyGoundDir(POINT dir);

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Doppel")
		void Die();

	UFUNCTION(BlueprintImplementableEvent, Category = "Doppel")
		void Die2();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Doppel", meta = (AllowPrivateAccess = "true"))
		class ADoppelWorld* parent_world;

private:
	POINT curr_position = { -1, -1 };
	POINT next_position = { -1, -1 };

	UAnimationAsset *idle_anim, *walk_anim;
};
