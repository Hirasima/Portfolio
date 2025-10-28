// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase_V2.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include"NavigationSystem.h"
#include "AIController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

//�X�V�����Q///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//���̃t���[���̓���Destroy�����Ԃ�true�ɂ��邩�`�F�b�N
void AEnemyBase_V2::CheckDestroy() {
	//���ł�true�Ȃ疳��
	if (m_IsNextDestroy == true) {
		return;
	}

	//�t���O�`�F�b�N�u���b�N///////////////////////////////////////////////////////////////////////////////////
	//���̃t���[����destroy���邩�ǂ���
	bool DestroyFlag = false;

	//�A�j���[�V�����C�x���g�ŗ\�����������Ȃ甽�f
	if (m_IsEventNextDestroy == true) {
		DestroyFlag = true;
	}


	//true�������Ȃ甭�����鏈��////////////////////////////////////////////////////////////////////////////////

	//���t���[����Destroy���邱�Ƃ����܂����Ȃ�
	if (DestroyFlag == false) {
		return;
	}

	//���g�̏�L�t���O��true�Ŋm��
	m_IsNextDestroy = true;;
	//EnemyManager��Destroy�Ώ۔z��Ɏ��M��o�^
	if (m_RegisterToDestroyerCallBack.IsBound()) {
		m_RegisterToDestroyerCallBack.Execute(this);
	}
}

//Tick()�N
void AEnemyBase_V2::GameTick(float _deltaTime) {
	//���ʂ�Tick�͌Ă�
	Super::Tick(_deltaTime);

	//����ł��痬��
	if (m_NowSituation == EEnemySituation_V2::Dead) {
		return;
	}
}

//���ۂ�Enemy��Destroy���s����(��Destroy()���܂܂�Ă���̂ŃK�`�Ŋ��ɌĂԂ��ƁI�I)
void AEnemyBase_V2::ExecuteDestroy() {
	//�����ŏ��Ŏ��g���K�[�֐����Ă�

	//���g��j��
	Destroy();
}


//�Q�b�^�[�E�Z�b�^�[/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Enemy�}�l�[�W���̔j��\��z��ւ̃��W�X�^�R�[���o�b�N���Z�b�g����֐�
void AEnemyBase_V2::SetRegisterToDestroyerCallBack(FRegisterToDestroyer _callback) {
	//�R�[���o�b�N�̐ݒ�
	m_RegisterToDestroyerCallBack=_callback;
}

//���݂�AnimState���擾����֐�
EEnemyAnimState_V2 AEnemyBase_V2::GetEnemyAnimState_V2() {
	return m_NowAnimState;
}
//���̃t���[����AnimState��ݒ肷��֐�
void AEnemyBase_V2::SetAnimState_V2(EEnemyAnimState_V2 _state) {

	//�㏑���\�Ȃ甽�f:��r�u���b�N//////////////////////////////////////////////////
	
	//�@���͉������D�悳���
	if (_state == EEnemyAnimState_V2::Dead||m_NextAnimState==EEnemyAnimState_V2::Dead||m_NowAnimState==EEnemyAnimState_V2::Dead) {
		m_NextAnimState = EEnemyAnimState_V2::Dead;
		return;
	}

	//�A���D�揇�ʂǂ����悤�˂��A�A�A:�Ƃ肠�����v���C���[�ɗL���Ȃ悤��Stag�D��ݒ�
	if (_state == EEnemyAnimState_V2::Stag || m_NextAnimState == EEnemyAnimState_V2::Stag) {
		m_NextAnimState = EEnemyAnimState_V2::Stag;
	}

	//��������������t���[�����̘b������Ƃ肠�����D�揇�ʂ��������Ȃ������������邩�H
	m_NextAnimState = _state;
}

//�v���C���[�̍��W��ݒ肷��֐�
void AEnemyBase_V2::SetPlayerPos(FVector _pos) {
	m_PlayerPos=_pos;
}
//�ҋ@���W��ݒ肷��֐�
void AEnemyBase_V2::SetWaitPos(FVector _pos) {
	m_WaitPos=_pos;
}
//�ꎞ�I�Ȉړ��Ώۍ��W�i�A���[�����j��ݒ肷��֐�,��2����:true�Ȃ�ҋ@���W�ɖ߂�悤�Ɏw��
void AEnemyBase_V2::SetTargetAlartPos(FVector _pos, bool _resetFlag) {

	//���Z�b�g�t���O��true�Ȃ烊�Z�b�g���ď����𗬂�
	if (_resetFlag == true) {
		//�x���ꏊ��ҋ@���W�ɏ�����
		m_AlartPos=m_WaitPos;
		//�x����Ԃ�����
		m_IsAlart=false;
		return;
	}

	//�x���Ώۍ��W��ݒ�
	m_AlartPos=_pos;
	//�x����Ԃɓ���
	m_IsAlart=true;
}

//�C�x���g�֐��E�C�x���g�ŌĂ΂��֐��ŏ�L�ȊO//////////////////////////////////////////////////////
//���t���[���Ɏ󂯂鑍��_���[�W�����Z����֐�
void AEnemyBase_V2::AddNextFlameDamage(int _damage) {
	m_NextFlameDamage+=_damage;
}

