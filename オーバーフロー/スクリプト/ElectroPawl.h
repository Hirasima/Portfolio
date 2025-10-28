// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "ElectroPawl.generated.h"

//�N���X�̑O���錾
class ANeonManager;
class UBoxComponent;

UCLASS()
class ORIGINALACTION_API AElectroPawl : public AInteractionActor
{
	GENERATED_BODY()
	
public:
	//�R���X�g���N�^
	AElectroPawl();

	//�C���^���N�V�������Ă΂��֐�
	virtual void UseInteraction()override;
	
protected:
	//������
	virtual void BeginPlay()override;

	//�F�ω�����p
	virtual void OnConstruction(const FTransform& Transform)override;

	//�l�I����������������肷��֐�
	void SwitchNeon();
	//�l�I�����x���F�ŕ�������֐�
	void BePointAlertNeon();

	//���g�̐F��ς���֐�
	void SetMyColor(FLinearColor _color);
protected:
	//���g�̊�{�F
	UPROPERTY(EditAnywhere, Category = "Neon")
	int NeonMatNum;

	//���g�̊�{�F
	UPROPERTY(EditAnywhere, Category = "Neon")
	FLinearColor Color = FLinearColor::Red;

	//�^�[�Q�b�g�̃l�I���}�l�[�W����BeginPlay�Ƃ��Ŏ擾���������悳��
	UPROPERTY(EditAnywhere, Category = "Neon")
	float m_BlindTime;

	//�^�[�Q�b�g�̃l�I���}�l�[�W����BeginPlay�Ƃ��Ŏ擾���������悳��
	UPROPERTY(EditAnywhere, Category = "Neon")
	ANeonManager* m_targetNeonMng;

	//����U�����̋��ʕϐ��Ƃ��ēd���̃I���I�t���Ǘ�
	static bool m_neonSwich;

	//�d�����x����Ԃŕ�������p�̃^�C�}�[
	FTimerHandle TimeSlowTimerHandle;
};
