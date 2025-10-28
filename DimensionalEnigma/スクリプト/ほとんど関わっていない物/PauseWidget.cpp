// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"
#include "InGameHUD.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ButtonResumeのOnClickedに「OnButtonResumeClicked」を関連づける
	ButtonReStart->OnClicked.AddUniqueDynamic(this, &UPauseWidget::OnButtonReStartClicked);

	// ButtonTitleのOnClickedに「OnButtonTitleClicked」を関連づける
	ButtonWorldMap->OnClicked.AddUniqueDynamic(this, &UPauseWidget::OnButtonWorldMapClicked);

	// ButtonQuitのOnClickedに「OnButtonQuitClicked」を関連づける
	ButtonTitle->OnClicked.AddUniqueDynamic(this, &UPauseWidget::OnButtonTitleClicked);

	// ButtonQuitのOnClickedに「OnButtonQuitClicked」を関連づける
	ButtonCancel->OnClicked.AddUniqueDynamic(this, &UPauseWidget::OnButtonCancelClicked);

	ButtonEndGame->OnClicked.AddUniqueDynamic(this, &UPauseWidget::OnButtonEndGame);

}

// リスタート
void UPauseWidget::OnButtonReStartClicked() {

	// PlayerControllerを取得する
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// InGameHUDクラスを取得する
	AInGameHUD* HUD = Cast<AInGameHUD>(PlayerController->GetHUD());

	// 現在のレベル名を取得
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);

	// 現在のレベルを読み直す
	HUD->OpenLevel(FName((CurrentLevelName)));
}

// ワールドセレクト
void UPauseWidget::OnButtonWorldMapClicked() 
{
	// PlayerControllerを取得する
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// InGameHUDクラスを取得する
	AInGameHUD* HUD = Cast<AInGameHUD>(PlayerController->GetHUD());

	// レベルを開く
	HUD->OpenLevel(FName(TEXT("WorldSelect")));
}


// タイトル
void UPauseWidget::OnButtonTitleClicked() {
	// PlayerControllerを取得する
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// InGameHUDクラスを取得する
	AInGameHUD* HUD = Cast<AInGameHUD>(PlayerController->GetHUD());

	// Pauseを解除する
	HUD->OpenLevel(FName(TEXT("GameTitle")));

	UKismetSystemLibrary::PrintString(this, TEXT("GameTitle"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}

// キャンセル
void UPauseWidget::OnButtonCancelClicked() {

	UKismetSystemLibrary::PrintString(this, TEXT("Cancel"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

	// PlayerControllerを取得する
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// InGameHUDクラスを取得する
	AInGameHUD* HUD = Cast<AInGameHUD>(PlayerController->GetHUD());

	// Pauseを解除する
	HUD->DispPause(false);
}

void UPauseWidget::OnButtonEndGame()
{
	UWorld* World = GetWorld();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (PlayerController)
	{
		// ゲームを終了（PIE モードの場合はセッション終了）
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
	}
}


void UPauseWidget::SetFocusToButton(UButton* Button)
{
	TSharedPtr<SWidget> SlateWidget = Button->TakeWidget();
	FSlateApplication::Get().SetKeyboardFocus(SlateWidget, EFocusCause::Navigation);

	// 現在のフォーカスボタンを更新
	CurrentFocusedButton = Button;
}