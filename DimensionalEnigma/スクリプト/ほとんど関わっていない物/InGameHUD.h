// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

public:
	// ポーズ画面の表示/非表示
	void DispPause(const bool IsPause);

	// Levelを開く
	void OpenLevel(const FName LevelName);

private:
	// ポーズ画面のウィジェット
	UUserWidget* PauseWidget;

};
