// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonReStart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonWorldMap;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonCancel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonEndGame;


	// 現在選択しているボタン
	UButton* CurrentFocusedButton;

protected:
	// NativeConstruct
	void NativeConstruct() override;

private:
	// ButtonReStartのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonReStartClicked();

	// ButtonTitleのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonWorldMapClicked();

	// ButtonTitleのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonTitleClicked();

	// ButtonQuitのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonCancelClicked();

	// ButtonQuitのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonEndGame();


	void SetFocusToButton(UButton* Button);

	void OnNavigateUp();
	void OnNavigateDown();


};
