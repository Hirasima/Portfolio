// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Wire_GameState.h"
#include "PlayPawn.h"
#include "Wire_GameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyManager::AEnemyManager():
	PlayerPos(FVector::Zero())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
}

//エネミーの初期設定を実行する関数
void AEnemyManager::InitAllEnemies() {
	//エネミーを一括登録//////////////////////////////////////////////////////////////////////////////
	//ワールドを取得
	UWorld* World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: World--But EnemyManager BeginPlay Called"));
		return;
	}
	//ワールド中のエネミーをすべて取得
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AEnemy_Base::StaticClass(), FoundActors);
	//エネミーを登録
	for (AActor* Actor : FoundActors) {
		AEnemy_Base* Enemy = Cast<AEnemy_Base>(Actor);
		if (Enemy) {
			AllEnemies.Add(Enemy);

			//管理対象から解放するデリゲートの登録
			FUnregisterFromEnemyMng Delegate;
			Delegate.BindUObject(this, &AEnemyManager::UnregisterMngEnemy);
			Enemy->UnregisterFromEnemyMngCallBack = Delegate;
			//バトルマネージャに登録するデリゲートを登録
			Enemy->RegisterToBattleMngCallBack = m_BattleManagerRegister;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
}

//エネミーを新規登録する関数
void AEnemyManager::AddNewEnemy(AEnemy_Base* _pEnemy) {
	if (_pEnemy) {
		if (AllEnemies.Contains(_pEnemy)) {
			return;
		}
		AllEnemies.Add(_pEnemy);

		//管理対象から解放するデリゲートの登録
		FUnregisterFromEnemyMng Delegate;
		Delegate.BindUObject(this, &AEnemyManager::UnregisterMngEnemy);
		_pEnemy->UnregisterFromEnemyMngCallBack = Delegate;
		//バトルマネージャに登録するデリゲートを登録
		_pEnemy->RegisterToBattleMngCallBack = m_BattleManagerRegister;
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ゲームステートの取得
	AWire_GameState* G_S = Cast<AWire_GameState>(GetWorld()->GetGameState());

	//プレイヤーの座標を更新
	PlayerPos = GetWorld()->GetGameState<AWire_GameState>()->GetPlayerPos();

	//エネミーの更新処理
	for (int i = AllEnemies.Num() - 1;i >= 0;--i) {
		if (!AllEnemies[i]) {
			return;
		}
		//照明状況の伝達
		AllEnemies[i]->SetIsBlind(!(G_S->GetIslLight()));
		//更新関数
		AllEnemies[i]->EnemyTick(DeltaTime, PlayerPos);
	}

}

//バトルマネージャレジスタ関数をもらう関数
void AEnemyManager::SetBattleManagerRegister(FRegisterFromBattleMng _regis) {
	m_BattleManagerRegister = _regis;
}

//管理対象から解放する関数
void AEnemyManager::UnregisterMngEnemy(AEnemy_Base* _deadEnemy) {
	if (!AllEnemies.Contains(_deadEnemy)) {
		UE_LOG(LogTemp, Warning, TEXT("Irregular Unregist:AllEnemy Is Not Contain this Enemy"));
		return;
	}
	AllEnemies.Remove(_deadEnemy);
}

//警告状態になった事を伝達する関数
void AEnemyManager::BeAlart(bool _Alart,FVector _alartpos) {
	TArray<AEnemy_Base*> BattleEnemies;

	for (AEnemy_Base* Enemy : AllEnemies) {
		if (!Enemy) {
			continue;
		}
		(_Alart) ? Enemy->BeAlarm(_alartpos) : Enemy->EndAlarm();
	}
}

//交戦範囲内のエネミーを返す関数
TArray<AEnemy_Base*> AEnemyManager::GetBattleEnemies() {
	TArray<AEnemy_Base*> BattleEnemies;

	for (AEnemy_Base* Enemy : AllEnemies) {
		if (!Enemy) {
			continue;
		}
		//★これ全員ぶんやるのクッソ無駄に重い
		if (FVector::Dist(PlayerPos, Enemy->GetActorLocation()) > Enemy->GetViewLange()) {
			continue;
		}
		BattleEnemies.Add(Enemy);
	}
	return BattleEnemies;
}

//n番目に近いエネミーを返す関数★バトルマネージャに移行
AEnemy_Base* AEnemyManager::GetEnemyByDistNum(int _DistNum) {
	//最も近いエネミーの距離
	float NearlestDist = 100000;
	//最も近いエネミー
	AEnemy_Base* NearlestEnemy = nullptr;

	//★いったん面倒だから最短を返す
	for (AEnemy_Base* Enemy : AllEnemies) {
		if (!Enemy) {
			continue;
		}

		if (FVector::Dist(PlayerPos, Enemy->GetActorLocation()) >= NearlestDist) {
			continue;
		}
		NearlestEnemy = Enemy;
	}
	return NearlestEnemy;
}

//ターゲットエネミーを返す関数 //平
//
//探索距離内に存在しない場合はnullptrを返す
//探索距離内に前のターゲット (_previousTarget) しか存在しなければ_previousTargetを返す
AEnemy_Base* AEnemyManager::GetTargetEnemy(float _searchDistance, FVector _searcherRightVector, FVector _searcherPostion, float _nextTargetDirection, AEnemy_Base* _previousTarget)
{
	//探索距離内の敵
	TArray<AEnemy_Base*> enemysWithinRange;

	//探索距離内の敵の距離
	TArray<float> enemysDistance;

	//探索距離内の敵の外積結果
	TArray<float> enemysClossResult;

	//最も探索方向に近い敵
	AEnemy_Base* returnEnemy = _previousTarget;

	//右側で最も近い敵
	AEnemy_Base* rightEnemy = nullptr;

	//左側で最も近い敵
	AEnemy_Base* leftEnemy = nullptr;

	//暫定的結果の敵の距離
	float rightDistance = FLT_MAX;
	float leftDistance = FLT_MAX;

	//線形探索で探索距離内の敵を探索
	for (int i = 0; i < AllEnemies.Num(); ++i)
	{
		//もしi番目の敵が有効でなければ次のループへ
		if (!AllEnemies[i]) continue;

		//探索者から見た敵の距離
		float dis = FVector::Dist(AllEnemies[i]->GetActorLocation(), _searcherPostion);

		//索敵距離内に存在していれば
		if (dis <= _searchDistance)
		{
			//探索距離内の敵に追加
			enemysWithinRange.Add(AllEnemies[i]);
			enemysDistance.Add(dis);

			//クロス積(2D)で左右を判定する

			//探索者から見た敵の位置
			FVector location = AllEnemies[i]->GetActorLocation();

			//クロス積
			float cross = FVector2D::CrossProduct(FVector2D(location.X, location.Y), FVector2D(_searcherPostion.X, _searcherPostion.Y));
			enemysClossResult.Add(cross);
		}
	}

	//探索距離内の敵がいなければ
	if (enemysWithinRange.Num() <= 0)
	{
		//ログを残し、ヌルを返して終了
		UE_LOG(LogTemp, Log, TEXT("EnemyManager : There Are No Enemies Within Search Distance"));
		return nullptr;
	}

	//線形探索で探索距離内の敵の中で最も近い敵を探索
	for (int i = 0; i < enemysWithinRange.Num(); ++i)
	{
		//以前のターゲットであれば探索から弾き次のループへ
		if (_previousTarget && enemysWithinRange[i] == _previousTarget) continue;

		//もしも右側なら
		if (enemysClossResult[i] <= 0)
		{
			//距離がより近ければ
			if (enemysDistance[i] < rightDistance)
			{
				//暫定結果を更新
				rightEnemy = enemysWithinRange[i];
				rightDistance = enemysDistance[i];
			}
		}
		//もし左側なら
		else
		{
			//距離がより近ければ
			if (enemysDistance[i] < leftDistance)
			{
				//暫定結果を更新
				leftEnemy = enemysWithinRange[i];
				leftDistance = enemysDistance[i];
			}
		}
	}

	//次のターゲット方向が正（右側）なら
	if (_nextTargetDirection >= 0)
	{
		//右側の敵を優先
		if (rightEnemy)
		{
			returnEnemy = rightEnemy;
		}
		else
		{
			returnEnemy = leftEnemy;
		}
	}
	//次のターゲット方向が負（左側）なら
	else
	{
		//左側の敵を優先
		if (leftEnemy)
		{
			returnEnemy = leftEnemy;
		}
		else
		{
			returnEnemy = rightEnemy;
		}
	}

	//最終的に最も条件に近い結果を返す
	return returnEnemy;
}

