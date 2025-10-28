// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FighterCharacter.h"
#include "GirlsBitWeapon.generated.h"

/**
 �����{�X�̗��p���鋐�啐��
 */

 //��U�������A�N�V����
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

	//�����I�Ɏ茳�ɖ߂�֐�
	void HandWarpQuickly();

	void SetIsWait(bool _flag);

protected:
	void UpdateMoving(float DeltaTime);

public:
	//�o�t�ۑ��p
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	UPROPERTY(EditAnywhere)
		//���݂̏�
	EBitWeaponState m_State;
protected:
	//�W���ҋ@�g�����X�t�H�[��
	FTransform m_TargetWaitPos;
	FName HandleSocketName;
	UPROPERTY()
	class AEnemy_Base* m_pUser;
	//�U���ڕW�n�_
	FVector m_TargetAttackPos;
	//�����s�����i�\�P�b�g�Ԃ�Ԃ񂶂�Ȃ��j���ۂ�
	bool m_IsAutoFly;

	//�U����]�C�p
	float m_AttackedDulation;
	float m_AttackedDulationTimer;

	//��U���ς݂̃A�N�^�[
	TArray<AActor*> m_attackedActors;

	//���ۂ̑��x
	float m_nowSpeed;
	//���݂̍U�����[�h
	EAttackPattarnState m_AttackState;
	//���݂̍U����
	int m_Damage;

	bool m_IsWait;

	float m_SinTimer;
};
