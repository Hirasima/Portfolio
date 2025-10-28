// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyHpBarWidget.h"
#include "BossHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UBossHpBarWidget : public UEnemyHpBarWidget
{
	GENERATED_BODY()
	
public:
	//�R���X�g���N�^
	virtual void NativeConstruct()override;

	//Flow�Q�[�W�̊������󂯎��֐�(����̓`�B�̂��߃I�[�o�[���C�h)
	void SetBossFlowPercent(float _percent,float _flowLimitGein);

	//HP�Q�[�W���\���ۂ��̐؂�ւ��֐�
	void SetBeFrontVitalGage(bool _flag);

protected:
	//��������̂��
	UPROPERTY(meta = (BindWidget))
	UImage* FlowLimitGage;

	UPROPERTY()
	UMaterialInstanceDynamic* FlowLimitMat;
};
