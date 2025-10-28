// Fill out your copyright notice in the Description page of Project Settings.


#include "Girl_Boss.h"
#include "TimerManager.h"
#include "AIController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AGirl_Boss::BeginPlay() {
	Super::BeginPlay();
	
	m_CanKnockUp = false;
	//�v���s�\�ɂ���
	m_bEnableCritical = false;
	StageLv = 0;
	//1�{�ڂ̕�������
	if (!m_WeaponsInfo[0].Weapon) {
		return;
	}
	m_WeaponsInfo[0].Weapon->Initialize(this, m_WeaponsInfo[0].WaitHandle.HandHandleName, m_WeaponsInfo[0].WaitHandle.WaitTransform);

	Timer = 0.0f;
	m_firstPlayerPos = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	//��Enemy_Base�ɈڐA�\��J�[�h���X�g�̏�����////////////////////////////////////////////////////////////////////////////////////
	//��D�͂���
	Now_Hand.Empty();

	//�J�[�h���X�g�iUPROPERTY�j�\��
	FIntPoint ThrowDuoKnife = FIntPoint(3, 1);
	FIntPoint SingleBlade = FIntPoint(3, 1);
	FIntPoint DuoBlade = FIntPoint(2, 3);
	FIntPoint BlinkDuoBlade = FIntPoint(2, 5);
	CardsData.Add(ThrowDuoKnife);
	CardsData.Add(SingleBlade);
	CardsData.Add(DuoBlade);
	CardsData.Add(BlinkDuoBlade);
	//�f�b�L������
	//�J�[�h�̃f�[�^���Ԃ��
	for (int i = 0;i < CardsData.Num();i++) {
		//�f�b�L�ɖ������ǉ�
		for (int j = 0;j < CardsData[i].X;j++) {
			CardsDeck.Add(i);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

EGirlBossAttackState AGirl_Boss::GetGirlBossAnimState() {
	return m_AttackState;
}

int AGirl_Boss::GetStageLv(){
	return StageLv;
}

//����̏d�Ȃ������̃C�x���g
void AGirl_Boss::OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult)
{
	//���g�ɓ������Ă����疳��
	if (_otherActor == this)
	{
		return;
	}

	//�A�N�^�[�̓��ꕨ
	//�����݃q�b�g�m�F�̂���
	AFighterCharacter* fighter = nullptr;
	//�L���X�g
	fighter = Cast<AFighterCharacter>(_otherActor);

	//���������A�N�^�[���t�@�C�^�[�Ȃ�
	if (fighter)
	{
		//���ɍU������Ă���A�N�^�[�Ȃ疳��
		if (m_attackedActors.Contains(fighter))
		{
			return;
		}
		//�G���m�Ȃ疳��
		if (Tags.Contains("Enemy") && fighter->Tags.Contains("Enemy"))
		{
			return;
		}

		//UE_LOG(LogTemp, Warning, TEXT("%s : Attack Is Hit"), *this->GetName());

		//��U�����̏�Ԏ󂯎��p
		EAttackReaction attackReaction = EAttackReaction::None;
		//�U����(���̃I�u�W�F�N�g)�̏�Ԓʒm�p
		EAttackPattarnState attackState = GetAttackPattarnState();

		//�U���ς݃A�N�^�[�ɒǉ�
		m_attackedActors.Add(fighter);

		//�U���͂��擾
		int attackPoint = m_attackPoint;

		//��U�����̃C�x���g�֐����Ăяo���A��Ԃ��󂯎��
		attackReaction = fighter->OnHitAttack(attackPoint, 55, attackState, Cast<AFighterCharacter>(this));
		UE_LOG(LogTemp, Log, TEXT("%s : Attack Point Is : %d"), *this->GetName(), attackPoint);
		//UE_LOG(LogTemp, Warning, TEXT("%s : %s Reaction is %s"), *this->GetName(), *fighter->GetName(), *UEnum::GetValueAsString(attackReaction));

	}
}

void AGirl_Boss::ReturnLHandWeaponToWait() {
	if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
		return;
	}
	m_WeaponsInfo[0].Weapon->Initialize(this, m_WeaponsInfo[0].WaitHandle.HandHandleName, m_WeaponsInfo[0].WaitHandle.WaitTransform);
	m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(1.0f);
	m_WeaponsInfo[0].Weapon->SetIsWait(true);
}
void AGirl_Boss::ReturnRHandWeaponToWait() {
	if (m_WeaponsInfo[1].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
		return;
	}
	m_WeaponsInfo[1].Weapon->Initialize(this, m_WeaponsInfo[1].WaitHandle.HandHandleName, m_WeaponsInfo[1].WaitHandle.WaitTransform);
	m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(1.0f);
	m_WeaponsInfo[1].Weapon->SetIsWait(true);
}


