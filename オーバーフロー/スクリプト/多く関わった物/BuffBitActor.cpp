// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffBitActor.h"
#include "Enemy_Base.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABuffBitActor::ABuffBitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���b�V���̏�����
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = m_pStaticMeshComp;

	//�i�C�A�K���R���|�[�l���g������
	m_pOrbitComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	m_pOrbitComp->SetupAttachment(m_pStaticMeshComp);
	m_pOrbitComp->SetActive(false);


}

// Called when the game starts or when spawned
void ABuffBitActor::BeginPlay()
{
	Super::BeginPlay();
	
	m_pMaterial = m_pStaticMeshComp->GetMaterial(0);

	m_pOrbitComp->SetAsset(m_pBitOrbit);
	m_pOrbitComp->SetActive(false);

	//m_pBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBitBeam, GetActorLocation());
	//m_pBeamComponent->Deactivate();

	m_eBitState = EBuffPattern::None;

	m_bIsAttacking = false;

	//��L�̃}�e���A���ɐF��t�^���ēo�^
	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
	DyMat->SetVectorParameterValue(FName("Color"), NoneBuff_Color);
}

// Called every frame
void ABuffBitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMove(DeltaTime);

	UpdateAttack(DeltaTime);
}

//�o�t��`�B�����֐�
void ABuffBitActor::SetBuffType(EBuffPattern _bufftype) {
	//�X�e�[�g�ݒ�
	m_eBitState = _bufftype;

	//�X�e�[�g���Ƃɕ���
	switch (_bufftype)
	{
	case EBuffPattern::None:
	{
		//���b�V����ύX
		m_pStaticMeshComp->SetStaticMesh(m_pCircleModeMesh);
		//���I�}�e���A�����擾
		UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
		//�F�ύX
		DyMat->SetVectorParameterValue(FName("Color"), NoneBuff_Color);
		//�O���G�t�F�N�g������
		m_pOrbitComp->SetActive(false);

		//�_�Ń��[�h�Ȃ�
		if (IsPoint)
		{
			//�}�e���A����_�łɐݒ�
			DyMat->SetScalarParameterValue(FName("IsPoint"), IsPoint);
		}

		break;
	}
	case EBuffPattern::SpeedUp:
	{
		//���b�V����ύX
		m_pStaticMeshComp->SetStaticMesh(m_pCircleModeMesh);
		//���I�}�e���A�����擾
		UMaterialInstanceDynamic* DyMat1 = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
		//�F�ύX
		DyMat1->SetVectorParameterValue(FName("Color"), SpeedBuff_Color);
		//�O���G�t�F�N�g������
		m_pOrbitComp->SetActive(false);
		
		//�_�Ń��[�h�Ȃ�
		if (IsPoint)
		{
			//�}�e���A����_�łɐݒ�
			DyMat1->SetScalarParameterValue(FName("IsPoint"), IsPoint);
		}

		break;
	}
	case EBuffPattern::AdditionalAttack:
	{
		//���b�V����ύX
		m_pStaticMeshComp->SetStaticMesh(m_pAttackModeMesh);
		//���I�}�e���A�����擾
		UMaterialInterface* MetalMat=m_pAttackModeMesh->GetMaterial(0);
		UMaterialInstanceDynamic* Metal = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MetalMat);
		UMaterialInstanceDynamic* DyMat2 = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(1, m_pMaterial);

		//�F�ύX
		DyMat2->SetVectorParameterValue(FName("Color"), AttackBuff_Color);
		//�f�^�b�`
		RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		//
		FVector v = m_pParentSkeltalMeshComp->GetComponentLocation();
		v.Z += 100;

		m_pBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBitAwake, v);
		m_pBeamComponent->SetVariableVec3(FName("ConvergencePos"), GetActorLocation());

		//�_�Ń��[�h�Ȃ�
		if (IsPoint)
		{
			//�}�e���A����_�łɐݒ�
			DyMat2->SetScalarParameterValue(FName("IsPoint"), IsPoint);
		}

		break;
	}
	default:
		break;
	}
}

//�\�P�b�g���ݒ�
void ABuffBitActor::SetSocketName(FName _idolSocket, FName _activeSocket)
{
	m_idolSocketName = _idolSocket;
	m_activeSocketName = _activeSocket;
}

//�e�X�P���^�����b�V���ݒ�
void ABuffBitActor::SetParentSkeltalMeshComp(USkeletalMeshComponent* _skeltalMeshComp)
{
	m_pParentSkeltalMeshComp = _skeltalMeshComp;
}

//�_�Ő؂�ւ��ݒ�
void ABuffBitActor::SetBitIsPoint(bool _anable)
{
	IsPoint = _anable;

	SetBuffType(m_eBitState);

	/*if (IsPoint && m_eBitState != EBuffPattern::None)
	{
	//���c�F�̕�U���ł��ĂȂ��ċ����������i�ԁj�ɂȂ��Ă���
		UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
		DyMat->SetScalarParameterValue(FName("IsPoint"), _anable);
	}*/
}

