// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_Base.h" 
#include "BattleManager.generated.h"

//�N���X�̑O���錾
class AEnemyManager;
class AEnemy_Base;

UCLASS()
class ORIGINALACTION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�G�l�~�[�}�l�[�W���Ɍ�풆�̃G�l�~�[�����Ȃ����m�F����X�V�֐�
	void SetBattleEnemies();

	//�^�[�Q�b�g�G�l�~�[��ݒ肷��֐�
	void SetTargetEnemy();

	//���X�g����ΏۃG�l�~�[���������֐�
	void UnregisterBattleEnemy(AEnemy_Base* _deadEnemy);

	//�G�l�~�[����̃q�b�g�X�g�b�v�ʒm����������֐�
	void CallOfHitStop();

protected:
	UPROPERTY()
	AEnemyManager* Ref_EnemyManager;
	UPROPERTY()
	AEnemy_Base* m_pTargetEnemy;

	int TargetEnemyNum = 0;

	bool IsNowSlow = false;

	


public:		
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�G�l�~�[�}�l�[�W����ݒ肷��֐�
	void SetEnemyManagerRef(AEnemyManager* _eMng);

	//�Ǘ��ΏۂƂ��ēo�^����֐�
	void AddEnemyForRegister(AEnemy_Base* _enemy);

	//�^�[�Q�b�g�ԍ���ύX����֐�
	void ChangeTarget(int _targetNum);

	//�^�[�Q�b�g�G�l�~�[��Ԃ��֐�
	AEnemy_Base* GetTargetEnemy();
	

	//�v���C���[�̍U���̋N�������m����֐�
	void SetPlayerAttackScence(EAttackPattarnState _pattarn, FVector _PlayerPos, FVector _playerFrontVec, float _AttackRange, float _AttackDeg);

	//�Ǘ��Ώۂ̃G�l�~�[�Q����
	UPROPERTY(EditAnywhere)
	TArray<AEnemy_Base*> BattleEnemies;
};