void AGirl_Boss::Call_L_Weapon() {
	SpawnCausionEffect(m_WeaponsInfo[0].Weapon->GetActorLocation());
	FVector target = m_WeaponsInfo[0].HandHandle.WaitTransform.GetLocation();
	target.Z -= FVector::Dist(GetMesh()->GetSocketLocation(m_WeaponsInfo[0].HandHandle.HandHandleName), m_PlayerPos);
	FTransform T = m_WeaponsInfo[0].HandHandle.WaitTransform;
	T.SetLocation(target);
	m_WeaponsInfo[0].Weapon->Initialize(this, m_WeaponsInfo[0].HandHandle.HandHandleName, m_WeaponsInfo[0].HandHandle.WaitTransform);
	m_WeaponsInfo[0].Weapon->HandWarpQuickly();
	m_WeaponsInfo[0].Weapon->SetIsWait(false);

}

void AGirl_Boss::Call_R_Weapon() {
	SpawnCausionEffect(m_WeaponsInfo[1].Weapon->GetActorLocation());
	FVector target = m_WeaponsInfo[1].HandHandle.WaitTransform.GetLocation();
	target.Z -= FVector::Dist(GetMesh()->GetSocketLocation(m_WeaponsInfo[1].HandHandle.HandHandleName), m_PlayerPos);
	FTransform T = m_WeaponsInfo[1].HandHandle.WaitTransform;
	T.SetLocation(target);

	m_WeaponsInfo[1].Weapon->Initialize(this, m_WeaponsInfo[1].HandHandle.HandHandleName, m_WeaponsInfo[1].HandHandle.WaitTransform);
	m_WeaponsInfo[1].Weapon->HandWarpQuickly();
	m_WeaponsInfo[1].Weapon->SetIsWait(false);
}

void AGirl_Boss::L_WeaponActive() {
	m_WeaponsInfo[0].Weapon->SetAttacking(true);
}

void AGirl_Boss::R_WeaponActive() {
	m_WeaponsInfo[1].Weapon->SetAttacking(true);
}

void AGirl_Boss::EndAnim() {
	m_MotionState = EEnemyMotionState::Wait;
	NowMotion = false;
	m_AttackState = EGirlBossAttackState::Wait;
}
void AGirl_Boss::EndAttackMotion() {
	m_WeaponsInfo[0].Weapon->SetAttacking(false);
	m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::None);
	m_attackedActors.Empty();
}
void AGirl_Boss::EndAttackMotion2() {
	m_WeaponsInfo[1].Weapon->SetAttacking(false);
	m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::None);
	m_attackedActors.Empty();
}

