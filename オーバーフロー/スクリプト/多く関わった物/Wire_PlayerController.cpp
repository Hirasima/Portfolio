// Fill out your copyright notice in the Description page of Project Settings.

#include "InputMappingContext.h"
#include "Wire_PlayerController.h"
#include "Wire_PCManager.h"
#include "EnhancedInputSubsystems.h"

AWire_PlayerController::AWire_PlayerController()
{
	//PlayerCameraManagerを設定
	PlayerCameraManagerClass = m_PlayerCameraManagerClass;
}

void AWire_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 入力関係初期化 ////////////////////////////////////////////////////////////

	//ローカルプレイヤー
	ULocalPlayer* localPlayer = nullptr;
	//入力サブシステム
	UEnhancedInputLocalPlayerSubsystem* enhancedInputLocalPlayerSubSystem = nullptr;

	//ローカルプレイヤーを取得
	localPlayer = GetLocalPlayer();

	//ローカルプレイヤーが有効でなければ終了
	if (!localPlayer) return;

	//入力サブシステムを取得
	enhancedInputLocalPlayerSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);

	//入力サブシステムが有効でなければ終了
	if (!localPlayer) return;
	//マッピングコンテキストが有効でなければ終了
	if (!DefaultInputMappingContext) return;

	//入力サブシステムにマッピングコンテキストを追加
	enhancedInputLocalPlayerSubSystem->AddMappingContext(DefaultInputMappingContext, 0);

	UE_LOG(LogTemp, Log, TEXT("PlayerController : Input Initializing Complete"));

	// カメラ関係初期化 ////////////////////////////////////////////////////////////////

	//デフォルトポーン
	APawn* defaultPawn = nullptr;
	//ポーン取得
	defaultPawn = GetPawn();

	//ポーンが有効でなければ終了
	if (!defaultPawn)return;

	//ターゲットビューを設定
	SetViewTarget(defaultPawn);

	UE_LOG(LogTemp, Log, TEXT("PlayerControlller : ViewTarget Initializing Complete"));
}