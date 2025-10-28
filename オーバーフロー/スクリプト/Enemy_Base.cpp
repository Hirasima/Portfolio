// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Base.h"
#include "Components/CapsuleComponent.h"
#include "EnemyHpBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Weapon.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include"Engine/StaticMeshActor.h"
#include "GameFramework/Controller.h"
#include"NavigationSystem.h"
#include "AIController.h"
#include "TimerManager.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "PlayPawn.h"
#include "NiagaraComponent.h"

// Sets default values
AEnemy_Base::AEnemy_Base()
	:m_Situation(EEnemySituationState::Patrol), m_MotionState(EEnemyMotionState::Wait), m_isAlartPoin(false), m_StagState(EStagType::None)
{	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Enemy�^�O��ǉ�
	Tags.Add("Enemy");
}

// Called when the game starts or when spawned
void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	//�v������
	m_bEnableCritical = false;

	//�����ʒu��ۑ�
	if (m_IsAnotherFirstPos==false) {
		m_firstPos = GetTransform();
	}

	//�X�e�[�^�X�̏�����
	Mp = Max_Mp;
	At = 0;
	//�K�[�h���Ԃ�������
	m_guardTime = 0.0f;

	//�ʂ����Ԃ�
	if (m_pFaceMesh) {
		// StaticMeshComponent�𐶐�
		UStaticMeshComponent* FaceMeshComp = NewObject<UStaticMeshComponent>(this);

		// ���b�V����ݒ�
		FaceMeshComp->SetStaticMesh(m_pFaceMesh);

		// Mobility��Movable�Ɂi�A�^�b�`��Transform�ύX�̂��߁j
		FaceMeshComp->SetMobility(EComponentMobility::Movable);

		// ���L�A�N�^�[�ɓo�^�i�o�^���Ȃ��ƕ\������Ȃ��j
		FaceMeshComp->RegisterComponent();

		// �\�P�b�g�ɃA�^�b�`�i��: HeadSetSocket�j
		FaceMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("HeadSetSocket"));
	}

}

//�G�l�~�[�̏󋵂�ݒ肷��֐�
void AEnemy_Base::SetEnemySituation(EEnemySituationState _situ) {
	m_Situation = _situ;
}

void AEnemy_Base::BeAlarm(FVector _alartPos) {
	m_IsAlart = true;
	m_isAlartPoin = false;
	//�x���ʒu��`�B
	m_alartPos = _alartPos;
}
void AEnemy_Base::EndAlarm() {
	m_IsAlart = false;
}

//�G�l�~�[�̗\�m���Ă���U����ݒ肷��֐�
void AEnemy_Base::SetScenceOfPlayerAttack(EAttackPattarnState _pattarn) {
	m_NowScenceOfPlayerAttack = _pattarn;
	UE_LOG(LogTemp, Warning, TEXT("GETTT"));
}

