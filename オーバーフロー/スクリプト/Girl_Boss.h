// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Base.h"
#include "GirlsBitWeapon.h"
#include "Girl_Boss.generated.h"

/**
 * 
 */

 //攻撃列挙型
UENUM(BlueprintType)
enum class EGirlBossAttackState : uint8
{
	Wait   UMETA(DisplayName = "Wait"),
	Montage   UMETA(DisplayName = "Montage")
};


//ソケットと待機トランスフォームの塊
USTRUCT(BlueprintType)
struct FHandleWaitInfo
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HandHandleName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform WaitTransform;
};

//ビットとソケットの塊
USTRUCT(BlueprintType)
struct FBitWeaponWithHandles
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGirlsBitWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHandleWaitInfo WaitHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHandleWaitInfo HandHandle;
};


UCLASS()
class ORIGINALACTION_API AGirl_Boss : public AEnemy_Base
{
	GENERATED_BODY()
	

public:

	virtual void BeginPlay()override;

	UFUNCTION(BlueprintCallable)
	EGirlBossAttackState GetGirlBossAnimState();
	UFUNCTION(BlueprintCallable)
	int GetStageLv();

	//武器重なった時
	virtual void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult)override;
	
	UFUNCTION(BlueprintCallable)
	void EndAnim();

	//☆試運転用
	UFUNCTION(BlueprintCallable)
	void ReturnLHandWeaponToWait();
	//☆試運転用
	UFUNCTION(BlueprintCallable)
	void ReturnRHandWeaponToWait();

	UFUNCTION(BlueprintCallable)
	void L_WeaponActive();
	UFUNCTION(BlueprintCallable)
	void R_WeaponActive();

	UFUNCTION(BlueprintCallable)
	void Call_R_Weapon();
	UFUNCTION(BlueprintCallable)
	void Call_L_Weapon();

	UFUNCTION(BlueprintCallable)
	void EndAttackMotion();
	UFUNCTION(BlueprintCallable)
	void EndAttackMotion2();

	//ビット配列
	UPROPERTY(EditAnywhere)
	TArray<FBitWeaponWithHandles> m_WeaponsInfo;

	UPROPERTY(EditAnywhere)
	UAnimMontage* m_ProtSlashMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* m_ProtDuoSlashMontage;


	void ForgetDamagedActor();

	virtual EAttackPattarnState GetAttackPattarnState()override;

protected:
	//Tick
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos)override;

	void SpawnCausionEffect(FVector _pos);

	//☆Enemy_Baseに移植したい手札システム
	// 
	// At<=0で手札最大枚数になるか、最大コストになるまでドロー
	// 現在手札の0番目から切っていく(このターンの行動回数は保存)
	// 手札が0になったらAtを元に戻す
	// 
	//手札選出
	void DrewHand();
	//現在手札(対応番号)
	TArray<int> Now_Hand;
	//現在カードインデックス(山札概念)(0,0,0,1,1,2見たいなカード一覧のインデックス群)
	TArray<int> CardsDeck;

	//デッキに所有しているカード一覧(インデックス=対応番号,要素(枚数,コスト))
	TArray<FIntPoint> CardsData;

	//現在の最大手札枚数
	int HandsMax=3;
	//現在の最大コスト数
	int MaxHandsCosts=6;
	//現在切った手札枚数
	int NowUsedCardsNum=0;

	bool NowMotion = false;

	/////////////////////////////////////////

	//モードチェンジのやつ
	void StepUp0_1();
	void StepUp1_2();

	//攻撃
	void Attack()override;

	//攻撃状態
	EGirlBossAttackState m_AttackState;

	//強化ステージ(0→2)
	int StageLv;
	FVector m_firstPlayerPos;
	float Timer;
};
