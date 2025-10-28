// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor.h"
#include "Components/BoxComponent.h"
#include "PlayPawn.h"
#include "EnemySpawner.h"
#include "Components/WidgetComponent.h"

//�R���X�g���N�^
AInteractionActor::AInteractionActor() {
	//�C���^���N�V�����p�R���W�����̍쐬
	m_pInteractionTriggerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollision"));
	m_pInteractionTriggerCollision->SetupAttachment(RootComponent);

	//��L�R���W�����̐ݒ�///////////////////////////////////////////////////////////////////////

	//���������A����̂�
	m_pInteractionTriggerCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//�S�`�����l��
	m_pInteractionTriggerCollision->SetCollisionObjectType(ECC_WorldDynamic);
	m_pInteractionTriggerCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_pInteractionTriggerCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	m_pInteractionTriggerCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//�I�[�o�[���b�v�C�x���g����
	m_pInteractionTriggerCollision->SetGenerateOverlapEvents(true);




	////////////////////////////////////////////////////////////////////////////////////////////////

	//�L�[widget�̏�����
	m_KeyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	m_KeyWidget->SetupAttachment(RootComponent);

	//
	m_KeyWidget->SetWidgetSpace(EWidgetSpace::World);
	m_KeyWidget->SetDrawAtDesiredSize(true);
	m_KeyWidget->SetTwoSided(true);

	//�ʒu�I�t�Z�b�g���}�W�b�N�i���o�[
	m_KeyWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 103.0f));
	m_KeyWidget->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	///////////////////////////////////////////////////////////////////////////////////////////////////



	m_isValiable = true;
}

void AInteractionActor::BeginPlay() {
	Super::BeginPlay();

	UStaticMeshComponent* Com = GetStaticMeshComponent();
	Com->SetMobility(EComponentMobility::Movable);

	//HP�o�[Widget�̏�����
	if (!m_KeyWidget) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:HpBarComponent In BeginPlay_EnemyBase"));
		return;
	}
	if (!m_KeyWidgetClass) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:HpBarWidgetClass In BeginPlay_EnemyBase"));
		return;
	}
	m_KeyWidget->SetWidgetClass(m_KeyWidgetClass);
	m_KeyWidget->InitWidget();
	m_KeyWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//����xHP�o�[�Ȃ��ɂ���
	m_KeyWidget->SetVisibility(false);

	if (m_pInteractionTriggerCollision) {
		m_pInteractionTriggerCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractionActor::BeVisibleWidget);
		m_pInteractionTriggerCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractionActor::BeUnVisibleWidget);
	}
}

//�C���^���N�V�������Ă΂��֐�
void AInteractionActor::UseInteraction() {
	m_isValiable = false;
	m_KeyWidget->SetVisibility(false);

	//�^�[�Q�b�g�X�|�i�[��S�ăX�|�[��������
	if (m_IsUseSpawner) {
		for (int i = m_pTargetSpawners.Num() - 1; i >= 0; i--) {
			AEnemySpawner* Sp = m_pTargetSpawners[i];
			if (!Sp) {
				continue;
			}
			Sp->DoSpawnEnemy();
		}
	}
}

//�C���^���N�V�����ΏۂƂ��ėL�����ۂ����擾����֐�
bool AInteractionActor::GetIsValiable() {
	return m_isValiable;
}

UAnimMontage* AInteractionActor::GetInteractMontage() {
	return m_pIntaeractMontage;
}

//Widget�̉���
void AInteractionActor::BeVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult) {
	
	if (m_isValiable == false) {
		return;
	}

	APlayPawn* Pl = Cast<APlayPawn>(_otherActor);
	if (!Pl) {
		return;
	}

	if (m_AutoUsing == true) {
		Pl->InputInteractionOrigin();
	}
	m_KeyWidget->SetVisibility(true);
}
void AInteractionActor::BeUnVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex) {

	APlayPawn* Pl = Cast<APlayPawn>(_otherActor);
	if (!Pl) {
		return;
	}
	m_KeyWidget->SetVisibility(false);
}

void AInteractionActor::BeUnVisibleWidget() {
	m_KeyWidget->SetVisibility(false);
}


