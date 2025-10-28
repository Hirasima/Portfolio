// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_Base.h" 
#include "BattleManager.generated.h"

//クラスの前方宣言
class AEnemyManager;
class AEnemy_Base;

UCLASS()
class ORIGINALACTION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//エネミーマネージャに交戦中のエネミーがいないか確認する更新関数
	void SetBattleEnemies();

	//ターゲットエネミーを設定する関数
	void SetTargetEnemy();

	//リストから対象エネミーを解放する関数
	void UnregisterBattleEnemy(AEnemy_Base* _deadEnemy);

	//エネミーからのヒットストップ通知を処理する関数
	void CallOfHitStop();

protected:
	UPROPERTY()
	AEnemyManager* Ref_EnemyManager;
	UPROPERTY()
	AEnemy_Base* m_pTargetEnemy;

	int TargetEnemyNum = 0;

	bool IsNowSlow = false;

	


public:		
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//エネミーマネージャを設定する関数
	void SetEnemyManagerRef(AEnemyManager* _eMng);

	//管理対象として登録する関数
	void AddEnemyForRegister(AEnemy_Base* _enemy);

	//ターゲット番号を変更する関数
	void ChangeTarget(int _targetNum);

	//ターゲットエネミーを返す関数
	AEnemy_Base* GetTargetEnemy();
	

	//プレイヤーの攻撃の起動を感知する関数
	void SetPlayerAttackScence(EAttackPattarnState _pattarn, FVector _PlayerPos, FVector _playerFrontVec, float _AttackRange, float _AttackDeg);

	//管理対象のエネミー群★仮
	UPROPERTY(EditAnywhere)
	TArray<AEnemy_Base*> BattleEnemies;
};
