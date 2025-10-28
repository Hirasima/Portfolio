// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "FighterCharacter.h"
#include "Enemy_Base.generated.h"

//クラスの前方宣言
class AEnemyManager;
class ABattleManager;
class UCapsuleComponent;
class AWeapon;
class UWidgetComponent;
class UNiagaraSystem;


//状態列挙型
UENUM(BlueprintType)
enum class EEnemySituationState : uint8
{
	Patrol   UMETA(DisplayName = "Patrol"),
	Battle   UMETA(DisplayName = "Battle"),
	Dead     UMETA(DisplayName = "Dead")
};

//状態列挙型
UENUM(BlueprintType)
enum class EEnemyMotionState : uint8
{
	Wait			UMETA(DisplayName = "Wait"),
	Dash			UMETA(DisplayName = "Dash"),
	UpGuard			UMETA(DisplayName = "UpGuard"),
	DownGuard		UMETA(DisplayName = "DownGuard"),
	Parried			UMETA(DisplayName = "Parried"),
	Damaged			UMETA(DisplayName = "Damaged"),
	Attack			UMETA(DisplayName = "Attack"),
	Stag			UMETA(DisplayName = "Stag"),
	Assacinated		UMETA(DisplayName = "Assacinated"),
	Dead			UMETA(DisplayName = "Dead")
};

//エネミーマネージャからのアンレジスタ関数
DECLARE_DELEGATE_OneParam(FUnregisterFromEnemyMng, AEnemy_Base*);
//バトルマネージャからのアンレジスタ関数
DECLARE_DELEGATE_OneParam(FUnregisterFromBattleMng, AEnemy_Base*);
//バトルマネージャへの登録関数
DECLARE_DELEGATE_OneParam(FRegisterFromBattleMng, AEnemy_Base*);
//バトルマネージャにヒットストップ通知をする関数
DECLARE_DELEGATE(FCallOfHitStop);


UCLASS()
class ORIGINALACTION_API AEnemy_Base : public AFighterCharacter
{
	GENERATED_BODY()

	//各マネージャクラスのフレンド化★大規模化の場合インターフェース利用に切り替え
	friend class AEnemyManager;
	friend class ABattleManager;

public:
	// Sets default values for this pawn's properties
	AEnemy_Base();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//アニメーションステート取得関数
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetNowTargetMontage();

	//攻撃時にアニメーションモンタージュを取得する関数
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetAttackMontage();

	//よろけ時にアニメーションモンタージュを取得する関数
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetStagMontage();

	//現在のモーション状況を取得する関数
	UFUNCTION(BlueprintCallable)
	virtual EEnemyMotionState GetNowMotionState()const;

	//現在のモーション状況を設定する関数
	UFUNCTION(BlueprintCallable)
	void SetNowMotionState(EEnemyMotionState _state);

	//ノックバックを終了させる関数
	UFUNCTION(BlueprintCallable)
	void KnockBackEnd();

	//ノックアップを終了させる関数
	UFUNCTION(BlueprintCallable)
	void KnockUpEnd();

	//ノックバックする関数
	void KnockBackStart(FVector _powerPos, float _power);

	//ガードを開始する関数:終了はAttackFinishを呼べばいいので簡略化
	UFUNCTION(BlueprintCallable)
	void GuardStart();

	//ガン見開始
	UFUNCTION(BlueprintCallable)
	void LockOnStart();

	//攻撃されたアクターのリストの中身を消す
	UFUNCTION(BlueprintCallable)
	virtual void ClearAttackedActors();

	//エネミーの状態更新関数*エネミーマネージャからのみ呼ぶこと*
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos);
	//エネミーのアニメーションステート更新
	virtual void EnemyBattleAnimTick();
	//エネミーの戦闘更新関数*バトルマネージャからのみ呼ぶこと*
	virtual void EnemyBattleTick(float _DeltaTime, FVector _PlayerPos);


	//エネミーの状況を設定する関数
	void SetEnemySituation(EEnemySituationState _situ);
	//エネミーの予知している攻撃を設定する関数*バトルマネージャからのみ呼び出し
	UFUNCTION(BlueprintCallable)
	void SetScenceOfPlayerAttack(EAttackPattarnState _pattarn);

	//エネミーマネージャからのアンレジスタ関数コールバック*エネミーマネージャからのみ変更する事*
	FUnregisterFromEnemyMng UnregisterFromEnemyMngCallBack;
	//バトルマネージャからのアンレジスタ関数コールバック*バトルマネージャからのみ変更する事*
	FUnregisterFromBattleMng UnregisterFromBattleMngCallBack;
	//バトルマネージャへの登録関数コールバック*エネミーマネージャからのみ変更する事*
	FRegisterFromBattleMng RegisterToBattleMngCallBack;
	//バトルマネージャにヒットストップ通知をする関数*バトルマネージャのみから変更すること
	FCallOfHitStop CallOfHitStopCallBack;

	//攻撃時関数☆時間が来ていたらの条件はTick側にしておいて
	virtual void Attack();

	//巡回中（非戦闘時）の挙動
	virtual void Patrol();

	//警戒状態のオンオフ関数
	virtual void BeAlarm(FVector _alartPos);
	virtual void EndAlarm();

	//視界にプレイヤーがいるかチェック
	virtual bool PlayerFindCheck();

	//バトルマネージャから離脱する関数(友バトルマネージャから呼ばれることはある)
	virtual void CheckOutBattleManager(FVector _PlayerPos);

	////戦闘中の移動周り
	virtual void InBattleMove();

	virtual void SpawnEffect(EAttackReaction _attackReaction)override;

	//プレイヤーの方を向く関数
	void LookforPlayer();

	//★HPバー技術検証用
	UFUNCTION(BlueprintImplementableEvent,Category="DebugFunc")
	void ScreenHpBar();

	//被致命攻撃時
	virtual void OnCriticalAttacked(int _damage)override;

	//フロー割れの時にトリガーする関数
	virtual void BreakingFlowTrriger();

	//フローの更新関数
	virtual void UpdateFlow(float _deltaTime);