//Tick���ŌĂԊ֐�//////////////////////////////////////////////////////////////////////////////////

//�O�t���[������_���[�W��HP�ւ̔��f
void AEnemyBase_V2::ApplyHp() {
	m_hitPoint-=m_NextFlameDamage;
	//0�ɕ␳
	if (m_hitPoint < 0) {
		m_hitPoint=0;
	}
	//�ő�l�ɕ␳
	if (m_hitPoint > m_Max_HitPoint) {
		m_hitPoint=m_Max_HitPoint;
	}
}

//��g�X�e�[�g�̕ύX�`�F�b�N/////////////////////////////////////////////////
void AEnemyBase_V2::DefineSituation(float _deltaTime) {
	//���͐�΂Ȃ̂Ŋ��Ɏ��Ȃ痬��
	if (m_NowSituation == EEnemySituation_V2::Dead) {
		return;
	}

	//HP�؂�Ȃ玀
	if (m_hitPoint <= 0) {
		m_NowSituation=EEnemySituation_V2::Dead;
		return;
	}

	//���݂��p�g���[���Ȃ�F�v���C���[�����F������o�g���Ɉڍs
	if (m_NowSituation == EEnemySituation_V2::Patrol) {
		if (CheckPlayerFound() == true) {
			m_NowSituation=EEnemySituation_V2::Battle;
		}
		return;
	}

	//���݂��o�g���Ȃ�F��莞�ԃv���C���[������������p�g���[���Ɉڍs
	if (m_NowSituation == EEnemySituation_V2::Battle) {
		//�v���C���[�������p�����Ԃ̍X�V
		if (CheckPlayerFound() == false) {
			m_LostPlayerTime+=_deltaTime;
		}
		else {
			m_LostPlayerTime=0.0f;
		}

		//�v���C���[���������Ԃ��K�莞�Ԉȏ�Ȃ�p�g���[����
		if (m_LostPlayerTime >= m_KeepBattleTime) {
			//���������Ԃ̏�����
			m_LostPlayerTime=0.0f;
			m_NowSituation=EEnemySituation_V2::Patrol;
		}
	}

}
//�v���C���[�����F���Ă��邩�̊m�F
bool AEnemyBase_V2::CheckPlayerFound() {
	//�����`�F�b�N
	if (FVector::Dist(m_PlayerPos, GetActorLocation()) > m_ViewLength) {
		return false;
	}

	//����p�`�F�b�N���}�W�b�N�i���o�[�Ŏ���p�X�O�ɂȂ��Ă�
	FVector ToTargetDirection = (m_PlayerPos - GetActorLocation()).GetSafeNormal();
	//�h�b�g��
	float Dot = FVector::DotProduct(GetActorForwardVector(), ToTargetDirection);
	//�p�x���f�O���[�ɕϊ�
	float TargetDegree = FMath::Acos(Dot) * (180.f / PI);
	if (TargetDegree > 180 / 2.0f) {
		return false;
	}

	//�Օ��`�F�b�N
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), m_PlayerPos, ECC_Visibility, Params);
	if (!bHit) {
		return false;
	}
	//�q�b�g�����A�N�^�[���v���C���[�Ȃ�true
	if (bHit && HitResult.GetActor()) {
		if (HitResult.GetActor()->ActorHasTag(FName("Player"))) {
			return true;
		}
	}

	return false;
}


//��g�X�e�[�g���Ƃ̕��򏈗�:Dead�͂�������return�Ȃ̂Ŗ���,��������AnimState���߂��Ⴈ��/////////////

//�p�g���[��
void AEnemyBase_V2::DoPatrol() {
	//�x�����̏���/////////////////////////////////////////////////////
	if (m_IsAlart == true) {
		return;
	}


	//�ꎞ�x���ʒu��ڎw���Ĉړ�����F����Ȃ�////////////////////////////
	//AI�R���g���[���̎擾
	AAIController* m_pController= Cast<AAIController>(GetController());
	
	//�ꎞ�x���ʒu�Ƃ̋���������Ă��āA�X�e�[�g��Wait�Ȃ�:���d�Ăяo���h�~
	if(FVector::Dist(GetActorLocation(),m_AlartPos)>50.0f&&m_NowAnimState==EEnemyAnimState_V2::Wait){
		//���t���[���\��A�j���[�V������Walk��
		SetAnimState_V2(EEnemyAnimState_V2::Walk);
		if (m_NextAnimState == EEnemyAnimState_V2::Walk) {
			//�ꎞ�x���ʒu�Ɉړ�
			m_pController->MoveToLocation(m_AlartPos);
		}
	}
	//�قړ������Ă���Ȃ�
	else if(m_NowAnimState==EEnemyAnimState_V2::Walk){
		//���t���[���\��A�j���[�V������Wait��
		SetAnimState_V2(EEnemyAnimState_V2::Wait);
		//��~
		m_pController->StopMovement();
	}
	
}

//�o�g��///////////
void AEnemyBase_V2::DoBattle() {

}

//���肳�ꂽNextAnimState��NowAnimState�ɔ��f+Next�̏�����
void AEnemyBase_V2::ApplyAnimState() {

}