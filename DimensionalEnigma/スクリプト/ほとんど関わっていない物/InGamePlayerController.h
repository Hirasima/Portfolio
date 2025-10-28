// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "inputActionValue.h"
#include "InputAction.h"
#include "InGamePlayerController.generated.h"


class InputAction;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AInGamePlayerController();

protected:
	virtual void BeginPlay()override;

public:
	// APlayerControllerÇ©ÇÁOverrideÇ∑ÇÈ
	virtual void SetupInputComponent() override;

protected:
	// PauseÉÅÉjÉÖÅ[Çï\é¶Ç∑ÇÈ
	void DispPause(const FInputActionValue& _Value);

private:
	void PressedDebugEndGame();
	void PressedDebugGameTitle();
	void PressedDebugRestartInGame();

	void OpenLevel(const FName _LevelName);

	void SetUpInputAction();


private:
	// PauseInputAction
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	// PauseInputAction
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_DebugEndGame;

	// PauseInputAction
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_DebugGameTitle;

	// PauseInputAction
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_DebugRestartInGame;

};
