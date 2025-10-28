// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "FighterCharacter.h"
#include "Wire_HUD.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	////�e�A�N�^�[�擾
	//AActor* actor = GetOwner();

	//if (!actor)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ItemComponent : Owner Is None"));
	//	return;
	//}

	//m_pParentFighter = Cast<AFighterCharacter>(actor);

	HUD = Cast<AWire_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	//�A�C�e����������
	m_hpRecoveryItemNum = HPRecoveryItemNumMax;
	
	//���L���X�g�^�C�}�[������
	m_hpRecoveryItemRecastTimer = 0.0f;

	//HUD�ɓ`�B
	CommunicateToHUD();
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �A�C�e�����L���X�g���� ////////////////////////////////
	//�A�C�e�������ő�łȂ����
	if (m_hpRecoveryItemNum < HPRecoveryItemNumMax)
	{
		m_hpRecoveryItemRecastTimer += DeltaTime;
	}

	//�������L���X�g�^�C�����^�C�}�[�������Ă����
	if (m_hpRecoveryItemRecastTimer >= HpRecoveryItemRecastTime)
	{
		//���L���X�g�^�C�������炷
		m_hpRecoveryItemRecastTimer -= HpRecoveryItemRecastTime;

		//�A�C�e�����ǉ�
		++m_hpRecoveryItemNum;

		UE_LOG(LogTemp, Log, TEXT("ItemComponet : Item Add"));
	}

	//HUD�ɓ`�B
	CommunicateToHUD();
}

//�A�C�e�����g�p����ۂɌĂ΂��֐�
void UItemComponent::UseItem(EItemKinds _itemKinds)
{
	switch (_itemKinds)
	{
	case EItemKinds::HpRecovery:

		RecoveryHp();

		break;

	default:

		break;
	}
}

//HP�񕜏���
void UItemComponent::RecoveryHp()
{
	//�Ώۂ̃k���`�F�b�N
	if (!m_pParentFighter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemComponet : Fighter Is None"));
		return;
	}

	//�񕜃A�C�e�������Ȃ�������
	if (m_hpRecoveryItemNum <= 0)
	{
		return;
	}

	//HP����
	int hp = m_pParentFighter->m_hitPoint;

	//HP������ɒB���Ȃ��悤�ɐ���
	m_pParentFighter->m_hitPoint = FMath::Clamp(hp + HPRecoveryAmount, 0, m_pParentFighter->m_Max_HitPoint);

	//�A�C�e��������
	--m_hpRecoveryItemNum;

	UE_LOG(LogTemp, Log, TEXT("ItemComponent : Recovery %d"), m_pParentFighter->m_hitPoint - hp);
}

//HUD�ɓ`�B����
void UItemComponent::CommunicateToHUD()
{
	if (!HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemComponent : HUD is None"));
		return;
	}

	HUD->SetRecoveryItemState(m_hpRecoveryItemNum, m_hpRecoveryItemRecastTimer / HpRecoveryItemRecastTime);

	//UE_LOG(LogTemp, Log, TEXT("ItemComponent : HUD"));
}