//�G�l�~�[�̍X�V�֐�*�G�l�~�[�}�l�[�W���̂݌Ăяo��*
void AEnemy_Base::EnemyTick(float DeltaTime,FVector PlayerPos)
{
	Super::Tick(DeltaTime);

	if (m_bIsKeepZAxis==true&&bKnockBack==true) {
		SetActorLocation(KeepPos);
		return;
	}
	//����ł���or�m�b�N�o�b�N���͉������Ȃ�
	if (m_Situation == EEnemySituationState::Dead || bKnockBack == true) {
		return;
	}

	//�����u�����S����
	//HP��0�Ȃ玀��ԂɂȂ�
	if (m_hitPoint <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
		//�󋵓o�^
		m_Situation = EEnemySituationState::Dead;
		//�A�j���[�V�����p�o�^������
		m_MotionState = EEnemyMotionState::Dead;
		PlayAnimMantage(m_pDeadMontage);
		return;
	}


	//�v�������������Ȃ�
	if (m_MotionState == EEnemyMotionState::Assacinated) {
		return;
	}

	//�v���C���[�̍��W��ۑ�
	m_PlayerPos = PlayerPos;

	//�K����
	if (m_LookPlayer) {
		LookforPlayer();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//���݂̏󋵂ɍ��킹��Tick�����𕪂���
	switch (m_Situation)
	{
	case EEnemySituationState::Battle:
		InBattleMove();
		//�U���^�C�}�[��i�߂�
		if (At > 0) {
			At -= DeltaTime;
		}
		//�t���[���p�҂̂��߂̃t���[�X�V����
		UpdateFlow(DeltaTime);
		break;
	case EEnemySituationState::Patrol:
		Patrol();
		break;
	default:
		break;
	}
	/////////////////////////////////////////////////////////////////////////////////////////


	// �A�j���[�V������ԕʏ������֐������悤�˂�////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�͂�����d����Ԃ̏I���m�F�Ǝ��ԉ��Z//////////////////////////////////////////////////////////////////////
	if (m_bParried)
	{
		//�e���ꎞ�Ԃɉ��Z
		m_parriedTime += DeltaTime;

		m_MotionState = EEnemyMotionState::Parried;
		PlayAnimMantage(m_pUpParriedMontage);

		//�e����I���m�F
		//�t���[���͒Z��
		if (m_IsFlow) {
			if (m_parriedTime >= m_parriedMaxTime * 0.5f)
			{
				//�A�j���[�V������ҋ@��
				m_MotionState = EEnemyMotionState::Wait;

				//����̋O���𖳌���
				m_pWeapon->SetBladeOrbitActive(false);

				//�e���ꎞ�Ԃ�������
				m_parriedTime = 0.0f;
				m_bParried = false;

				UE_LOG(LogTemp, Warning, TEXT("Enemy : Parried Finish"));
			}
		}
		//��t���[��
		else {
			if (m_parriedTime >= m_parriedMaxTime)
			{
				//�A�j���[�V������ҋ@��
				m_MotionState = EEnemyMotionState::Wait;

				//����̋O���𖳌���
				m_pWeapon->SetBladeOrbitActive(false);

				//�e���ꎞ�Ԃ�������
				m_parriedTime = 0.0f;
				m_bParried = false;

				UE_LOG(LogTemp, Warning, TEXT("Enemy : Parried Finish"));
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//�ҋ@���W��ݒ肷��֐�
void AEnemy_Base::SetWaitPosition(FTransform _targetPos) {
	m_firstPos = _targetPos;
}

//���񒆁i��퓬���j�̋���
void AEnemy_Base::Patrol() {
	//���F�͈͂Ƀv���C���[�����邩�`�F�b�N
	if (PlayerFindCheck()) {
		if (RegisterToBattleMngCallBack.IsBound()) {
			RegisterToBattleMngCallBack.Execute(this);
			m_IsAlart = false;
			return;
		}
	}

	//�ÈŒ��͉����ł��Ȃ�
	if (m_IsBlind) {
		Cast<AAIController>(GetController())->StopMovement();
		return;
	}

	//�x����ԂȂ�ړI�n��ڎw��
	if (m_IsAlart) {
		//�x���ʒu�ɒ��������Ƃ��Ȃ��Ȃ�
		if (!m_isAlartPoin) {
			//�x���ʒu�Ƃ̋��������ȓ��Ȃ�x��������
			if (float Dist = FVector2D::Distance(FVector2D(m_alartPos.X,m_alartPos.Y), FVector2D(GetActorLocation().X,GetActorLocation().Y)) < 200.0f) {
				// �w�莞�Ԍ�Ɍ��ɖ߂��^�C�}�[���Z�b�g
				GetWorld()->GetTimerManager().SetTimer(
					AlartEndTimerHandle,
					this,
					&AEnemy_Base::EndAlarm,
					5.0f, // �� �����Ԃŉ��b�ɂ��邩
					false
				);
				m_isAlartPoin = true;
				m_MotionState = EEnemyMotionState::Wait;
				//�~�܂�
				AAIController* AICon = Cast<AAIController>(GetController());
				if (AICon) {
					AICon->StopMovement();
				}
				//���茩��ׂ�
				SetActorRotation(GetActorRotation() * -1);
				return;
			}
		}
		else {
			//������낫��낵���݂͂���
			m_MotionState = EEnemyMotionState::Wait;
			return;
		}

		//�����ʒu������
		FVector Direction = (m_alartPos - GetActorLocation()).GetSafeNormal();
		FRotator Rotation = Direction.Rotation();
		Rotation.Pitch = 0;
		SetActorRotation(Rotation);
		//�����ʒu�Ɉړ�
		AAIController* AICon = Cast<AAIController>(GetController());
		if (AICon) {
			AICon->MoveToLocation(m_alartPos);
		}
		//�\�Ȃ�ړ��A�j���[�V�����ɂ��������͂���
		m_MotionState = EEnemyMotionState::Dash;
		return;
	}

	//�x��������Ȃ��Ȃ猳�̏ꏊ�ɖ߂�
	if (float Dist = FVector2D::Distance(FVector2D(m_firstPos.GetLocation().X, m_firstPos.GetLocation().Y), FVector2D(GetActorLocation().X,GetActorLocation().Y)) < 200.0f) {
		m_MotionState = EEnemyMotionState::Wait;
		FRotator Rotation = m_firstPos.GetRotation().Rotator();
		Rotation.Pitch = 0;
		SetActorRotation(Rotation);
		return;
	}
	//�����ʒu������
	FVector Direction = (m_firstPos.GetLocation() - GetActorLocation()).GetSafeNormal();
	FRotator Rotation = Direction.Rotation();
	Rotation.Pitch = 0;
	SetActorRotation(Rotation);
	//�����ʒu�Ɉړ�
	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon) {
		AICon->MoveToLocation(m_firstPos.GetLocation());
	}
	//�\�Ȃ�ړ��A�j���[�V�����ɂ��������͂���
	m_MotionState = EEnemyMotionState::Dash;
	return;
}

//���E�Ƀv���C���[�����邩�`�F�b�N
bool AEnemy_Base::PlayerFindCheck() {

	//�����`�F�b�N
	if (FVector::Dist(m_PlayerPos, GetActorLocation())>GetViewLange()) {
		return false;
	}

	//����p�`�F�b�N���}�W�b�N�i���o�[�Ŏ���p�X�O�ɂȂ��Ă�
	FVector ToTargetDirection = (m_PlayerPos - GetActorLocation()).GetSafeNormal();
	//�h�b�g��
	float Dot=FVector::DotProduct(GetActorForwardVector(), ToTargetDirection);
	//�p�x���f�O���[�ɕϊ�
	float TargetDegree = FMath::Acos(Dot) * (180.f / PI);
	if (TargetDegree > 180 / 2.0f) {
		return false;
	}

	//�Օ��`�F�b�N
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), m_PlayerPos, ECC_Visibility,Params);
	if (!bHit) {
		return true;
	}
	if (bHit && HitResult.GetActor()) {
		if (HitResult.GetActor()->ActorHasTag(FName("Player"))) {
			return true;
		}
	}

	return false;
}

//���S�A�j���[�V�����I�����ɌĂԂ�i���������j
void AEnemy_Base::DeleteOut() {
	//���S������
	if (m_Situation == EEnemySituationState::Dead) {
		//�G�l�~�[�}�l�[�W��������
		if (UnregisterFromEnemyMngCallBack.IsBound()) {
			UnregisterFromEnemyMngCallBack.Execute(this);
		}
		//�o�g���}�l�[�W��������
		if (UnregisterFromBattleMngCallBack.IsBound()) {
			UnregisterFromBattleMngCallBack.Execute(this);
		}
		if (m_pWeapon)
		{
			m_pWeapon->Destroy();
		}
		//�폜
		this->Destroy();

		return;
	}
}

//���E�͈͂��擾����֐�
float AEnemy_Base::GetViewLange() {
	float ViewDist = (m_IsBlind) ? m_BlindBattleSearchRange : m_BattleSearchRange;
	return ViewDist;
}

//�퓬���̈ړ�����
void AEnemy_Base::InBattleMove() {
	//�s�����͈ړ����Ȃ�
	if (m_MotionState != EEnemyMotionState::Wait && m_MotionState != EEnemyMotionState::Dash) {
		return;
	}

	//�͈͊O�Ȃ�o�g���𔲂���
	CheckOutBattleManager(m_PlayerPos);
	if (m_Situation != EEnemySituationState::Battle) {
		return;
	}

	//�v���C���[�̕�������
	LookforPlayer();

	//�v���C���[�Ƃ̋����𑪒�
	float PlayerDist = FVector::Dist(m_PlayerPos, GetActorLocation());
	//�߂�����Ȃ痣���
	if (PlayerDist < m_NearestRange) {
		//�v���C���[�̋t�����Ɉړ�
		SetActorLocation(GetActorForwardVector() * -m_walkSpeed *5.0f* UGameplayStatics::GetGlobalTimeDilation(GetWorld()) + GetActorLocation());
		//���o�b�N�X�e�b�v�ɂ����݂͂���
		m_MotionState = EEnemyMotionState::Dash;
		return;
	}
	//�U���͈͂�艓���ɂ���Ȃ�ړ�
	if (PlayerDist > m_AttackRange) {
		//�v���C���[�̕����Ɉړ�
		AAIController* AICon = Cast<AAIController>(GetController());
		if (AICon) {
			AICon->MoveToLocation(m_PlayerPos);
		}
		//�\�Ȃ�ړ��A�j���[�V�����ɂ��������͂���
		m_MotionState = EEnemyMotionState::Dash;
		return;
	}
	//���傤�ǂ��������Ȃ�퓬
	//wait�ɂ��Ă���
	m_MotionState = EEnemyMotionState::Wait;
	//�~�܂�
	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon) {
		AICon->StopMovement();
	}
	EnemyBattleAnimTick();

}

//�G�l�~�[�̐퓬�A�j���[�V�����X�e�[�g�؂�ւ�
void AEnemy_Base::EnemyBattleAnimTick() {

	//�ҋ@�A�������Ȃ�X�e�[�g��؂�ւ���
	if (m_MotionState == EEnemyMotionState::Wait || m_MotionState == EEnemyMotionState::Dash) {
		//���Ԃ����Ă���U��(�D��x1)
		Attack();
	}
}

//�A�j���[�V���������^�[�W���擾�֐�
UFUNCTION(BlueprintCallable)
UAnimMontage* AEnemy_Base::GetNowTargetMontage() {
	UAnimMontage* monta = nullptr;

	//�A�j���[�V�����󋵂ɂ���ď����𕪕�
	switch (m_MotionState)
	{
	case EEnemyMotionState::Wait:
		break;
	case EEnemyMotionState::Dash:
		break;
	case EEnemyMotionState::UpGuard:
		break;
	case EEnemyMotionState::DownGuard:
		break;
	case EEnemyMotionState::Parried:
		break;
	case EEnemyMotionState::Damaged :
		break;
	case EEnemyMotionState::Attack:
		monta = GetAttackMontage();
		break;
	case EEnemyMotionState::Stag:
		monta = GetStagMontage();
		break;
	case EEnemyMotionState::Dead:
		monta = m_pDeadMontage;
		break;
	case EEnemyMotionState::Assacinated:
		monta = m_pAssacinatedMontage;
		break;
	default:
		break;
	}

	return monta;
}

//�U�����ɃA�j���[�V���������^�[�W�����擾����֐�
UFUNCTION(BlueprintCallable)
UAnimMontage* AEnemy_Base::GetAttackMontage() {
	return nullptr;
}

//�U�����ɃA�j���[�V���������^�[�W�����擾����֐�
UFUNCTION(BlueprintCallable)
UAnimMontage* AEnemy_Base::GetStagMontage() {
	UAnimMontage* Monta = nullptr;

	switch (m_StagState)
	{
	case EStagType::None:
		break;
	case EStagType::SmallStag:
		Monta = m_pMiniStagMontage;
		break;
	case EStagType::BigStag:
		Monta = m_pBigStagMontage;
		break;
	case EStagType::Down:
		break;
	default:
		break;
	}
	return Monta;
}

//���݂̃��[�V�����󋵂��擾����֐�
UFUNCTION(BlueprintCallable)
EEnemyMotionState AEnemy_Base::GetNowMotionState()const {
	return m_MotionState;
}

//���݂̃��[�V�����󋵂�ݒ肷��֐�
UFUNCTION(BlueprintCallable)
void AEnemy_Base::SetNowMotionState(EEnemyMotionState _state) {
	m_MotionState = _state;
}

//�G�l�~�[�̐퓬�X�V�֐�*�o�g���}�l�[�W������̂݌ĂԂ���*
void AEnemy_Base::EnemyBattleTick(float _DeltaTime, FVector _PlayerPos) {

}

//�o�g���}�l�[�W�����痣�E����֐�
void AEnemy_Base::CheckOutBattleManager(FVector _PlayerPos) {
	float PlayerDist = FVector::Dist(GetActorLocation(), _PlayerPos);
	//�v���C���[�Ƃ̋��������E��艓���Ȃ�//////////////////////////////////////////////////////
	
	//�����󋵂Ŏ��E�����肷��
	float ViewDist = (m_IsBlind) ? m_BlindBattleSearchRange : m_BattleSearchRange;

	if (PlayerDist >= ViewDist) {
		//�ҋ@��
		m_Situation = EEnemySituationState::Patrol;
		m_MotionState = EEnemyMotionState::Wait;

		//�o�g���}�l�[�W��������
		if (UnregisterFromBattleMngCallBack.IsBound()) {
			UnregisterFromBattleMngCallBack.Execute(this);
		}
		return;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//�v���C���[�̕��������֐�
void AEnemy_Base::LookforPlayer() {
	FVector Direction = (m_PlayerPos - GetActorLocation()).GetSafeNormal();
	FRotator Rotation = Direction.Rotation();
	Rotation.Pitch = 0;
	SetActorRotation(Rotation);
}

//�Ɩ��󋵂̓`�B�֐�
void AEnemy_Base::SetIsBlind(bool _isBlind) {
	m_IsBlind = _isBlind;
}


//HP�擾�֐�
int AEnemy_Base::GetEnemyHp() {
	return m_hitPoint;
}

//�U�������A�N�^�[�����
void AEnemy_Base::ClearAttackedActors() {
	m_attackedActors.Empty();
}

//�U����
void AEnemy_Base::OnAttack()
{
	m_pWeapon->SetAttacking(true);
	//SE�Đ�
	UGameplayStatics::PlaySoundAtLocation(this, AttackSE, GetActorLocation(), SE_Volume);
}

//�U���I����
void AEnemy_Base::OnFinishAttack()
{
	m_pWeapon->SetAttacking(false);
	
	m_MotionState = EEnemyMotionState::Wait;

	ClearAttackedActors();

	m_LookPlayer = false;
}

//�U���\���X�|�[��
void AEnemy_Base::SpawnAttackScenceEffect()
{
	if (!AttackScenceEffect) {
		return;
	}
	UNiagaraComponent* niagaraComp;

	niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackScenceEffect, m_pWeapon->GetActorLocation(), GetActorRotation());
	niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor::Yellow*50);
}

//�U�����󂯂��Ƃ��ɌĂ΂��֐�
EAttackReaction AEnemy_Base::OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp) {
	//�U�����̏�Ԃ��L���łȂ����
	if (_attackState == EAttackPattarnState::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy : Attack Pattarn Is None"));
		return EAttackReaction::None;
	}
	if (m_Situation == EEnemySituationState::Dead) {
		return EAttackReaction::None;
	}

	//�U�����A�N�V�����̖߂�l�p
	EAttackReaction attackReaction = EAttackReaction::None;

	bool isDamage(true);
	bool isParried(false);

	//�K�[�h��v�Ȃ�_���[�W�͎󂯂Ȃ�:�������[�N�Ώ�/////////////////////////
	//bool CheckGuardAttack()�ɂ���/////////////////////////////////////////////////////////////////////////////
	if (m_MotionState == EEnemyMotionState::UpGuard && _attackState == EAttackPattarnState::UpAttack&& _canKnockUp == false)
	{
		isDamage = false;
	}
	else if (m_MotionState == EEnemyMotionState::UpGuard && _attackState == EAttackPattarnState::DownAttack&&_canKnockUp==false)
	{
		isDamage = false;
	}

	if (_canKnockUp == true) {
		isDamage = true;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////

	//�_���[�W�̗L���ɂ�鏈������i��e���̉e�������j////////////////////////////////////////////////////////////////
	//�_���[�W���󂯂Ă�����
	if (isDamage&&m_hitPoint>0)
	{
		//���A�N�V�����𓖂�������
		attackReaction = EAttackReaction::Hit;
		//�L�[�v����
		m_bIsKeepZAxis = false;
		//�m�b�N�A�b�v�U���Ȃ�
		if (_canKnockUp == true&& m_CanKnockUp&&m_hitPoint>_damage) {
			bKnockBack = true;
			m_MotionState = EEnemyMotionState::Stag;
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCharacterMovement()->Velocity.X = 0;
			GetCharacterMovement()->Velocity.Y = 0;
			GetCharacterMovement()->SetPlaneConstraintEnabled(false);
			


			if (m_bIsKnockUpping == true) {
				PlayAnimMantage(m_pKnockUpKeepMontage);
				
			}else{
				PlayAnimMantage(m_pKnockUpMontage);
			}
			Super::OnHitAttack(_damage, _breakPower, _attackState, _attacker, _canKnockUp);

		}
		else {

			//��P��������i��퓬��ԂȂ�j�Ƃ肠�����v���C���[�̕�������
			if (m_Situation == EEnemySituationState::Patrol) {
				LookforPlayer();
			}
			//�����u����낯����
			//�t���[��or���͂�낯�Ȃ�
			if (!m_IsFlow && m_MotionState != EEnemyMotionState::Stag && m_hitPoint > _damage) {
				//�ꉞ�U��������~�ی�
				OnFinishAttack();
				//���������Ȃ��Ȃ�
				GetCharacterMovement()->StopMovementImmediately();
				if (_attackState == EAttackPattarnState::UpAttack) {
					m_MotionState = EEnemyMotionState::Stag;
					m_StagState = EStagType::BigStag;
				}
				else {
					m_MotionState = EEnemyMotionState::Stag;
					m_StagState = EStagType::SmallStag;
				}
				PlayAnimMantage(GetStagMontage());
				//�p���[�[���m�b�N�o�b�N����
				bKnockBack = true;
			}
		}
		Damaged(_damage);

		//SE�Đ�
		UGameplayStatics::PlaySoundAtLocation(this, DamageSE, GetActorLocation(), SE_Volume);
	}
	//�K�[�h�ł��Ă�����
	else if(m_hitPoint > 0)
	{
		//���A�N�V������h���
		attackReaction = EAttackReaction::Guard;
		//�t���[�Q�[�W�̌����Ɠ`�B
		//�t���[�Q�[�W�����炷
		Mp -= GuardGein_Mp;

		if (Mp <= 0) {
			Mp = 0;
		}

		//�J�E���^�[���[�V�����ɔh��
		if (GuardCounterAttack.AnimMantage&&m_hitPoint>0) {
			m_MotionState = EEnemyMotionState::Wait;
			PlayAnimMantage(GuardCounterAttack.AnimMantage);
			m_MotionState = EEnemyMotionState::Attack;
			m_combo = 99;
		}

		//SE�Đ�
		UGameplayStatics::PlaySoundAtLocation(this, JustGuardSE, GetActorLocation(), SE_Volume);

		UE_LOG(LogTemp, Warning, TEXT("Enemy : Guard!!!!!!!!!"));
		//UE_LOG(LogTemp, Warning, TEXT("EnemyAnim : %s"), *UEnum::GetValueAsString(m_AttackAnimState));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//�Ή������G�t�F�N�g���o��
	SpawnEffect(attackReaction);

	return attackReaction;
}

//�G�t�F�N�g���o��
void AEnemy_Base::SpawnEffect(EAttackReaction _attackReaction)
{
	//���A�N�V�����������������疳��
	if (_attackReaction == EAttackReaction::None)
	{
		return;
	}
	//��_����
	else if (_attackReaction == EAttackReaction::Hit)
	{
		UNiagaraComponent* niagaraComp;

		niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageEffect, GetCapsuleComponent()->GetComponentLocation(), GetActorRotation());
		niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor::Blue*50.0f);
	}
	//�K�[�h��
	else if (_attackReaction == EAttackReaction::Guard)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GuardEffect, m_pWeapon->GetActorLocation(), GetActorRotation());
	}
	//�e����
	else if (_attackReaction == EAttackReaction::Parry)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryEffect, m_pWeapon->GetActorLocation(), GetActorRotation());
	}
}

