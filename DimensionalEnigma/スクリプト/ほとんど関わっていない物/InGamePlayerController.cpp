// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "StageSubSystem.h"

AInGamePlayerController::AInGamePlayerController()
{
	// InputActionを読み込む
	PauseAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/EnhancedInput/Input/IA_Start"));

	// Debug用のInputを取得
	m_DebugEndGame = LoadObject<UInputAction>(nullptr, TEXT("/Game/EnhancedInput/Input/IA_DebugEndGame"));
	m_DebugGameTitle = LoadObject<UInputAction>(nullptr, TEXT("/Game/EnhancedInput/Input/IA_DebugEndGame"));
	m_DebugRestartInGame = LoadObject<UInputAction>(nullptr, TEXT("/Game/EnhancedInput/Input/IA_DebugEndGame"));

	void SetUpInputAction();

}

void AInGamePlayerController::BeginPlay()
{
	// 入力をGameのみにする
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this, false);

	// マウスを非表示にする
	SetShowMouseCursor(false);
}

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// バインド設定
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AInGamePlayerController::DispPause);

		// デバッグ用のキー
		EnhancedInputComponent->BindAction(m_DebugEndGame, ETriggerEvent::Triggered, this, &AInGamePlayerController::PressedDebugEndGame);
		EnhancedInputComponent->BindAction(m_DebugGameTitle, ETriggerEvent::Triggered, this, &AInGamePlayerController::PressedDebugGameTitle);
		EnhancedInputComponent->BindAction(m_DebugRestartInGame, ETriggerEvent::Triggered, this, &AInGamePlayerController::PressedDebugRestartInGame);

	}
}

// ポーズ画面の表示
void AInGamePlayerController::DispPause(const FInputActionValue& Value)
{
	if (bool V = Value.Get<bool>())
	{
		// GetWorldからStageSubsystemを取得
		if (UWorld* World = GetWorld())
		{
			UStageSubSystem* StageManager = World->GetSubsystem<UStageSubSystem>();

			// ゴールしていたら表示できないように
			if (!StageManager->GetGoaling())
			{
				if (AInGameHUD* HUD = Cast<AInGameHUD>(GetHUD()))
				{
					// Pause画面の表示非表示を実行する
					HUD->DispPause(!UGameplayStatics::IsGamePaused(GetWorld()));
				}
			}
		}

	}
}

// ゲームを終了させるデバッグキー（Escキー）
void AInGamePlayerController::PressedDebugEndGame()
{
	// GetWorldからコントローラーを取得
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* PlayerController = world->GetFirstPlayerController();

		// nullチェックをし
		if (PlayerController)
		{
			// ゲームを終了
			UKismetSystemLibrary::QuitGame(world, PlayerController, EQuitPreference::Quit, false);
		}
	}
}

// タイトルに戻るデバッグキー（F1）
void AInGamePlayerController::PressedDebugGameTitle()
{
	UKismetSystemLibrary::PrintString(this, TEXT("DebugGameTitle Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));
	OpenLevel(FName(TEXT("WorldSelect")));
}

// 現在のレベルを再スタートするデバッグキー(F2)
void AInGamePlayerController::PressedDebugRestartInGame()
{
	UKismetSystemLibrary::PrintString(this, TEXT("DebugRestartInGame Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

	// 現在のレベル名を取得
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	OpenLevel(FName(CurrentLevelName));
}

// レベルを開く関数
void AInGamePlayerController::OpenLevel(FName _LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), _LevelName);
}

