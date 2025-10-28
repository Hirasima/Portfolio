// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Wire_GameMode.generated.h"

//クラスの前方宣言
class AEnemyManager;
class ABattleManager;


UCLASS()
class ORIGINALACTION_API AWire_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	//コンストラクタ
	AWire_GameMode();
private:
	//ゲーム開始時処理
	virtual void BeginPlay() override;
public:
	//エネミーマネージャを返す関数
	AEnemyManager* GetEnemyManager();
	//バトルマネージャを返す関数
	ABattleManager* GetBattleManager();

private:
	UPROPERTY()
	AEnemyManager* m_EnemyManager;
	ABattleManager* m_BattleManager;
};