//�U�����s�֐�(�e�ł��̂̓^�C�}�[���Z�b�g�̂�)
void AEnemy_Base::Attack() {
	//�U����Ԃ�
	m_MotionState = EEnemyMotionState::Attack;
	//�U���ҋ@���Ԃ����ɖ߂�
	At = Max_At;

	return;
}

//��_���[�W��
void AEnemy_Base::Damaged(int _damagePoint)
{
	//�_���[�W����
	//���t���[�����ۂ��Ŕ�_���ύX
	if (m_IsFlow && m_IsUseFlow) {
		//���䎮:�t���[���͓��̑���Ƀt���[�l����
		Mp -= _damagePoint;
		//���łɃt���[�^�C�}�[��0�ɂ���
		FlowTimer = 0.0f;
		//�t���[���ꂩ�m�F
		if (Mp <= 0) {
			BreakingFlowTrriger();
		}
	}
	else {
		//Hp���_���[�W�����炷
		m_hitPoint -= _damagePoint;
	}
	//�\�m����𖳂ɂ���
	m_NowScenceOfPlayerAttack = EAttackPattarnState::None;

	//�U�����󂯂���U���^�C�}�[�����������遚�㉺�ŕ�������
	At -= UpperAnger;

	UE_LOG(LogTemp, Warning, TEXT("EnemyHp:%d"), m_hitPoint);

}