//Tick
void AGirl_Boss::EnemyTick(float _DeltaTime, FVector _PlayerPos) {
	Super::EnemyTick(_DeltaTime, _PlayerPos);
	Timer += _DeltaTime;
	if (m_MotionState == EEnemyMotionState::Stag) {
		m_MotionState = EEnemyMotionState::Wait;
	}
	//HP�����B���Ă���X�e�b�v�A�b�v
	if (StageLv == 0) {
		if (m_hitPoint <= m_Max_HitPoint * 0.9f) {
			StepUp0_1();
		}
	}

	if (m_hitPoint <= m_Max_HitPoint * 0.2f) {
		StepUp1_2();
	}
	


	//���Ԏ��U������
	if (StageLv == 1) {
		//�U���J�E���g������
		At -= _DeltaTime;
		//�~�܂�
		if (float PlayerDist = FVector::Dist(m_PlayerPos, GetActorLocation()) <= 200.0f) {
			AAIController* AICon = Cast<AAIController>(GetController());
			if (AICon) {
				AICon->StopMovement();
			}
		}
		else {
			if (m_AttackState != EGirlBossAttackState::Montage) {
				GetCharacterMovement()->MaxWalkSpeed = 300.0f * FMath::Sin(Timer);
			}
		}
		//�ړ����ɍU���J�E���g�����܂�����U��
		if (At <= 0.0f&&m_MotionState==EEnemyMotionState::Dash&&!NowMotion) {
			//��D�؂�Ă�����g�p�J�[�h�������Z�b�g���ă^�C�}�[�����Z�b�g����
			if (!Now_Hand.IsValidIndex(NowUsedCardsNum)&&NowUsedCardsNum!=0) {
				NowUsedCardsNum = 0;
				At = Max_At;
				UE_LOG(LogTemp, Warning, TEXT("Costs Hand End"));
				return;
			}

			//�^�[���J�n���Ɏ�D���[
			if (NowUsedCardsNum == 0) {
				DrewHand();
			}
			if (!Now_Hand.IsValidIndex(NowUsedCardsNum)) {
				UE_LOG(LogTemp, Warning, TEXT("Irregular Card Selected to Use"));
				return;
			}

			//�J�[�h��I�o
			int32 RandomValue = Now_Hand[NowUsedCardsNum];
			//�g�p�J�[�h�������Z
			++NowUsedCardsNum;
			//�U�����[�V����1���I���܂ōs�������b�N
			NowMotion = true;

			
			if (RandomValue == 0) {
				if (FVector::Dist(m_PlayerPos, GetActorLocation()) >= 600.0f) {
					//�U���p�^�[��A�F�r�b�g���G2����//////////////////////////////////////////////////////////
					//1�{��
					m_WeaponsInfo[0].Weapon->SetDamage(20);
					m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
					FVector Target = m_PlayerPos;
					Target.Z = 0.0f;
					m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
					m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
					SpawnCausionEffect(m_WeaponsInfo[0].Weapon->GetActorLocation());
					//2�{��
					FTimerHandle DuoAttackHandle;
					GetWorld()->GetTimerManager().SetTimer(DuoAttackHandle, [this]()
						{
							m_WeaponsInfo[1].Weapon->SetDamage(20);
							m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
							FVector Target = m_PlayerPos;
							Target.Z = 0.0f;
							m_WeaponsInfo[1].Weapon->StartAttack(Target, 1.0f, 0.5f);
							m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(5.0f);
							SpawnCausionEffect(m_WeaponsInfo[1].Weapon->GetActorLocation());
						}, 0.7f, false);

					//�s�����񕜃^�C�}�[��ݒ�
					FTimerHandle DuoAttackEndHandle;
					GetWorld()->GetTimerManager().SetTimer(DuoAttackEndHandle, [this]()
						{
							//�s��������
							NowMotion = false;
						}, 2.5f, false);

					/////////////////////////////////////////////////////////////////////////////////
				}
				else {

					//�������߂��Ȃ��A�؂�////////////////////////
					m_MotionState = EEnemyMotionState::Attack;
					m_AttackState = EGirlBossAttackState::Montage;
					m_attackPoint = 35;
					PlayAnimMantage(m_ProtDuoSlashMontage);
					/////////////////////////////////////////
				}
			}
			//�P���؂�////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 1) {
				m_MotionState = EEnemyMotionState::Attack;
				m_AttackState = EGirlBossAttackState::Montage;
				m_attackPoint = 35;
				PlayAnimMantage(m_ProtSlashMontage);
			}
			/////////////////////////////////////////////////////////////////////////////////////////////
			
			//2�A�؂�////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 2) {
				m_MotionState = EEnemyMotionState::Attack;
				m_AttackState = EGirlBossAttackState::Montage;
				m_attackPoint = 35;
				PlayAnimMantage(m_ProtDuoSlashMontage);
			}
			/////////////////////////////////////////////////////////////////////////////////////////////

			//�u�����N�{2�A����//////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 3) {
				if (FVector::Dist(m_firstPos.GetLocation(), GetActorLocation()) > FVector::Dist(m_firstPlayerPos, GetActorLocation())) {
					//�������W�Ƀ��[�v
					TeleportTo(m_firstPos.GetLocation(), GetActorRotation());
				}
				else {
					TeleportTo(m_firstPlayerPos, GetActorRotation());
				}
				//�s�����񕜃^�C�}�[��ݒ�
				FTimerHandle AHandle;
				GetWorld()->GetTimerManager().SetTimer(AHandle, [this]()
					{
						SpawnCausionEffect(m_WeaponsInfo[0].Weapon->GetActorLocation());
						//1�{��
						m_WeaponsInfo[0].Weapon->SetDamage(20);
						m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
						FVector Target = m_PlayerPos;
						Target.Z = 0.0f;
						m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
						m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
						//2�{��
						FTimerHandle DuoAttackHandle;
						GetWorld()->GetTimerManager().SetTimer(DuoAttackHandle, [this]()
							{
								SpawnCausionEffect(m_WeaponsInfo[1].Weapon->GetActorLocation());
								m_WeaponsInfo[1].Weapon->SetDamage(20);
								m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
								FVector Target = m_PlayerPos;
								Target.Z = 0.0f;
								m_WeaponsInfo[1].Weapon->StartAttack(Target, 1.0f, 0.5f);
								m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(5.0f);
							}, 0.7f, false);

						//�s�����񕜃^�C�}�[��ݒ�
						FTimerHandle DuoAttackEndHandle;
						GetWorld()->GetTimerManager().SetTimer(DuoAttackEndHandle, [this]()
							{
								//�s��������
								NowMotion = false;
							}, 2.5f, false);

					}, 1.0f, false);
				
				//��D�ɒǌ���ǉ�
				Now_Hand.Add(4);
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//��O2�A�؂�////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 4) {
				//�v���C���[�̑O�Ƀ��[�v
				FVector targetDire = (GetActorLocation() - m_PlayerPos).GetSafeNormal();
				TeleportTo(m_PlayerPos + targetDire * 200.0f,GetActorRotation());

				m_MotionState = EEnemyMotionState::Attack;
				m_AttackState = EGirlBossAttackState::Montage;
				m_attackPoint = 35;
				PlayAnimMantage(m_ProtDuoSlashMontage);
			}
			/////////////////////////////////////////////////////////////////////////////////////////////
		}
	}

}

