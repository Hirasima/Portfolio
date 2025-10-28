// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalTile.generated.h"

UENUM()
enum class ETransitionMode : uint8
{
	NextLevel = 0,
	ReturnSelect = 1,
};

UCLASS()
class AGoalTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsGoal(float DeltaTime);

private:
	void LoadNextLevel();

	void DebugNextLevel();

	bool m_bGoal;

	UPROPERTY(EditAnywhere, Category = "TransitionMode")
	ETransitionMode m_TransitionMode;

	UPROPERTY(EditAnywhere, Category = "Level")
	FName m_LevelName;

	//ƒŠƒUƒ‹ƒg‚ÌUI
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> m_resultUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> m_clearTimeUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> m_playerMoveUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> m_cameraMoveUI;*/

	float m_GoalDelayTime;
};
