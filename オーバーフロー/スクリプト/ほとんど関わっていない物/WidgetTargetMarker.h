// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetTargetMarker.generated.h"

//�O���錾
class UImage;

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UWidgetTargetMarker : public UUserWidget
{
	GENERATED_BODY()

	//���\�b�h/////////////////////////////////////////////////////////////
public:

	//�R���X�g���N�^
	virtual void NativeConstruct()override;

	//�^�[�Q�b�g�̎c��HP�����ݒ�
	void SetTargetHPRatio(float _ratio);

	//�^�[�Q�b�g�̎c��HP�����擾
	UFUNCTION(BlueprintCallable)
	float GetTargetHPRatio();

	//�^�[�Q�b�g�ύX���ɌĂт����֐�
	//UFUNCTION(BlueprintCallable)
	virtual void OnChangeTarget();

	//�v���p�e�B//////////////////////////////////////////////////////////

private :
	//�^�[�Q�b�g�̎c��HP����
	float m_targetHPRatio;

};