//��v���U����
void AEnemy_Base::OnCriticalAttacked(int _damage)
{
	UE_LOG(LogTemp, Log, TEXT("Enemy : Critical Attack Damaged"));

	//�����t���[���������狭������
	if (m_IsFlow == true) {
		m_IsFlow = false;
	}
	m_MotionState = EEnemyMotionState::Assacinated;
	PlayAnimMantage(m_pAssacinatedMontage);
}

//�͂����ꂽ���̌Ă΂��֐�
void AEnemy_Base::Parried()
{
	Super::Parried();

	//���������Ȃ��Ȃ�
	GetCharacterMovement()->StopMovementImmediately();
	//�͂����ꂽ��Ԃɂ���
	m_MotionState = EEnemyMotionState::Parried;
	//�ꉞ�U��������~�ی�
	OnFinishAttack();

	//�e���ꎞ�ԏ�����
	m_parriedTime = 0.0f;
	m_bParried = true;
}

//�m�b�N�o�b�N���I��������֐�
void AEnemy_Base::KnockBackEnd() {
	if (auto* Move = GetCharacterMovement()) {
		Move->GroundFriction = m_Def_GroundFriction;
		Move->BrakingFrictionFactor = m_Def_BrakingFrictionFactor;
		Move->BrakingDecelerationWalking = m_Def_BrakingDecelWalking;
		Move->bOrientRotationToMovement = m_Def_bOrientRotationToMovement;
	}
	bKnockBack = false;
	UE_LOG(LogTemp, Warning, TEXT("ENDDD"));
}