//�����̓G�ɍU������֐�
void ABuffBitActor::AttackTargetActor(AActor* _actor)
{
	if (!_actor) {
		return;
	}
	
	//�^�C�}�[������
	attackTimer = 0.0f;

	//�U�����t���O��L����
	m_bIsAttacking = true;
	m_bAttacked = false;

	//�U���Ώېݒ�
	m_pAttckActor = _actor;

	//�U������ʒu��ݒ�//////////////////

	//�U���Ώۂ̈ʒu
	FVector actorPos = _actor->GetActorLocation();
	//�U���ʒu
	FVector attackPos;

	//��]�����߂�
	FRotator attackRot = {
		FMath::RandRange(AttackMinRotation.Pitch, AttackMaxRotation.Pitch),
		FMath::RandRange(AttackMinRotation.Yaw, AttackMaxRotation.Yaw),
		FMath::RandRange(AttackMinRotation.Roll, AttackMaxRotation.Roll)
	};

	//��]�ƃ^�[�Q�b�g�݌ɍ��W����U���ʒu�����߂�
	attackPos.X = AttackRadius * FMath::Cos(FMath::DegreesToRadians(attackRot.Pitch)) * FMath::Cos(FMath::DegreesToRadians(attackRot.Yaw));
	attackPos.Y = AttackRadius * FMath::Cos(FMath::DegreesToRadians(attackRot.Pitch)) * FMath::Sin(FMath::DegreesToRadians(attackRot.Yaw));
	attackPos.Z = AttackRadius * FMath::Sin(FMath::DegreesToRadians(attackRot.Pitch));

	//�^�[�Q�b�g�ʒu�ɐݒ�
	m_attackPos = actorPos + attackPos;
	m_attackRot = (actorPos - m_attackPos).Rotation();
	//UE_LOG(LogTemp, Log, TEXT("Positon : %f, %f , %f"), m_attackRot.Pitch, m_attackRot.Yaw, m_attackRot.Roll);

	//���b�V���̂˂���𒼂���������
	m_attackRot.Roll += -m_attackRot.Pitch;
	m_attackRot.Yaw -= 90.0f;

	//UE_LOG(LogTemp, Log, TEXT("Positon : %f, %f , %f"), m_attackRot.Pitch, m_attackRot.Yaw, m_attackRot.Roll);

	m_pOrbitComp->SetActive(true);
}

//�ړ��X�V
void ABuffBitActor::UpdateMove(float DeltaTime)
{
	//�A�^�b�`�ς݂Ȃ�ړ����Ȃ�
	if (GetAttachParentActor())
	{
		//UE_LOG(LogTemp, Log, TEXT("Attaching"));
		return;
	}

	//�ړ���̍��W
	FVector targetPos;

	//�ړ���̉�]
	FRotator targetRot;

	//�A�N�e�B�u��
	if (m_eBitState == EBuffPattern::AdditionalAttack)
	{
		//�U����
		if (m_bIsAttacking)
		{
			//UE_LOG(LogTemp, Log, TEXT("BitAttack"));
			//�U���n�_�ݒ�
			targetPos = m_attackPos;
			targetRot = m_attackRot;
		}
		//�Ǐ]��
		else
		{
			targetPos = m_pParentSkeltalMeshComp->GetSocketLocation(m_activeSocketName);
			targetRot = m_pParentSkeltalMeshComp->GetSocketRotation(m_activeSocketName);
		}
	}
	//��A�N�e�B�u��
	else
	{
		targetPos = m_pParentSkeltalMeshComp->GetSocketLocation(m_idolSocketName);
		targetRot = m_pParentSkeltalMeshComp->GetSocketRotation(m_idolSocketName);

		//�����������߂�������
		if ((targetPos - GetActorLocation()).Length() <= 50.0f)
		{
			//�\�P�b�g�ɐڑ�
			AttachToComponent(m_pParentSkeltalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, m_idolSocketName);
			return;
		}
	}

	timer += DeltaTime;
	
	//�ړ��ʃx�N�g��
	FVector moveVec;
	FRotator moveRot;

	//���`��Ԃňړ��ʂ��v�Z
	moveRot = FMath::RInterpTo(GetActorRotation(), targetRot, DeltaTime, 45.0f);
	moveVec = FMath::VInterpTo(GetActorLocation(), targetPos, DeltaTime, 10.0f);

	//�㉺�ɗh�炷
	moveVec.Z += 0.7f * FMath::Sin(timer * 2);

	//�ړ�
	SetActorRotation(moveRot);
	SetActorLocation(moveVec);

}

//�U������
void ABuffBitActor::UpdateAttack(float DeltaTime)
{
	//�U����ԂłȂ��Ȃ�I��
	if (!m_bIsAttacking)return;

	//�U�����ԍX�V
	attackTimer += DeltaTime;

	//�U������n�_�܂ł̋������߂���΂��U���ς݂łȂ����
	if ((m_attackPos - GetActorLocation()).Length() <= 20.0f && !m_bAttacked)
	{
		//�U���n�_�Ɉړ�
		SetActorLocation(m_attackPos);

		if (!m_pBeamComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Beam Niagara Comp Is None"));
			return;
		}

		//�r�b�g�r�[���G�t�F�N�g���X�|�[��
		m_pBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBitBeam, GetActorLocation());
		//�r�[���I�_��ݒ�
		if (m_pAttckActor) {
			m_pBeamComponent->SetVariableVec3(FName("BeamEnd"), m_pAttckActor->GetActorLocation());
		}
		//�U���ς�
		m_bAttacked = true;

		//UE_LOG(LogTemp, Log, TEXT("Beeeeeeeeeeeeeeeam"));
	}

	//�U�����Ԃ��I��������U���ς݂Ȃ�
	if (attackTimer >= 0.5f && m_bAttacked)
	{
		//�U�����t���O��������
		m_bIsAttacking = false;
		//�U���ς݃t���O��������
		m_bAttacked = false;
		//�O���G�t�F�N�g������
		m_pOrbitComp->SetActive(false);
	}
}