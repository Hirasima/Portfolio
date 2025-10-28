// Fill out your copyright notice in the Description page of Project Settings.


#include "GirlsBitWeapon.h"
#include "Enemy_Base.h"
#include "Girl_Boss.h"
#include "TimerManager.h"

void AGirlsBitWeapon::Tick(float DeltaTime) {
	//�e
	Super::Tick(DeltaTime);
	UpdateMoving(DeltaTime);
}

void AGirlsBitWeapon::Initialize(class AEnemy_Base* _user, FName _socketName, FTransform _waitTransform) {
	SetTargetSocket(_user, _socketName);
	SetWaitTransform(_waitTransform);
	m_State = EBitWeaponState::Return;
	m_IsAutoFly = true;
	m_AttackedDulationTimer = 0.0f;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AGirlsBitWeapon::BeginPlay() {
	Super::BeginPlay();
	m_SinTimer = 0.0f;
	m_IsWait = true;
	GetPrimitiveComponent()->SetSimulatePhysics(false);
	//�U���q�b�g���֐���ݒ�
	GetPrimitiveComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGirlsBitWeapon::OnBitWeaponOverlap);
}

void AGirlsBitWeapon::SetTargetSocket(AEnemy_Base* _user, FName _socketName) {
	m_pUser = _user;
	HandleSocketName = _socketName;
}
void AGirlsBitWeapon::SetWaitTransform(FTransform _waitTransform) {
	m_TargetWaitPos = _waitTransform;
}

void AGirlsBitWeapon::SetBitMoveState(EBitWeaponState _state) {
	m_State = _state;
}

EBitWeaponState AGirlsBitWeapon::GetBitMoveState() {
	return m_State;
}

void AGirlsBitWeapon::SetMoveSpeedAccelerator(float _MoveSpeed) {
	m_nowSpeed = MoveSpeed * _MoveSpeed;
}

void AGirlsBitWeapon::SetDamage(int _damage) {
	m_Damage = _damage;
}
void AGirlsBitWeapon::SetAttackType(EAttackPattarnState _type) {
	m_AttackState = _type;
}

void AGirlsBitWeapon::StartAttack(FVector _targetPos, float AttackDulation, float Dulation) {
	if (m_State != EBitWeaponState::Wait) {
		return;
	}
	m_TargetAttackPos = _targetPos;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FRotator Target = (_targetPos - GetActorLocation()).Rotation();
	Target.Yaw -= 90;
	SetActorRotation(Target);
	m_AttackedDulation = AttackDulation;
	m_AttackedDulationTimer = 0.0f;
	m_IsAutoFly = true;
	FTimerHandle AtHandle;
	GetWorld()->GetTimerManager().SetTimer(AtHandle, [this,_targetPos,AttackDulation,Dulation]()
		{
			m_State = EBitWeaponState::Go;
			SetAttacking(true);
			m_attackedActors.Empty();
		}, Dulation, false);
}

void AGirlsBitWeapon::SetIsWait(bool _flag) {
	m_IsWait = _flag;
}

//�����I�Ɏ茳�ɖ߂�֐�
void AGirlsBitWeapon::HandWarpQuickly() {
	m_IsAutoFly = false;
	m_IsWait = false;
	m_State = EBitWeaponState::Wait;
	AttachToComponent(m_pUser->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandleSocketName);

	SetActorRelativeTransform(m_TargetWaitPos);
}

void AGirlsBitWeapon::UpdateMoving(float DeltaTime) {
	//���[�U�[�����Ȃ��Ȃ�ӂ�ӂ킵�Ȃ�
	if (!m_pUser) {
		return;
	}
	//�\�P�b�g�ɊǗ�����Ă���Ȃ痬��
	if (m_IsAutoFly == false) {
		if (m_IsWait==true) {
			//�㉺�ɗh�炷
			m_SinTimer += DeltaTime;
			FVector Pos = GetActorLocation();
			Pos.Z += 0.7f * FMath::Sin(m_SinTimer * 2);
			SetActorLocation(Pos);
			return;
		}
	}
	//�U��������Ȃ��Ȃ�ҋ@���W�ɋA�낤�Ƃ���
	if (m_State == EBitWeaponState::Return) {
		//���������ȓ��Ȃ�ҋ@�\�P�b�g�ɂ�����
		FVector Soc = m_pUser->GetMesh()->GetSocketLocation(HandleSocketName);
		if (FVector::Dist(GetActorLocation(), Soc )<200.0f) {
			AttachToComponent(m_pUser->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandleSocketName);
			SetActorRelativeTransform(m_TargetWaitPos);
			m_State = EBitWeaponState::Wait;
			m_IsAutoFly = false;
			//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Ret_Fin"));
			return;
		}

		//�ҋ@�ꏊ�ɖ߂鏈��
		FVector CurrentPos = GetActorLocation();
		FVector NextPos = FMath::VInterpTo(CurrentPos, Soc, DeltaTime, MoveSpeed);
		FRotator NextRot = FMath::RInterpTo(GetActorRotation(), m_TargetWaitPos.GetRotation().Rotator(), DeltaTime, MoveSpeed);
		SetActorLocation(NextPos);
		SetActorRotation(NextRot);
		//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Ret"));
		return;
	}

	//�U�����Ȃ�U���ڕW���W�ɔ��ł���
	if (m_State == EBitWeaponState::Go) {
		//�U���Ώۍ��W�Ȃ�
		if (FVector::Dist(GetActorLocation(), m_TargetAttackPos) < 10.0f) {
			m_AttackedDulationTimer += DeltaTime;
			//�����u���Y�p
			m_attackedActors.Empty();
			m_AttackState = EAttackPattarnState::None;
			if (m_AttackedDulationTimer >= m_AttackedDulation) {
				//�]�C��ɖ߂�
				m_State = EBitWeaponState::Return;
				SetAttacking(false);
			}
			//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Go_Fin"));
			return;
		}

		//�U���ΏۂɌ���������
		FVector CurrentPos = GetActorLocation();
		FVector NextPos = FMath::VInterpTo(CurrentPos, m_TargetAttackPos, DeltaTime, m_nowSpeed);
		SetActorLocation(NextPos);
		//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Go"));
		return;
	}
}


//����̏d�Ȃ������̃C�x���g
void AGirlsBitWeapon::OnBitWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult)
{
	//���g�ɓ������Ă����疳��
	if (_otherActor == this||_otherActor==m_pUser)
	{
		return;
	}

	if (m_IsAutoFly == false) {
		m_pUser->OnWeaponOverlap(_overlappedComponent, _otherActor, _otherComp, _otherBodyIndex, _bFromSweep, _sweepResult);
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

		//�U���ς݃A�N�^�[�ɒǉ�
		m_attackedActors.Add(fighter);


		//��U�����̃C�x���g�֐����Ăяo���A��Ԃ��󂯎��
		attackReaction = fighter->OnHitAttack(m_Damage, 55, m_AttackState, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("State: % s"), *UEnum::GetValueAsString(m_AttackState));

		//UE_LOG(LogTemp, Warning, TEXT("%s : %s Reaction is %s"), *this->GetName(), *fighter->GetName(), *UEnum::GetValueAsString(attackReaction));

		//�p���B����Ă�����
		/*if (attackReaction == EAttackReaction::Parry)
		{
			�p���B���C�x���g�֐�
			Parried();
		}*/
	}
}
