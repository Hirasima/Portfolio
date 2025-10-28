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

	//�@�I���W�i�����\�b�h ////////////////////////////////////

	//�^�[�Q�b�g�Ƀ_���[�W��^����
	void Damage();

	//�_���[�W�Ώۂ�ݒ�
	void SetDamageTarget(TArray<AActor*> _targets);

	//�_���[�W��^����񐔂�ݒ�
	void SetAttackNum(int _attackNum);

	//�_���[�W�ʂ�ݒ�
	void SetAttackPoint(int _attackPoint);

	// �v���p�e�B ///////////////////////////////////////////
public:

	//�^����_���[�W��
	UPROPERTY(EditDefaultsOnly)
	int AttackPoint;

	//�_���[�W��^�������
	UPROPERTY(EditDefaultsOnly)
	float AttackPeriod;

	//�_���[�W��^�����
	UPROPERTY(EditDefaultsOnly)
	int AttackNum;

	//�_���[�W���̃G�t�F�N�g
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HitEffect;

private:

	//�i�C�A�K���R���|�[�l���g
	UNiagaraComponent* NiagaraComp;

	//�_���[�W��^������
	int m_attackCount;

	//�_���[�W��^���鎞��
	float m_attackTimer;

	//�_���[�W�Ώ�
	TArray<AActor*> m_damageTargets;
};
