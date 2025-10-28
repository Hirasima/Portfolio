// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include "EnemyManager.h"
#include "Wire_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ABattleManager::ABattleManager():Ref_EnemyManager(nullptr), m_pTargetEnemy(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////交戦中エネミーの登録関数
	//if (!Ref_EnemyManager) {
	//	UE_LOG(LogTemp, Warning, TEXT("Not Found: Ref_EnemyManager in BattleManager Tick()"));
	//	return;
	//}
	//SetBattleEnemies();

}

//エネミーマネージャをの参照を設定する関数
void ABattleManager::SetEnemyManagerRef(AEnemyManager* _eMng) {
	if (!_eMng) {
		UE_LOG(LogTemp, Warning, TEXT("Irregular Ref:EnemyManager Ref To Set BattleManager"));
	}
	Ref_EnemyManager = _eMng;
}

//プレイヤーの攻撃の起動を感知する関数☆D
void ABattleManager::SetPlayerAttackScence(EAttackPattarnState _pattarn,FVector _PlayerPos,FVector _playerFrontVec, float _AttackRange, float _AttackDeg) {
	for (AEnemy_Base* Enemy : BattleEnemies) {
		if (!Enemy) {
			continue;
		}
		//射程内か調査
		FVector ToEnemy = (Enemy->GetActorLocation()) - _PlayerPos;
		if (ToEnemy.Size() > _AttackRange) {
			continue;
		}
		//角度内か調査
		float Dot = FVector::DotProduct(_playerFrontVec.GetSafeNormal(), ToEnemy.GetSafeNormal());
		float HalfCosAngle = FMath::Cos(FMath::DegreesToRadians(_AttackDeg * 0.5f));
		if (Dot < HalfCosAngle) {
			continue;
		}

		//射程内なので攻撃予備動作を通知
		Enemy->SetScenceOfPlayerAttack(_pattarn);
	}

	//ヒットストップ受容のためスロウ判定をリセット
	IsNowSlow = false;
}

//ターゲット番号を変更する関数★D
void ABattleManager::ChangeTarget(int _targetNum) {
	TargetEnemyNum = _targetNum;
	//ターゲットエネミーを変更
	SetTargetEnemy();
}

//管理対象として登録する関数☆D
void ABattleManager::AddEnemyForRegister(AEnemy_Base* _enemy) {
	if (!_enemy) {
		return;
	}
	if (BattleEnemies.Contains(_enemy)) {
		return;
	}

	//登録
	BattleEnemies.Add(_enemy);
	//対象にアンレジスター用デリゲートを設定
	FUnregisterFromBattleMng Delegate;
	Delegate.BindUObject(this, &ABattleManager::UnregisterBattleEnemy);
	//★フレンドから切り離すならセット関数に切り替え
	_enemy->UnregisterFromBattleMngCallBack = Delegate;
	//ヒットストップ伝達用コールバックを設定
	FCallOfHitStop Calldelegate;
	Calldelegate.BindUObject(this, &ABattleManager::CallOfHitStop);
	_enemy->CallOfHitStopCallBack = Calldelegate;
	//UE_LOG(LogTemp, Warning, TEXT("HitStopCallBackSat!"));

	//エネミーの状況を戦闘に変更
	_enemy->SetEnemySituation(EEnemySituationState::Battle);

}

//エネミーマネージャに交戦中のエネミーがいないか確認する更新関数☆彡こいつ死んでるやん
void ABattleManager::SetBattleEnemies() {
	if (!Ref_EnemyManager) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: Ref_EnemyManager in SetBattleEnemies"));
		return;
	}

	//管理対象として登録
	TArray<AEnemy_Base*> Enemies=Ref_EnemyManager->GetBattleEnemies();
	for (AEnemy_Base* Enemy : Enemies) {
		if (Enemy && !BattleEnemies.Contains(Enemy)) {
			BattleEnemies.Add(Enemy);
			//対象にアンレジスター用デリゲートを設定
			FUnregisterFromBattleMng Delegate;
			Delegate.BindUObject(this, &ABattleManager::UnregisterBattleEnemy);
			//★フレンドから切り離すならセット関数に切り替え
			Enemy->UnregisterFromBattleMngCallBack = Delegate;
			//エネミーの状況を戦闘に変更
			Enemy->SetEnemySituation(EEnemySituationState::Battle);
		}
	}
}


//ターゲットエネミーを設定する関数
void ABattleManager::SetTargetEnemy() {
	//最も近いエネミーの距離
	float NearlestDist =FLT_MAX;
	//最も近いエネミー
	AEnemy_Base* NearlestEnemy = nullptr;

	//★仮
	if (BattleEnemies.Num() <= 0)
	{
		m_pTargetEnemy = nullptr;
		return;
	}

	//★いったん面倒だから最短を返す
	for (AEnemy_Base* Enemy : BattleEnemies) {
		if (!Enemy) {
			continue;
		}
		
		m_pTargetEnemy = Enemy;
	}
}

//ターゲットエネミーを返す関数
AEnemy_Base* ABattleManager::GetTargetEnemy() {
	SetTargetEnemy();
	if (!m_pTargetEnemy) {
		return nullptr;
	}
	return m_pTargetEnemy;
}

//リストから対象エネミーを解放する関数
void ABattleManager::UnregisterBattleEnemy(AEnemy_Base* _deadEnemy) {
	if (!BattleEnemies.Contains(_deadEnemy)) {
		UE_LOG(LogTemp, Warning, TEXT("Irregular Unregist:BattleEnemy Is Not Contain this Enemy"));
		return;
	}
	BattleEnemies.Remove(_deadEnemy);
}

//エネミーからのヒットストップ通知を処理する関数
void ABattleManager::CallOfHitStop() {
	if (IsNowSlow) {
		return;
	}
	IsNowSlow = true;

	//☆彡ここでスロウモーション処理をする//

	float dilationTime = 0.0001;
	float stopTime = 0.15;

	//現在のレベルを取得
	UWorld* world = GetWorld();

	//時間を止める
	UGameplayStatics::SetGlobalTimeDilation(world, dilationTime);


	//タイマーハンドル
	FTimerHandle TimerHandle;

	//タイマーセット
	//ヒットストップ終了時に元の時間の進む速度に戻す
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Hit Stop End"));
		},
		stopTime * dilationTime,
		false
	);
}




