// Fill out your copyright notice in the Description page of Project Settings.
//★こいつボス個体らしい

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Base.h"
#include "Enemy_0.generated.h"

//クラスの前方宣言
class UAnimMontage;
class UNiagaraComponent;
class UBossHpBarWidget;

//アニメーションステート列挙型★こいつはこれから攻撃パターンのみになる
UENUM(BlueprintType)
enum class EAttackAnimState : uint8
{
	Wait   UMETA(DisplayName = "Wait"),
	Dash   UMETA(DisplayName = "Dash"),
	BackStep   UMETA(DisplayName = "BackStep"),
	UpAttack   UMETA(DisplayName = "Up_Attack"),
	UnderAttack   UMETA(DisplayName = "Under_Attack"),
	CombAttack   UMETA(DisplayName = "CombAttack"),
	UpGuard  UMETA(DisplayName = "Up_Guard"),
	UnderGuard   UMETA(DisplayName = "Down_Guard"),
	Parry	UMETA(DisplayName = "Parry"),
	Parried UMETA(DisplayName = "Parried"),
	Damage	UMETA(DisplayName = "Damage"),
	Dead   UMETA(DisplayName = "Dead")
};

UCLASS()
class ORIGINALACTION_API AEnemy_0 : public AEnemy_Base
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AEnemy_0();

	//雑魚の状態に対応するAttackAssetを返す★親移行予定
	FAttackAsset GetAttackAsset()override;

	//攻撃を受けたときに呼ばれる関数
	virtual EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false)override;
	
protected:
	virtual void BeginPlay() override;

	//フローの増減伝達の為にオーバーライド
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos)override;

	//エネミーのアニメーションステート更新
	virtual void EnemyBattleAnimTick()override;

	//アニメーションステート取得関数
	UFUNCTION(BlueprintCallable)
	EAttackAnimState GetAttackAnimState();

	//アニメーションモンタージュを取得する関数
	virtual UAnimMontage* GetAttackMontage()override;

	//攻撃終了時
	void OnFinishAttack()override;

	//攻撃の種類の取得
	virtual EAttackPattarnState GetAttackPattarnState()override;

	//攻撃時関数☆時間が来ていたらの条件はTick側にしておいて
	virtual void Attack()override;

	//攻撃アニメーションステート
	EAttackAnimState m_AttackAnimState = EAttackAnimState::Wait;

	//アタックアセット★親移行予定
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUnders;

	//ボスのゲージ類Widgetの実体
	UPROPERTY(VisibleInstanceOnly)
	UBossHpBarWidget* m_BossGageWidget;

private:
	//コンボ数★親移行予定
	int m_combo;
	int m_DefaultMaxMp;
	bool IsViewGage = false;
public:
	//アニメーションモンタージュ群//////////////////////////////////
	//上段攻撃
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpAttackMontage;
	//下段攻撃
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUnderAttackMontage;
	//必殺技★説明用仮置き
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pCombAttack;
	
	
	////////////////////////////////////////////////////////////////

	//---------------------------------------------------
	// エフェクト関係
	//---------------------------------------------------

	////////////////////////////////////////////////////////////////

	//ボスゲージ関係
	//プレイヤーのゲージ類Widgetのクラス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> BossGageWidgetClass;


	//チュートリアル系
	UPROPERTY(EditAnywhere, Category = "Chutereal")
	bool m_isChutereal = false;
	UPROPERTY(EditAnywhere, Category = "Chutereal")
	TSubclassOf<class AChuteRealManager> ChuterealManagerClass;
	bool IsSendedDead = false;
};
