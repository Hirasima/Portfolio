// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterCharacter.h"
#include "FighterPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFighterCharacter::AFighterCharacter()
	:m_AttackBuffRate(1.0f)
	,m_montagePlayRate(1.0f)
	,m_bEnableCritical(true)
	,m_bIsKnockUpping(false)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void AFighterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//����̏�����
	if (m_pWeaponClass) {
		m_pWeapon = GetWorld()->SpawnActor<AWeapon>(m_pWeaponClass);
		m_pWeapon->GetRootComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("R_WeaponSocket"));
		m_pWeapon->m_pWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AFighterCharacter::OnWeaponOverlap);
		m_pWeapon->SetAttacking(false);
	}

	//��̏�����
	if (m_pSheathClass) {
		m_pSheath = GetWorld()->SpawnActor<AWeapon>(m_pSheathClass);
		m_pSheath->GetRootComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("L_WeaponSocket"));
		m_pSheath->m_pWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AFighterCharacter::OnWeaponOverlap);
		m_pSheath->SetAttacking(false);
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	UE_LOG(LogTemp, Log, TEXT("Channel"));

	//�A�j���C���X�^���X�̎擾
	m_pAnimInstance = GetMesh()->GetAnimInstance();
	//�A�j���C���X�^���X�̃k���`�F�b�N
	if (!m_pAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("%s PlayPawn : Anim Instance Is None"), *this->GetName());
	}

	//HP�̏�����
	m_hitPoint = m_Max_HitPoint;
	//UE_LOG(LogTemp, Warning, TEXT("HP Init : %d"), m_hitPoint);
}

// Called every frame
void AFighterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�m�b�N�o�b�N�X�V����
	UpdateKnockBack(DeltaTime);

	//�q�b�g�|�C���g���O�ȉ��ł���΁uDead�v�^�O��ǉ�
	if (m_hitPoint <= 0)
	{
		Tags.Add("Dead");
	}
}

// Called to bind functionality to input
void AFighterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//�U�����ꂽ�A�N�^�[�̃��X�g�̒��g������
void AFighterCharacter::ClearAttackedActors()
{
	m_attackedActors.Empty();
}

//�G�t�F�N�g���o��
void AFighterCharacter::SpawnEffect(EAttackReaction _attackReaction)
{
	
}

//�m�b�N�o�b�N���X�V�֐������V���M
void AFighterCharacter::UpdateKnockBack(float DeltaTime) {
	if (!bIsKnockBack) {
		return;
	}

	//�m�b�N�o�b�N���W�̌v�Z�Ɣ��f
	KnockBackElapsed += DeltaTime;
	float Alpha = FMath::Clamp(KnockBackElapsed / KnockBackDuration, 0.f, 1.0f);
	FVector NextPos = FMath::Lerp(KnockBackStartPos, KnockBackEndPos, Alpha);
	SetActorLocation(NextPos);

	//�m�b�N�o�b�N�̏C���m�F
	if (Alpha >= 1.0f) {
		bIsKnockBack = false;
	}
}

void AFighterCharacter::PlayAnimMantage(UAnimMontage* _animMontage)
{
	//�k���`�F�b�N
	if (!_animMontage)
	{
		return;
	}

	//�A�j���[�V���������^�[�W�����Đ�
	m_pAnimInstance->Montage_Play(_animMontage, m_montagePlayRate);
}

//---------------------------------------------------
// �Q�b�^�[ �E�@�Z�b�^�[
//---------------------------------------------------

//�U���̎�ނ̎擾
EAttackPattarnState AFighterCharacter::GetAttackPattarnState()
{
	return EAttackPattarnState::None;
}

//���݂̏�ԂɑΉ�����AttackAsset��Ԃ�
FAttackAsset AFighterCharacter::GetAttackAsset()
{
	FAttackAsset attackAsset;

	return attackAsset;
}

//�K�[�h���x�����͂�Ԃ�
int AFighterCharacter::GetGuardBreakPower()
{
	return 20;
}

//�ł��グ�U���\���Ԃ�
bool AFighterCharacter::GetCanKnockUp()
{
	return false;
}

//���݂�HP������Ԃ�
float AFighterCharacter::GetHPRatio()
{
	float ratio = (float)m_hitPoint / (float)m_Max_HitPoint;

	//UE_LOG(LogTemp, Log, TEXT("HP Ratio : %f"), ratio);

	return ratio;
}

//----------------------------------------------------
// �C�x���g�֐�
//----------------------------------------------------

//�U�����󂯂��Ƃ�
EAttackReaction AFighterCharacter::OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp)
{
	//�ł��グ�U����������
	if (_canKnockUp)
	{
		m_bIsKnockUpping = true;
		UE_LOG(LogTemp, Log, TEXT("%s : KnockUpping!!!!!!!!!!!!!!"), *this->GetName());
	}

	return EAttackReaction::None;
}

//��_���[�W��
void AFighterCharacter::Damaged(int _damagePoint)
{
	m_hitPoint -= _damagePoint;
}

//��v���U����
void AFighterCharacter::OnCriticalAttacked(int _damage)
{
	Damaged(_damage);

	UE_LOG(LogTemp, Log, TEXT("Critical Attack Damaged"));
}

//��p���B��
void AFighterCharacter::Parried()
{
	//����̍U����Ԃ��L���ɂ���
	m_pWeapon->SetCollisionActive(false);
}

//��m�b�N�o�b�N��
void AFighterCharacter::KnockBacked(FVector _direction, float _Time) {
	KnockBackStartPos = GetActorLocation();
	KnockBackEndPos = KnockBackStartPos + _direction;
	KnockBackDuration = _Time;
	KnockBackElapsed = 0.0f;
	bIsKnockBack = true;
}

//�U���q�b�g���̃C�x���g�֐�
void AFighterCharacter::OnAttackHitEvent()
{

}

//----------------------------------------------------
//�Փ˔���֌W
//----------------------------------------------------

//����̏d�Ȃ������̃C�x���g
void AFighterCharacter::OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
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
		
		//�K�[�h�ɑ΂���j���
		int breakPower = GetGuardBreakPower();

		//�ł��グ�U����
		bool canKnockUp = GetAttackAsset().CanKnockUp;

		//�U���ς݃A�N�^�[�ɒǉ�
		m_attackedActors.Add(fighter);

		//�U���͂��擾
		int attackPoint = (int)((float)GetAttackAsset().AttackPoint * m_AttackBuffRate);

		//��U�����̃C�x���g�֐����Ăяo���A��Ԃ��󂯎��
		attackReaction = fighter->OnHitAttack(attackPoint, breakPower, attackState, Cast<AFighterCharacter>(this), canKnockUp);
		//UE_LOG(LogTemp, Log, TEXT("%s : Attack Point Is : %d"), *this->GetName(), attackPoint);
		//UE_LOG(LogTemp, Warning, TEXT("%s : %s Reaction is %s"), *this->GetName(), *fighter->GetName(), *UEnum::GetValueAsString(attackReaction));

		OnAttackHitEvent();

		//�p���B����Ă�����
		if (attackReaction == EAttackReaction::Parry)
		{
			//�p���B���C�x���g�֐�
			Parried();
		}
	}
}

//��������������֐������V���M
void AFighterCharacter::FaceTargetLocation(const FVector _targetLocation) {
	FVector Dir = _targetLocation - GetActorLocation();
	Dir.Z = 0.0f;

	FRotator Rot = Dir.Rotation();

	FRotator NextRot = GetActorRotation();
	NextRot.Yaw = Rot.Yaw;

	SetActorRotation(NextRot);
}