// Fill out your copyright notice in the Description page of Project Settings.


#include "Wire_GameMode.h"
#include"EnemyManager.h"
#include"BattleManager.h"
#include "PlayPawn.h"
#include "Wire_GameState.h"
#include "Wire_HUD.h"
#include "Kismet/GameplayStatics.h"

//コンストラクタ
AWire_GameMode::AWire_GameMode():m_EnemyManager(nullptr),m_BattleManager(nullptr) {


}
//ゲーム開始時処理
void AWire_GameMode::BeginPlay() {
	Super::BeginPlay();

	//ワールドを取得
	UWorld* World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp,Warning, TEXT("Not Found: World--But GameMode BeginPlay Called"));
		return;
	}
	//エネミーマネージャの合成
	m_EnemyManager = World->SpawnActor<AEnemyManager>(AEnemyManager::StaticClass());
	//バトルマネージャの合成
	m_BattleManager = World->SpawnActor<ABattleManager>(ABattleManager::StaticClass());

	//バトルマネージャにエネミーマネージャの参照を渡す
	m_BattleManager->SetEnemyManagerRef(m_EnemyManager);

	//プレイヤーPawnを取得（Super::BeginPlayで生成済み）////////////////////////////////////////////////////////////////
	APawn* P_Pawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!P_Pawn) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: PlayerPawn--But GameMode BegnPlay Called"));
		return;
	}
	APlayPawn* Player = Cast<APlayPawn>(P_Pawn);
	if (!Player) {
		UE_LOG(LogTemp, Warning, TEXT("Faild Cast: PlayerPawn to APlayPawn"));
		return;
	}

	//HUDを取得
	AHUD* hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	if (!hud) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HUD--But GameMode BegnPlay Called"));
		return;
	}
	AWire_HUD* WireHud = Cast<AWire_HUD>(hud);
	if (!WireHud) {
		UE_LOG(LogTemp, Warning, TEXT("Faild Cast: HUD to AWire_HUD"));
		return;
	}
	//プレイヤーにHUDを伝達
	Player->SetHUDtoPlayer(WireHud);

	//バトルマネージャのプレイヤー予備動作探知関数をプレイヤーに伝達
	FReceivePlayerAttackScence ScenceDelegate;
	ScenceDelegate.BindUObject(m_BattleManager, &ABattleManager::SetPlayerAttackScence);
	Player->SetAttackScenceReceiver(ScenceDelegate);

	//バトルマネージャのターゲットエネミー取得関数をプレイヤーに伝達
	FGetTargetEnemy TargetDelegate;
	TargetDelegate.BindUObject(m_EnemyManager, &AEnemyManager::GetTargetEnemy);
	Player->SetTargetEnemyDelegate(TargetDelegate);

	//エネミーマネージャにバトルマネージャに登録する関数を伝達★
	FRegisterFromBattleMng RegistDelegate;
	RegistDelegate.BindUObject(m_BattleManager, &ABattleManager::AddEnemyForRegister);
	m_EnemyManager->SetBattleManagerRegister(RegistDelegate);

	//エネミーを初期化
	m_EnemyManager->InitAllEnemies();

	//ゲームステートにプレイヤーの座標取得関数をデリゲートとして登録/////////////////////////////////////////////////
	//ゲームステートの取得
	AWire_GameState* G_S = GetWorld()->GetGameState<AWire_GameState>();
	if (!G_S) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: GameMode--But GameMode BegnPlay Called"));
		return;
	}
	//デリゲートにプレイヤー座標取得関数をバインド
	FGetPlayerPosDelegate PLDelegate;
	PLDelegate.BindUObject(Player, &APawn::GetActorLocation);

	//ゲームステートに登録
	G_S->SetPlayerPosGetter(PLDelegate);
}


//エネミーマネージャを返す関数
AEnemyManager* AWire_GameMode::GetEnemyManager() {
	if (!m_EnemyManager) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: EnemyManager--But GetEnemyManager Called"));
		return nullptr;
	}

	return m_EnemyManager;
}
//バトルマネージャを返す関数
ABattleManager* AWire_GameMode::GetBattleManager() {
	if (!m_BattleManager) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: BattleManager--But GetBattleManager Called"));
		return nullptr;
	}

	return m_BattleManager;
}

