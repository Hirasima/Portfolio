// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Base.h"
#include "Easy_Enemy.generated.h"

/**
 * 
 */
 //攻撃パターン列挙型
UENUM(BlueprintType)
enum class EAttackState : uint8
{
	Wait   UMETA(DisplayName = "Wait"),
	UpAttack   UMETA(DisplayName = "Up_Attack"),
	UnderAttack   UMETA(DisplayName = "Under_Attack"),
};

UCLASS()
class ORIGINALACTION_API AEasy_Enemy : public AEnemy_Base
{
	GENERATED_BODY()
public:
	//攻撃アニメーションステートの取得関数
	UFUNCTION(BlueprintCallable)
	EAttackState GetAttackState();

	//攻撃時のアニメーションモンタージュを取得する関数
	virtual UAnimMontage* GetAttackMontage()override;

	//攻撃パターン取得関数
	virtual EAttackPattarnState GetAttackPattarnState()override;

	//攻撃終了時
	virtual void OnFinishAttack()override;

	//雑魚の状態に対応するAttackAssetを返す★親移行予定
	FAttackAsset GetAttackAsset()override;

	//アタックアセット★親移行予定
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUnders;

protected:
	//攻撃関数
	virtual void Attack()override;

	//エネミーの状態更新関数*エネミーマネージャからのみ呼ぶこと*
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos)override;


protected:
	//攻撃状態
	EAttackState m_NowAttackState;

	//チュートリアル系
	UPROPERTY(EditAnywhere,Category="Chutereal")
	bool m_isChutereal = false;
	UPROPERTY(EditAnywhere, Category = "Chutereal")
	TSubclassOf<class AChuteRealManager> ChuterealManagerClass;

	//アニメーションモンタージュ群//////////////////////////////////
	//上段攻撃
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpAttackMontage;
	//下段攻撃
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUnderAttackMontage;

	////////////////////////////////////////////////////////////////

	bool IsSendedDead = false;
	
};
