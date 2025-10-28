// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetTargetMarker.h"

void UWidgetTargetMarker::NativeConstruct()
{
	m_targetHPRatio = 1.0f;
}

//ターゲットの残りHP割合設定
void UWidgetTargetMarker::SetTargetHPRatio(float _ratio)
{
	m_targetHPRatio = _ratio;
}

//ターゲットの残りHP割合取得
float UWidgetTargetMarker::GetTargetHPRatio()
{
	return m_targetHPRatio;
}

//ターゲット変更時に呼びたい関数
void UWidgetTargetMarker::OnChangeTarget()
{

}