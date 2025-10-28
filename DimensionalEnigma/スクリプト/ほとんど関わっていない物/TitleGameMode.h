// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/SCompoundWidget.h"
#include "TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	void OnLevelChanged(UWorld* LoadedWorld);
	void RemoveTitleScreen();
	ATitleGameMode();

	TSharedPtr<class SMainMenuWidget> MainMenuWidget;
};

