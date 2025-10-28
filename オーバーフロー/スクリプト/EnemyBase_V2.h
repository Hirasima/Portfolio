// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterCharacter.h"
#include "GameTickable.h"
#include "EnemyBase_V2.generated.h"

//Enemyマネージャの破壊予定配列へのレジスタ関数
DECLARE_DELEGATE_OneParam(FRegisterToDestroyer, AEnemyBase_V2*);

//Enemyの大枠ステート列挙型
UENUM(BlueprintType)
enum class EEnemySituation_V2 : uint8
{
	Patrol   UMETA(DisplayName = "Patrol"),
	Battle   UMETA(DisplayName = "Battle"),
	Dead     UMETA(DisplayName = "Dead")
};

//Enemyのアニメーションステート列挙型
UENUM(BlueprintType)
enum class EEnemyAnimState_V2 : uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	//Montage
	Action  UMETA(DisplayName = "Action"),
	Stag	UMETA(DisplayName = "Stag"),
	Dead    UMETA(DisplayName = "Dead"),
	//Sequence
	Walk	UMETA(DisplayName = "Walk"),
	Dash	UMETA(DisplayName = "Dash"),
	Wait	UMETA(DisplayName = "Wait")
};

UCLASS()
class ORIGINALACTION_API AEnemyBase_V2 : public AFighterCharacter , public IGameTickable
{
	GENERATED_BODY()
public:
	//更新関数群//////////////////////////////////////////////////////////////////////////////////////

	//実際のDestroy実行処理+消滅直前トリガーの呼び出し(☆Destroy()が含まれているのでガチ最期に呼ぶこと！！)
	virtual void ExecuteDestroy() override;
	//Tick()君
	virtual void GameTick(float _deltaTime)override;
	//次のフレームの頭にDestroyする状態をtrueにするかチェック
	virtual void CheckDestroy() override;

public:
	//ゲッター・セッター////////////////////////////////////////////////////////////////////////////////

	//Enemyマネージャの破壊予定配列へのレジスタコールバックをセットする関数
	void SetRegisterToDestroyerCallBack(FRegisterToDestroyer _callback);

	//現在のAnimStateを取得する関数
	UFUNCTION(BlueprintCallable)
	EEnemyAnimState_V2 GetEnemyAnimState_V2();
	//次のフレームのAnimStateを設定する関数
	UFUNCTION(BlueprintCallable)
	void SetAnimState_V2(EEnemyAnimState_V2 _state);

	//プレイヤーの座標を設定する関数
	void SetPlayerPos(FVector _pos);
	//待機座標を設定する関数
	void SetWaitPos(FVector _pos);
	//一時的な移動対象座標（アラーム等）を設定する関数,第2引数:trueなら待機座標に戻るように指示
	void SetTargetAlartPos(FVector _pos,bool _resetFlag=false);

public:
	//イベント関数・イベントで呼ばれる関数で上記以外//////////////////////////////////////////////////////
	void AddNextFlameDamage(int _damage);

protected:
	//Tick内で呼ぶ関数//////////////////////////////////////////////////////////////////////////////////
	
	//前フレーム総被ダメージ→HPへの反映
	void ApplyHp();

	//大枠ステートの変更チェック/////////////////////////////////////////////////
	void DefineSituation(float _deltaTime);
	//プレイヤーを視認しているかの確認
	bool CheckPlayerFound();


	//大枠ステートごとの分岐処理:Deadはそもそもreturnなので無し,☆ここでAnimState決めちゃおう
	
	//パトロール
	virtual void DoPatrol();

	//バトル///////////
	virtual void DoBattle();

	//決定されたNextAnimStateをNowAnimStateに反映+Nextの初期化
	void ApplyAnimState();

protected:
	//Enemyマネージャの破壊予定配列へのレジスタコールバック
	FRegisterToDestroyer m_RegisterToDestroyerCallBack;

protected:
	//UPROPERTY()で設定する系変数//////////////////////////////////////////////////////////////////
	
	//視認範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float m_ViewLength;

	//何秒プレイヤーを見失ったら帰るか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float m_KeepBattleTime;

protected:
	//現在のステート////////////////////////////////////////////////////////////////////////////////
	//現在の大枠ステート
	EEnemySituation_V2 m_NowSituation;
	//現在のAnimState
	EEnemyAnimState_V2 m_NowAnimState;

	//次のフレーム用ステート/////////////////////////////////////////////////////////////////////////
	
	//次のフレームのAnimState
	EEnemyAnimState_V2 m_NextAnimState;
	 
	//次のフレームの頭にDestroyするか
	bool m_IsNextDestroy = false;

	//次のフレームの予定総被ダメージ
	int m_NextFlameDamage;

	//イベントによる予兆///////////////////////////////////////////////////////////////////////
	//次のフレームの終わりにDestroy待機にするか
	bool m_IsEventNextDestroy = false;

	//抱えてる変数////////////////////////////////////////////////////////////////////////////

	//プレイヤーの座標
	FVector m_PlayerPos;
	//待機座標
	FVector m_WaitPos;
	//一時警戒対象座標
	FVector m_AlartPos;

	//一時警戒状態か否か
	bool m_IsAlart;
	//同一アニメーションステートを弾くか
	bool m_IsBlockingEqualAnimState;

	//プレイヤーを交戦中に見失った継続時間
	float m_LostPlayerTime;
};
