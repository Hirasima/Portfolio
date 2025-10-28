// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager_V2.h"
#include "EnemyBase_V2.h"


//���ۂ�Enemy��Destroy���s����
void UEnemyManager_V2::ExecuteDestroy() {
    //Destroy�Ώۂ�Enemy�Q������Destroy

    //�v�f�ԍ������h�����߈ꎞ�z��ɃR�s�[
    TArray<AEnemyBase_V2*> DestroyTargets=m_pDestroyTargetEnemies;
    //�����j��
    for (AEnemyBase_V2* target : DestroyTargets) {
        
        //�k���`�F�b�N
        if (!target) {
            return;
        }

        //���g�̊Ǘ��z�񂩂�̉���u���b�N/////////////////////////////////////////////////////

        //���g�̊Ǘ��Ώۂ���O��
        RemoveManagementEnemy(target);
        //���g��Destroy�\��҂���O��
        RemoveDestroyTargetEnemy(target);

        //Destroy���s�u���b�N///////////////////////////////////////////////////////////////
        //Destroy����+Destroy���O�Ƀg���K�[����֐�
        target->ExecuteDestroy();
    }
}



//���g�̍X�V�֐�(Enemy�z��̍X�V���܂Ƃ߂ČĂ�)
void UEnemyManager_V2::GameTick(float _deltaTime) {
    //�Ǘ��Ώۂ�Enemy�̍X�V���������ɌĂ�ł���
    for (AEnemyBase_V2* enemy : m_pEnemies) {
        //�Ώۃk���`�F�b�N
        if (!enemy) {
            continue;
        }

        //�X�V�֐����Ăԁi�ׂ����X�V����Enemy���Ō���j
        enemy->GameTick(_deltaTime);
    }
}


//Enemy�̎��̃t���[���̓���Destroy�����Ԃ�true�ɂ��邩�`�F�b�N
void UEnemyManager_V2::CheckDestroy() {
    //�Ǘ��Ώۂ�Enemy�̎��t���[����Destroy���邩�̊m�F����������
    for (AEnemyBase_V2* enemy : m_pEnemies) {
        //�Ώۃk���`�F�b�N
        if (!enemy) {
            continue;
        }

        //���t���[����Destroy���邩�̊m�F������
        enemy->CheckDestroy();
    }
}



//�Ǘ��Ώۂ̒ǉ��E�폜///////////////////////////////////////////////////////////////////////////////////////////////

//���g�̊Ǘ�����Enemy��ǉ��֐�����֐�
void UEnemyManager_V2::AddManagementEnemy(class AEnemyBase_V2* _target) {
    //�k���`�F�b�N
    if (!_target) {
        return;
    }
    //���łɊǗ����ɂ����Ă���Ȃ疳��
    if (m_pEnemies.Contains(_target)) {
       return;
    }

    //�Ǘ��Ώۂɒǉ�
    m_pEnemies.Add(_target);

    //Destroy�^�[�Q�b�g�ɒǉ�����R�[���o�b�N�̓o�^
    FRegisterToDestroyer Delegate;
    Delegate.BindUObject(this, &UEnemyManager_V2::AddDestroyTargetEnemy);
    _target->SetRegisterToDestroyerCallBack(Delegate);
}

//���g�̊Ǘ�������Enemy���J������֐�
void UEnemyManager_V2::RemoveManagementEnemy(class AEnemyBase_V2* _target) {
    //�k���`�F�b�N
    if (!_target) {
        return;
    }
    //���łɊǗ����ɒu���ĂȂ��Ȃ疳��
    if (m_pEnemies.Contains(_target)) {
        //�Ǘ��Ώۂ���O��
        m_pEnemies.Remove(_target);
    }
}

//���g��Destroy�\��Ώۂ�Enemy��ǉ�����֐�
void UEnemyManager_V2::AddDestroyTargetEnemy(class AEnemyBase_V2* _target) {
    //�k���`�F�b�N
    if (!_target) {
        return;
    }
    //���łɊǗ����ɂ����Ă���Ȃ疳��
    if (m_pDestroyTargetEnemies.Contains(_target)) {
        return;
    }

    //�Ǘ��Ώۂɒǉ�
    m_pDestroyTargetEnemies.Add(_target);
}


//���g��Destroy�\��Ώۂ���Enemy���J������֐�(Enemy��Delegate�Ƃ��ē`�B)
void UEnemyManager_V2::RemoveDestroyTargetEnemy(class AEnemyBase_V2* _target) {
    //�k���`�F�b�N
    if (!_target) {
        return;
    }
    //���łɊǗ����ɒu���ĂȂ��Ȃ疳��
    if (m_pDestroyTargetEnemies.Contains(_target)) {
        //�Ǘ��Ώۂ���O��
        m_pDestroyTargetEnemies.Remove(_target);
    }
}


