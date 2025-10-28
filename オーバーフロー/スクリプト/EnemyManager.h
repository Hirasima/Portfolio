// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_Base.h"
#include "EnemyManager.generated.h"

//クラスの前方宣言
class AEnemy_Base;
class APlayPawn;

UCLASS()
class ORIGINALACTION_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//管理対象から解放する関数
	void UnregisterMngEnemy(AEnemy_Base* _deadEnemy);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//交戦範囲内のエネミーを返す関数
	TArray<AEnemy_Base*> GetBattleEnemies();

	//n番目に近いエネミーを返す関数
	AEnemy_Base* GetEnemyByDistNum(int _DistNum);

	//バトルマネージャレジスタ関数をもらう関数
	void SetBattleManagerRegister(FRegisterFromBattleMng _regis);

	//エネミーの初期設定を実行する関数
	void InitAllEnemies();

	//警告状態になった事を伝達する関数
	void BeAlart(bool _Alart, FVector _alartpos = FVector::Zero());

	//エネミーを新規登録する関数
	void AddNewEnemy(AEnemy_Base* _pEnemy);

	//ターゲットエネミーを返す関数
	//
	//探索距離内に存在しない場合はnullptrを返す
	//探索距離内に前のターゲット (_previousTarget) しか存在しなければ_previousTargetを返す
	AEnemy_Base* GetTargetEnemy(float _searchDistance, FVector _searcherRightVector, FVector _searcherPostion, float _nextTargetDirection = 0.0f, AEnemy_Base* _previousTarget = nullptr);


protected:
	//管理対象のエネミー群
	UPROPERTY()
	TArray<AEnemy_Base*>AllEnemies;

	//プレイヤーの現在座標
	FVector PlayerPos;

	//バトルマネージャに登録するための関数デリゲート（エネミーに伝達したいので）
	FRegisterFromBattleMng m_BattleManagerRegister;

};


