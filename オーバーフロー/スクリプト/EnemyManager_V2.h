// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameTickable.h"
#include "EnemyManager_V2.generated.h"


//�N���X�̑O���錾
class AEnemyBase_V2;

UCLASS()
class ORIGINALACTION_API UEnemyManager_V2 : public UObject,public IGameTickable
{
	GENERATED_BODY()

	public:

	//�X�V�֐��Q///////////////////////////////////////////////////////////////////////////////////////////
	
		//���ۂ�Enemy��Destroy���s����
		virtual void ExecuteDestroy() override;
		//Enemy��Tick():�ׂ����Ăяo�����̓G�l�~�[Base���Ō���
		virtual void GameTick(float _deltaTime)override;
		//Enemy�̎��̃t���[���̓���Destroy�����Ԃ�true�ɂ��邩�`�F�b�N
		virtual void CheckDestroy() override;

	//�Ǘ��Ώۂ̒ǉ��E�폜/////////////////////////////////////////////////////////////////////////////////////////////

	//���g�̊Ǘ�����Enemy��ǉ�����֐�
	void AddManagementEnemy(AEnemyBase_V2* _target);

	//���g�̊Ǘ�������Enemy���J������֐�(Enemy��Delegate�Ƃ��ē`�B)
	void RemoveManagementEnemy(AEnemyBase_V2* _target);

	//���g��Destroy�\��Ώۂ�Enemy��ǉ�����֐�
	void AddDestroyTargetEnemy(AEnemyBase_V2* _target);

	//���g��Destroy�\��Ώۂ���Enemy���J������֐�(Enemy��Delegate�Ƃ��ē`�B)
	void RemoveDestroyTargetEnemy(AEnemyBase_V2* _target);


	//�Ǘ��Ώ۔z��Q//////////////////////////////////////////////////////////////////////////////////////////////////////
	protected:
	//�Ǘ��Ώ�Enemy�z��
	UPROPERTY()
	TArray<AEnemyBase_V2*> m_pEnemies;

	//Destroy�\��Enemy�z��
	UPROPERTY()
	TArray<AEnemyBase_V2*>m_pDestroyTargetEnemies;
};
