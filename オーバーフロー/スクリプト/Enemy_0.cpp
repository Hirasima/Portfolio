// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_0.h"
#include "Weapon.h"
#include "PlayPawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "EnemyHpBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include"ChuteRealManager.h"
#include "BossHpBarWidget.h"

AEnemy_0::AEnemy_0() {
	
}

void AEnemy_0::BeginPlay()
{
	Super::BeginPlay();

	m_CanKnockUp = false;

	//�v���s�\�ɂ���
	m_bEnableCritical = false;

	m_DefaultMaxMp = Max_Mp;

	//�{�X�̃Q�[�W��Widget�������A�`��//////////////////////////////////////////////////////////
	if (!BossGageWidgetClass) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:PlayerGageWidgetClass In BeginPlay_WireHUD"));
		return;
	}
	//����
	m_BossGageWidget = CreateWidget<UBossHpBarWidget>(GetWorld(), BossGageWidgetClass);
	if (!m_BossGageWidget) {
		UE_LOG(LogTemp, Warning, TEXT("Create Faild:BossGageWidget In BeginPlay_Enemy-0"));
		return;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void AEnemy_0::EnemyTick(float _DeltaTime, FVector _PlayerPos) {
	if (m_Situation == EEnemySituationState::Dead && IsSendedDead == false) {
		//�`���[�g���A���}�l�[�W����T��
		AChuteRealManager* Mana = Cast<AChuteRealManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ChuterealManagerClass));
		if (!Mana) {
		}
		else {
			Mana->AddDeadCount();
			IsSendedDead = true;
		}
	}

	//�t���[�̑�����`�B
	m_BossGageWidget->SetFlowPercent((float)Mp / (float)m_DefaultMaxMp);
	if (IsViewGage == false&&m_Situation==EEnemySituationState::Battle) {
		if (PlayerFindCheck()&&m_BossGageWidget) {
			//�`�恙��Ő퓬�J�n���Ɉړ�
			m_BossGageWidget->AddToViewport();
			IsViewGage = true;
		}
	}

	if (m_hitPoint <= 0) {
		m_BossGageWidget->RemoveFromParent();
	}
	Super::EnemyTick(_DeltaTime, _PlayerPos);
}

//�G�l�~�[�̃A�j���[�V�����X�e�[�g�X�V
void AEnemy_0::EnemyBattleAnimTick() {
	Super::EnemyBattleAnimTick();
}

//���c���ԕύX�֐�
//�U�����֐������Ԃ����Ă�����̏�����Tick���ɂ��Ă�����
void AEnemy_0::Attack() {
	//�X�e�[�g��Attack�ɕύX
	Super::Attack();
	//��������
	At = 0;
	
	//��D����Ȃ��[
	if (m_nowHandIndexies.Num() <= 0) {
		for (int i = 0;i < m_MaxCosts;i++) {
			m_nowHandIndexies.Add(FMath::RandRange(0, AttackUnders.Num() - 1));
		}
		m_nowHand = 0;
	}

	m_combo = m_nowHandIndexies[m_nowHand];
	m_AttackAnimState = EAttackAnimState::UnderAttack;
	//���݂̗��p��D�ԍ������Z
	m_nowHand++;

	//�N�[���_�E�����蔻��
	if (m_nowHand >= m_nowHandIndexies.Num()) {
		At = Max_At;
		m_nowHandIndexies.Empty();
	}

	PlayAnimMantage(GetAttackAsset().AnimMantage);
}

//�A�j���[�V�����X�e�[�g�擾�֐�
UFUNCTION(BlueprintCallable)
EAttackAnimState AEnemy_0::GetAttackAnimState() {
	return m_AttackAnimState;
}

//�U�����̃A�j���[�V���������^�[�W�����擾����֐�
UFUNCTION(BlueprintCallable)

//���c���ԕύX�֐�
UAnimMontage* AEnemy_0::GetAttackMontage() {
	UAnimMontage* monta = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Enemy AttackState : %s"), *UEnum::GetValueAsString(m_AttackAnimState));
	//�U���ɂ���ĕς���
	switch (m_AttackAnimState)
	{
	case EAttackAnimState::Wait:
		break;
	case EAttackAnimState::Dash:
		break;
	case EAttackAnimState::BackStep:
		break;
	case EAttackAnimState::UpAttack:
		monta = m_pUpAttackMontage;
		break;
	case EAttackAnimState::UnderAttack:
		monta = m_pUnderAttackMontage;
		break;
	case EAttackAnimState::CombAttack:
		monta = m_pCombAttack;
		break;
	case EAttackAnimState::UpGuard:
		break;
	case EAttackAnimState::UnderGuard:
		break;
	case EAttackAnimState::Parry:
		break;
	case EAttackAnimState::Parried:
		break;
	case EAttackAnimState::Damage:
		break;
	case EAttackAnimState::Dead:
		break;
	default:
		break;
	}

	return monta;
}

//�G���̏�ԂɑΉ�����AttackAsset��Ԃ�
FAttackAsset AEnemy_0::GetAttackAsset()
{
	FAttackAsset attackAsset;

	switch (m_AttackAnimState)
	{
	case EAttackAnimState::UpAttack:
		if (AttackUps.IsValidIndex(m_combo))
		{
			attackAsset = AttackUps[m_combo];
		}
		else
		{
			attackAsset = AttackUps[0];
		}

		break;
	case EAttackAnimState::UnderAttack:
		if (AttackUnders.IsValidIndex(m_combo))
		{
			attackAsset = AttackUnders[m_combo];
		}
		else
		{
			attackAsset = AttackUnders[0];
		}

		break;

	default:
		break;
	}

	return attackAsset;
}


//�U���I����
void AEnemy_0::OnFinishAttack()
{
	Super::OnFinishAttack();
	//�U����Ԃ�ҋ@��
	m_AttackAnimState = EAttackAnimState::Wait;
}


//�U���̎�ނ̎擾
EAttackPattarnState AEnemy_0::GetAttackPattarnState()
{
	//�߂�l�p
	EAttackPattarnState state = EAttackPattarnState::None;

	//���݂̏�Ԃ���i�U����������
	if (m_AttackAnimState == EAttackAnimState::UpAttack)
	{
		//�߂�l����i�U����
		state = EAttackPattarnState::UpAttack;
	}
	//���݂̏�Ԃ����i�U����������
	else if (m_AttackAnimState == EAttackAnimState::UnderAttack)
	{
		//�߂�l�����i�U����
		state = EAttackPattarnState::DownAttack;
	}

	//�߂�l��Ԃ�
	return state;
}

EAttackReaction AEnemy_0::OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp) {
	EAttackReaction returner;

	//��i�U���Ȃ�MP����ɔ�Q���󂯂�
	if (_attackState == EAttackPattarnState::UpAttack&&m_IsFlow) {
		Max_Mp -= _damage * 0.2f;
	}

	//HP�o�[�ɓ`�B
	returner=Super::OnHitAttack(_damage, _breakPower, _attackState, _attacker);
	m_BossGageWidget->SetHpPercent((float)m_hitPoint / (float)m_Max_HitPoint);
	m_BossGageWidget->SetBossFlowPercent((float)Mp / (float)m_DefaultMaxMp,1-(float)(Max_Mp) / (float)m_DefaultMaxMp);

	return returner;
}

