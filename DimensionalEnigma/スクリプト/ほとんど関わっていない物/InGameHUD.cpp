// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"


void AInGameHUD::BeginPlay()
{
	// WidgetBluePrintのクラスを取得
	FString PauseWidgetPath = TEXT("/Game/UI/Pause.Pause_C");
	TSubclassOf<UUserWidget> PauseWidgetClass = TSoftClassPtr<UUserWidget>(FSoftClassPath(*PauseWidgetPath)).LoadSynchronous();
	if (!PauseWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PauseWidget class failed to load"));
		return;
	}

	// PlayerControllerを取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PauseWidgetClass && PlayerController)
	{
		PauseWidget = UWidgetBlueprintLibrary::Create(GetWorld(), PauseWidgetClass, PlayerController);
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
		PauseWidget->AddToViewport(0);
	}

}

void AInGameHUD::DispPause(const bool IsPause) {

	APlayerController* PlayerController = GetOwningPlayerController();

	if (IsPause)
	{
		// Pause画面を表示
		PauseWidget->SetVisibility(ESlateVisibility::Visible);

		// UIモードに設定
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, PauseWidget,EMouseLockMode::DoNotLock, false);

		// GameをPause状態にする
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UE_LOG(LogTemp, Warning, TEXT("Paused"));

		// Mouseカーソルを表示する
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		// Mouseカーソルを表示する
		PlayerController->SetShowMouseCursor(false);

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		UE_LOG(LogTemp, Warning, TEXT("Restart"));

		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, false);

		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);

	}
}

void AInGameHUD::OpenLevel(const FName LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}