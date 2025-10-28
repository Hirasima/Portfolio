// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectroPawl.h"
#include"NeonManager.h"
#include "Components/BoxComponent.h"
#include "EnemyManager.h"
#include "Kismet/GameplayStatics.h"
#include "Wire_GameMode.h"	
#include "TimerManager.h"
#include"Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

//�d�����I�����I�t��
bool AElectroPawl::m_neonSwich = true;

//�R���X�g���N�^
AElectroPawl::AElectroPawl() {
	
}

void AElectroPawl::BeginPlay() {
	//�ŏ��̓C���^���N�V�����L��
	m_isValiable = true;
}

void AElectroPawl::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	//���g�̂𔽉f
	SetMyColor(Color);
}

//�C���^���N�V�������Ă΂��֐�
void AElectroPawl::UseInteraction() {
	SwitchNeon();
}

//�l�I�����x���F�ŕ�������֐�
void AElectroPawl::BePointAlertNeon() {
	if (!m_targetNeonMng) {
		return;
	}
	//�x���F�ɂ���
	m_targetNeonMng->BeAlert();
	m_targetNeonMng->BePointNeon();
	m_neonSwich = true;

	//���g�̐F���x���F�ɂ���
	SetMyColor(FLinearColor::Red);

	//�G�l�~�[�}�l�[�W���Ɍx����`�B����
	AWire_GameMode* _World= Cast<AWire_GameMode>(GetWorld()->GetAuthGameMode());
	_World->GetEnemyManager()->BeAlart(true, GetActorLocation());
}

//���g�̐F��ς���֐�
void AElectroPawl::SetMyColor(FLinearColor _color) {
	//���b�V������l�I���}�e���A���������Ă���

	UStaticMeshComponent* MeshComp = GetStaticMeshComponent();

	UMaterialInterface* BaseMat = MeshComp->GetMaterial(NeonMatNum);

	UMaterialInstanceDynamic* DyMat = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(NeonMatNum, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), _color);
}

void AElectroPawl::SwitchNeon() {
	if (!m_targetNeonMng) {
		return;
	}
	
	if (m_neonSwich) {
		// �w�莞�Ԍ�Ɍ��ɖ߂��^�C�}�[���Z�b�g
		GetWorld()->GetTimerManager().SetTimer(
			TimeSlowTimerHandle,
			this,
			&AElectroPawl::BePointAlertNeon,
			m_BlindTime, // �� �����Ԃŉ��b�ɂ��邩
			false
		);

		//����
		m_targetNeonMng->OffPointNeon();
		m_neonSwich = false;
		//���g������
		SetMyColor(FLinearColor::Black);

		//���g���L����
		m_isValiable = false;
	}
}



