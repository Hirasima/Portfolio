// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StartOrderBGM.generated.h"

UCLASS()
class AStartOrderBGM : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStartOrderBGM();

	UPROPERTY(EditANywhere, Category = "BGM_Path")
	FString BGM_Path = TEXT("/Game/Sound/BGM_Breath.BGM_Breath");//デフォルトの値としてとりあえず設定

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