void AEnemy_Base::KnockUpEnd() {
	bKnockBack = false;
	m_bIsKnockUpping = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

//�m�b�N�o�b�N����֐�
void AEnemy_Base::KnockBackStart(FVector _powerPos, float _power) {
	//�p���B���Ȃ�I��
	if (m_bParried) {
		m_bParried = false;
	}

	//�ꉞ�U��������~�ی�
	OnFinishAttack();
	//�A�j���[�V�����Đ�
	m_MotionState = EEnemyMotionState::Stag;
	PlayAnimMantage(m_pBigStagMontage);

	FVector Dir = GetActorLocation() - _powerPos;
	Dir.Z = 0.0f;

	auto* Move = GetCharacterMovement();
	if (Move) {
		//���O�̈ړ��x�N�g����S�폜
		Move->StopMovementImmediately();

		//���i�̈ړ������p�ݒ����������ۑ�
		m_Def_GroundFriction = Move->GroundFriction;
		m_Def_BrakingFrictionFactor = Move->BrakingFrictionFactor;
		m_Def_BrakingDecelWalking = Move->BrakingDecelerationWalking;
		m_Def_bOrientRotationToMovement = Move->bOrientRotationToMovement;

		//������疀�C������U����
		Move->GroundFriction = 0;
		Move->BrakingFrictionFactor = 0;
		Move->BrakingDecelerationWalking = 0;
		Move->bOrientRotationToMovement = false;
	}

	FVector Push = Dir * _power;
	LaunchCharacter(Push, true, true);

	bKnockBack = true;
}

void AEnemy_Base::BreakingFlowTrriger() {
	if (m_IsUseFlow == false) {
		return;
	}
	//�t���[���ꂽ�琁�����
	m_IsFlow = false;
	if (CallOfHitStopCallBack.IsBound()) {
		CallOfHitStopCallBack.Execute();
		UE_LOG(LogTemp, Warning, TEXT("HitStopCallBackCalled!"));
	}

	//SE�Đ�
	UGameplayStatics::PlaySoundAtLocation(this, FlowBreakSE, GetActorLocation(), SE_Volume);
}


void AEnemy_Base::UpdateFlow(float _deltaTime) {
	//�t���[���g��Ȃ��Ȃ痬��
	if (m_IsUseFlow == false) {
		return;
	}
	//�t���[���Ȃ�
	if (m_IsFlow) {
		//�t���[�����Ă������U�m�F
		if (Mp < Max_Mp) {
			if (FlowTimer > 2.0f) {
				Mp += 1;
				UE_LOG(LogTemp, Warning, TEXT("MpRegainNow:%d"), Mp);
			}
			else {
				FlowTimer += _deltaTime;
			}
		}
	}
	//�u���C�N���Ȃ�
	else {
		//�u���C�N�I�����m�F
		FlowTimer += _deltaTime;
		if (FlowTimer >= m_FlowBreakedTime) {
			m_IsFlow = true;
			FlowTimer = 0.0f;
		}
	}
}

//�K�[�h���J�n����֐�
void AEnemy_Base::GuardStart() {
	m_MotionState = EEnemyMotionState::UpGuard;
}

//�K�����J�n
void AEnemy_Base::LockOnStart() {
	m_LookPlayer = true;
}