protected:
	//プレイヤーの座標
	FVector m_PlayerPos;

	//現在のパラメーター変数
	int Mp;
	float At;
	//現在の状態
	EEnemySituationState m_Situation;
	//現在のよろけ状態
	EStagType m_StagState;
	//現在予知しているプレイヤーの攻撃
	EAttackPattarnState m_NowScenceOfPlayerAttack;

	//ガード継続時間
	float m_guardTime;

	//弾かれ継続時間
	float m_parriedTime;

	//弾かれているか
	bool m_bParried;

	//今失明しているか
	bool m_IsBlind = false;

	//警戒状態であるか
	bool m_IsAlart = false;

	//持ち場（初期位置）
	FTransform m_firstPos;
	//警告位置
	FVector m_alartPos;
	//警告位置についたか否か
	bool m_isAlartPoin;

	//現在のモーション状況
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	EEnemyMotionState m_MotionState;

	//面
	UPROPERTY(EditAnywhere)
	UStaticMesh* m_pFaceMesh;

	//警戒解除タイマー
	FTimerHandle AlartEndTimerHandle;

	//ノックバックの時に一時保存する用ムーブメントデータ
	int m_Def_GroundFriction;
	int m_Def_BrakingFrictionFactor;
	int m_Def_BrakingDecelWalking;
	bool m_Def_bOrientRotationToMovement;

	bool bKnockBack = false;

	//フロー関係タイマー
	float FlowTimer = 0;

	//☆攻撃ロジック変更のための変数
	TArray<int> m_nowHandIndexies;
	int m_nowHand;

	//打ちあがるか否か
	bool m_CanKnockUp = true;

	//コンボ数★AttackAssetシステム移行変数
	int m_combo;

	//プレイヤーを見続ける攻撃状態
	bool m_LookPlayer = false;

	

public:	

	//攻撃
	UFUNCTION(BlueprintCallable)
	virtual void OnAttack();

	//攻撃終了時
	UFUNCTION(BlueprintCallable)
	virtual void OnFinishAttack();

	//予兆関数
	UFUNCTION(BlueprintCallable)
	virtual void SpawnAttackScenceEffect();

	//死亡アニメーション終了時に呼ぶやつ（消去処理）
	UFUNCTION(BlueprintCallable)
	virtual void DeleteOut();

	//攻撃を受けたときに呼ばれる関数
	virtual EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false)override;

	//被ダメージ時
	virtual void Damaged(int _damagePoint)override;

	//はじかれた時の呼ばれる関数
	virtual void Parried()override;

	//待機座標を設定する関数
	void SetWaitPosition(FTransform _targetPos);

	//HP取得関数
	int GetEnemyHp();

	//照明状況の伝達関数
	void SetIsBlind(bool _isBlind);

	//視界範囲を取得する関数
	float GetViewLange();
	

	//パラメータ変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	bool m_IsUseGuard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	bool m_IsUseFlow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	int Max_Mp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float Max_At;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float GuardGein_Mp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float ParriedGein_Mp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	bool m_IsAttackLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AngerParam")
	float UpperAnger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AngerParam")
	float UnderAnger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_turningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_NearestRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_BattleSearchRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_BlindBattleSearchRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	bool m_IsAnotherFirstPos=false;

public:
	//基本アニメーションモンタージュ群///////////////////////////////////////
	//上段ガード
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpGuardMontage;
	//下段ガード
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUnderGuardMontage;
	//被弾き
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpParriedMontage;
	//死
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pDeadMontage;

	//よろけ群
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pMiniStagMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pBigStagMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pKnockUpMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pKnockUpKeepMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pAssacinatedMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset GuardCounterAttack;
	//////////////////////////////////////////////////////////////////////
	

	//ダメージエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* DamageEffect;

	//ガードエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* GuardEffect;
	//被パリィ時被弾エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParriedDamageEffect;

	//パリィエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryEffect;

	//フローオーラ（仮）エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* FlowAura;

	//攻撃予告エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* AttackScenceEffect;

	//ガードの最大時間
	UPROPERTY(EditAnywhere)
	float m_guardMaxTime;

	//はじかれ最大時間
	UPROPERTY(EditAnywhere)
	float m_parriedMaxTime;

	//フロー状態か否か
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
	bool m_IsFlow;
	//フロー倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
	float m_FlowSpeed;
	//フローブレイク継続時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
	float m_FlowBreakedTime=5.0f;

	//☆攻撃ロジック変更のための変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeckState")
	int m_MaxCosts;

	//ノックアップ高度をキープするか
	bool m_bIsKeepZAxis = false;
	FVector KeepPos;

	//-------------------------------------------------------
	// 効果音系
	//-------------------------------------------------------

public:

	//SE音量
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	float SE_Volume;

	//フロー終了SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* FlowBreakSE;
};
