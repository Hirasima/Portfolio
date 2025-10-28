// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameTickable.h"
#include "EnemyManager_V2.generated.h"


//クラスの前方宣言
class AEnemyBase_V2;

UCLASS()
class ORIGINALACTION_API UEnemyManager_V2 : public UObject,public IGameTickable
{
	GENERATED_BODY()

	public:

	//更新関数群///////////////////////////////////////////////////////////////////////////////////////////
	
		//実際のEnemyのDestroy実行処理
		virtual void ExecuteDestroy() override;
		//EnemyのTick():細かい呼び出し順はエネミーBase側で決定
		virtual void GameTick(float _deltaTime)override;
		//Enemyの次のフレームの頭にDestroyする状態をtrueにするかチェック
		virtual void CheckDestroy() override;

	//管理対象の追加・削除/////////////////////////////////////////////////////////////////////////////////////////////

	//自身の管理下にEnemyを追加する関数
	void AddManagementEnemy(AEnemyBase_V2* _target);

	//自身の管理下からEnemyを開放する関数(EnemyにDelegateとして伝達)
	void RemoveManagementEnemy(AEnemyBase_V2* _target);

	//自身のDestroy予定対象にEnemyを追加する関数
	void AddDestroyTargetEnemy(AEnemyBase_V2* _target);

	//自身のDestroy予定対象からEnemyを開放する関数(EnemyにDelegateとして伝達)
	void RemoveDestroyTargetEnemy(AEnemyBase_V2* _target);


	//管理対象配列群//////////////////////////////////////////////////////////////////////////////////////////////////////
	protected:
	//管理対象Enemy配列
	UPROPERTY()
	TArray<AEnemyBase_V2*> m_pEnemies;

	//Destroy予定Enemy配列
	UPROPERTY()
	TArray<AEnemyBase_V2*>m_pDestroyTargetEnemies;
};
