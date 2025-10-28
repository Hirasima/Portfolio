// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetTargetMarker.h"

void UWidgetTargetMarker::NativeConstruct()
{
	m_targetHPRatio = 1.0f;
}

//�^�[�Q�b�g�̎c��HP�����ݒ�
void UWidgetTargetMarker::SetTargetHPRatio(float _ratio)
{
	m_targetHPRatio = _ratio;
}

//�^�[�Q�b�g�̎c��HP�����擾
float UWidgetTargetMarker::GetTargetHPRatio()
{
	return m_targetHPRatio;
}

//�^�[�Q�b�g�ύX���ɌĂт����֐�
void UWidgetTargetMarker::OnChangeTarget()
{

}