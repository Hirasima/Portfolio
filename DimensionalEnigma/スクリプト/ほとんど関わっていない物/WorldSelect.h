// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnigmaGameInstance.h"
#include "GameManager.h"
#include "StageSubSystem.h"

#include "WorldSelect.generated.h"

UCLASS()
class AWorldSelect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldSelect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector GetSelectLocation();
	FVector SelectLocation;

private:
	void SetNextWorld();

	void LoadNextWorld();

	void DebugNextWorld();

	UPROPERTY(EditAnywhere, Category = "World")
	FName m_WorldName;

	UPROPERTY(EditAnywhere, Category = "World")
	FKey m_WorldNumber;

private:

	UWorld* world;
	UEnigmaGameInstance* gameinstance;
	UStageSubSystem* gamemanager;

};
