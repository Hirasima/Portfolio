// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHpBarWidget.generated.h"

//�N���X�̑O���錾
class UImage;
class UMaterialInstanceDynamic;
UCLASS()
class ORIGINALACTION_API UEnemyHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//�R���X�g���N�^
	virtual void NativeConstruct()override;

public:
	//HP�̊������󂯎��֐�
	void SetHpPercent(float _percent);
	//Flow�Q�[�W�̊������󂯎��֐�
	virtual void SetFlowPercent(float _percent);

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* HPBarImage;
	UPROPERTY(meta = (BindWidget))
	UImage* FlowBarImage;

	//���I�}�e���A���C���X�^���X
	UPROPERTY()
	UMaterialInstanceDynamic* BarMat;
	UPROPERTY()
	UMaterialInstanceDynamic* FlowBarMat;

};
