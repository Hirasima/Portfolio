// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FighterCharacter.h"
#include "GirlsBitWeapon.generated.h"

/**
 少女ボスの利用する巨大武器
 */

 //被攻撃時リアクション
 UENUM(BlueprintType)
 enum class EBitWeaponState : uint8
 {
 	None	UMETA(DisplayName = "None"),
 	Wait	UMETA(DisplayName = "Wait"),
 	Go		UMETA(DisplayName = "Go"),
 	Return	UMETA(DisplayName = "Return")
 };
UCLASS()
class ORIGINALACTION_API AGirlsBitWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime)override;

	virtual void BeginPlay()override;

	UFUNCTION()
	void OnBitWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);

	void Initialize(class AEnemy_Base* _user,FName _socketName,FTransform _waitTransform);

	void SetTargetSocket(AEnemy_Base* _user, FName _socketName);
	void SetWaitTransform(FTransform _waitTransform);
	
	void SetBitMoveState(EBitWeaponState _state);
	EBitWeaponState GetBitMoveState();

	void StartAttack(FVector _targetPos,float AttackedDulation,float Dulation=0.0f);
	
	void SetMoveSpeedAccelerator(float _MoveSpeed);
	void SetDamage(int _damage);
	void SetAttackType(EAttackPattarnState _type);

	//強制的に手元に戻る関数
	void HandWarpQuickly();

	void SetIsWait(bool _flag);

protected:
	void UpdateMoving(float DeltaTime);

public:
	//バフ保存用
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	UPROPERTY(EditAnywhere)
		//現在の状況
	EBitWeaponState m_State;
protected:
	//標準待機トランスフォーム
	FTransform m_TargetWaitPos;
	FName HandleSocketName;
	UPROPERTY()
	class AEnemy_Base* m_pUser;
	//攻撃目標地点
	FVector m_TargetAttackPos;
	//自立行動中（ソケットぶんぶんじゃない）か否か
	bool m_IsAutoFly;

	//攻撃後余韻用
	float m_AttackedDulation;
	float m_AttackedDulationTimer;

	//被攻撃済みのアクター
	TArray<AActor*> m_attackedActors;

	//実際の速度
	float m_nowSpeed;
	//現在の攻撃モード
	EAttackPattarnState m_AttackState;
	//現在の攻撃力
	int m_Damage;

	bool m_IsWait;

	float m_SinTimer;
};