//�U��
void AGirl_Boss::Attack() {

	if (StageLv == 0) {
		int32 RandomValue = FMath::RandRange(0, StageLv + 1);

		if (FVector::Dist(m_PlayerPos, GetActorLocation()) > 500.f) {
			RandomValue = 0;
		}
		else {
			RandomValue = 1;
		}
		if (RandomValue == 0) {
			//�U���p�^�[��A�F�r�b�g���G�ɕ���//////////////////////////////////////////////////////////
			if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			m_WeaponsInfo[0].Weapon->SetDamage(20);
			m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
			FVector Target = m_PlayerPos;
			Target.Z = 0.0f;
			m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
			m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
			//�^�C�}�[���Z�b�g
			Super::Attack();
			m_MotionState = EEnemyMotionState::Wait;
			/////////////////////////////////////////////////////////////////////////////////
		}
		else if (RandomValue == 2) {
			if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			if (m_WeaponsInfo[1].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			//1�{��
			m_WeaponsInfo[0].Weapon->SetAttacking(true);
			m_WeaponsInfo[0].Weapon->SetDamage(20);
			m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
			FVector Target = m_PlayerPos;
			Target.Z = 0.0f;
			m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
			m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
			//2�{��
			FTimerHandle DuoAttackHandle;
			GetWorld()->GetTimerManager().SetTimer(DuoAttackHandle, [this]()
				{
					m_WeaponsInfo[1].Weapon->SetAttacking(true);
					m_WeaponsInfo[1].Weapon->SetDamage(20);
					m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
					FVector Target = m_PlayerPos;
					Target.Z = 0.0f;
					m_WeaponsInfo[1].Weapon->StartAttack(Target, 1.0f, 0.5f);
					m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(5.0f);
					Super::Attack();
					m_MotionState = EEnemyMotionState::Wait;
				}, 0.7f, false);
		}
		else {
			//�U���p�^�[��B:��̃\�P�b�g�ɂ��U�邤///////////////////////////////////////
			if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			m_MotionState = EEnemyMotionState::Attack;
			m_AttackState = EGirlBossAttackState::Montage;
			m_attackPoint = 35;
			PlayAnimMantage(m_ProtSlashMontage);
			//�^�C�}�[���Z�b�g
			Super::Attack();
			//////////////////////////////////////////////////////////////////////
		}
	}
	
}

EAttackPattarnState AGirl_Boss::GetAttackPattarnState() {
	return EAttackPattarnState::DownAttack;
}

void AGirl_Boss::ForgetDamagedActor() {
	ClearAttackedActors();
}

void AGirl_Boss::StepUp0_1() {
	if (StageLv != 0) {
		return;
	}
	//2�{�ڂ̕�������
	if (!m_WeaponsInfo[1].Weapon) {
		return;
	}
	m_WeaponsInfo[1].Weapon->Initialize(this, m_WeaponsInfo[1].WaitHandle.HandHandleName, m_WeaponsInfo[1].WaitHandle.WaitTransform);

	//�����悤�ɕύX
	m_AttackRange = 1.0f;;

	StageLv = 1;
}

void AGirl_Boss::StepUp1_2() {
	if (StageLv != 1) {
		return;
	}
	//2�{�ڂ̕�������
	if (!m_WeaponsInfo[2].Weapon) {
		return;
	}
	if (!m_WeaponsInfo[3].Weapon) {
		return;
	}
	if (!m_WeaponsInfo[4].Weapon) {
		return;
	}
	if (!m_WeaponsInfo[5].Weapon) {
		return;
	}
	m_WeaponsInfo[2].Weapon->Initialize(this, m_WeaponsInfo[2].WaitHandle.HandHandleName, m_WeaponsInfo[2].WaitHandle.WaitTransform);
	m_WeaponsInfo[3].Weapon->Initialize(this, m_WeaponsInfo[3].WaitHandle.HandHandleName, m_WeaponsInfo[3].WaitHandle.WaitTransform);
	m_WeaponsInfo[4].Weapon->Initialize(this, m_WeaponsInfo[4].WaitHandle.HandHandleName, m_WeaponsInfo[4].WaitHandle.WaitTransform);
	m_WeaponsInfo[5].Weapon->Initialize(this, m_WeaponsInfo[5].WaitHandle.HandHandleName, m_WeaponsInfo[5].WaitHandle.WaitTransform);

	//�����悤�ɕύX
	m_AttackRange = 1.0f;

	StageLv = 2;
}

//��EnemyBase�ɈڐA���������
//��D�I�o
void AGirl_Boss::DrewHand() {
	//���Ⴆ��

	//�X�e�[�W���Ƃɕ�����
	if (StageLv == 0) {
		//�����P�p�^�[�������Ȃ�����
		return;
	}
	if (StageLv == 1) {
		//��D�����Z�b�g
		Now_Hand.Empty();
		//��D�R�X�g��
		int Costs=0;
		//��D����
		int CardsNum=0;
		//��D��I�o
		do {
			//�ő傪�R�D�����̗�������
			int32 RandomValue = FMath::RandRange(0, CardsDeck.Num());
			if (CardsDeck.IsValidIndex(RandomValue)) {
				//��D�ɓo�^
				Now_Hand.Add(CardsDeck[RandomValue]);
				//��D�����ƌ��݃R�X�g���X�V
				Costs += CardsData[CardsDeck[RandomValue]].Y;
				++CardsNum;
				//�f�o�b�N�p
				UE_LOG(LogTemp, Warning, TEXT("Costs_Size:%d"), Costs);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Irregular Cards Select To Hands"));
			}
		} while (Costs<=MaxHandsCosts&&CardsNum<HandsMax);

		//������̌Ăяo����낵����
		
	}
}


void AGirl_Boss::SpawnCausionEffect(FVector _pos) {
	if (!AttackScenceEffect) {
		return;
	}
	UNiagaraComponent* niagaraComp;

	niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackScenceEffect, _pos, GetActorRotation());
	niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor::Red * 50);
}


