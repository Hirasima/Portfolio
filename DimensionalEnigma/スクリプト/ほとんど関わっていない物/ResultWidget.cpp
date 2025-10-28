// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "StageSubSystem.h"
#include "ItemManager.h"
#include "Components/Widget.h"

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UStageSubSystem* stage = GetWorld()->GetSubsystem<UStageSubSystem>();

	if (stage)
	{
		m_rankTexture = LoadObject<UTexture2D>(nullptr, *(stage->GetRankTexturePath()));
		m_rankBrush.SetResourceObject(m_rankTexture);
		m_clearTime = stage->GetClearTime();
		m_playerStep = stage->GetPlayerMoveCount();
		m_cameraMove = stage->GetCameraMoveCount();
	}

	UItemManager* itemMng = GetWorld()->GetSubsystem<UItemManager>();

	if (itemMng)
	{
		m_itemTexturePathArray = itemMng->GetItemTexturePath();
		m_itemString = itemMng->GetItemString();
	}

	for (int i = 0; i < m_itemTexturePathArray.Num(); ++i)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *m_itemTexturePathArray[i]);
		FSlateBrush brush;
		brush.SetResourceObject(texture);
		m_itemBrush.Add(brush);
	}
}

void UResultWidget::NativeDestruct()
{
	m_rankBrush.SetResourceObject(nullptr);

	for(int i = 0; i < m_itemBrush.Num(); ++i)
	{
		m_itemBrush[i].SetResourceObject(nullptr);
	}
	

	Super::NativeDestruct();
}

void UResultWidget::SetRank(UTexture2D* _texture)
{
	m_rankTexture = _texture;
}

void UResultWidget::SetClearTime(float _clearTime)
{
	m_clearTime = _clearTime;
}

void UResultWidget::SetPlayerStep(int _playerStep)
{
	m_playerStep = _playerStep;
}

void UResultWidget::SetCameraMove(int _cameraMove)
{
	m_cameraMove = _cameraMove;
}

void UResultWidget::SetItemTexture(UTexture2D* _newTexture)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(_newTexture); 
}