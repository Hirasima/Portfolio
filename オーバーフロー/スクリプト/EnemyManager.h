// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_Base.h"
#include "EnemyManager.generated.h"

//�N���X�̑O���錾
class AEnemy_Base;
class APlayPawn;

UCLASS()
class ORIGINALACTION_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�Ǘ��Ώۂ���������֐�
	void UnregisterMngEnemy(AEnemy_Base* _deadEnemy);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//���͈͓��̃G�l�~�[��Ԃ��֐�
	TArray<AEnemy_Base*> GetBattleEnemies();

	//n�Ԗڂɋ߂��G�l�~�[��Ԃ��֐�
	AEnemy_Base* GetEnemyByDistNum(int _DistNum);

	//�o�g���}�l�[�W�����W�X�^�֐������炤�֐�
	void SetBattleManagerRegister(FRegisterFromBattleMng _regis);

	//�G�l�~�[�̏����ݒ�����s����֐�
	void InitAllEnemies();

	//�x����ԂɂȂ�������`�B����֐�
	void BeAlart(bool _Alart, FVector _alartpos = FVector::Zero());

	//�G�l�~�[��V�K�o�^����֐�
	void AddNewEnemy(AEnemy_Base* _pEnemy);

	//�^�[�Q�b�g�G�l�~�[��Ԃ��֐�
	//
	//�T���������ɑ��݂��Ȃ��ꍇ��nullptr��Ԃ�
	//�T���������ɑO�̃^�[�Q�b�g (_previousTarget) �������݂��Ȃ����_previousTarget��Ԃ�
	AEnemy_Base* GetTargetEnemy(float _searchDistance, FVector _searcherRightVector, FVector _searcherPostion, float _nextTargetDirection = 0.0f, AEnemy_Base* _previousTarget = nullptr);


protected:
	//�Ǘ��Ώۂ̃G�l�~�[�Q
	UPROPERTY()
	TArray<AEnemy_Base*>AllEnemies;

	//�v���C���[�̌��ݍ��W
	FVector PlayerPos;

	//�o�g���}�l�[�W���ɓo�^���邽�߂̊֐��f���Q�[�g�i�G�l�~�[�ɓ`�B�������̂Łj
	FRegisterFromBattleMng m_BattleManagerRegister;

};


