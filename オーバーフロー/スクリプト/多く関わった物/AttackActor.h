// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackActor.generated.h"

class AFighterCharacter;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ORIGINALACTION_API AAttackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//　オリジナルメソッド ////////////////////////////////////

	//ターゲットにダメージを与える
	void Damage();

	//ダメージ対象を設定
	void SetDamageTarget(TArray<AActor*> _targets);

	//ダメージを与える回数を設定
	void SetAttackNum(int _attackNum);

	//ダメージ量を設定
	void SetAttackPoint(int _attackPoint);

	// プロパティ ///////////////////////////////////////////
public:

	//与えるダメージ量
	UPROPERTY(EditDefaultsOnly)
	int AttackPoint;

	//ダメージを与える周期
	UPROPERTY(EditDefaultsOnly)
	float AttackPeriod;

	//ダメージを与える回数
	UPROPERTY(EditDefaultsOnly)
	int AttackNum;

	//ダメージ時のエフェクト
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HitEffect;

private:

	//ナイアガラコンポーネント
	UNiagaraComponent* NiagaraComp;

	//ダメージを与えた回数
	int m_attackCount;

	//ダメージを与える時間
	float m_attackTimer;

	//ダメージ対象
	TArray<AActor*> m_damageTargets;
};
