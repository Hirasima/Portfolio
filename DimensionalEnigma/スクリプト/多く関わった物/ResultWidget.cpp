// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "StageSubSystem.h"
#include "ItemManager.h"
#include "Components/Widget.h"

//�R���X�g���N�^
void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//�����x���̎擾
	UStageSubSystem* stage = GetWorld()->GetSubsystem<UStageSubSystem>();

	if (stage)
	{
		//������
		m_rankTexture = LoadObject<UTexture2D>(nullptr, *(stage->GetRankTexturePath()));
		m_rankBrush.SetResourceObject(m_rankTexture);
		m_clearTime = stage->GetClearTime();
		m_playerStep = stage->GetPlayerMoveCount();
		m_cameraMove = stage->GetCameraMoveCount();
	}

	//�A�C�e���}�l�[�W���[�擾
	UItemManager* itemMng = GetWorld()->GetSubsystem<UItemManager>();

	if (itemMng)
	{
		m_itemTexturePathArray = itemMng->GetItemTexturePath();
		m_itemString = itemMng->GetItemString();
	}

	//�A�C�e���e�N�X�`���̓ǂݍ���
	for (int i = 0; i < m_itemTexturePathArray.Num(); ++i)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *m_itemTexturePathArray[i]);
		FSlateBrush brush;
		brush.SetResourceObject(texture);
		m_itemBrush.Add(brush);
	}
}

//�f�X�g���N�^
void UResultWidget::NativeDestruct()
{
	m_rankBrush.SetResourceObject(nullptr);

	for(int i = 0; i < m_itemBrush.Num(); ++i)
	{
		m_itemBrush[i].SetResourceObject(nullptr);
	}
	

	Super::NativeDestruct();
}

//�����N�̃e�N�X�`���̎擾
void UResultWidget::SetRank(UTexture2D* _texture)
{
	m_rankTexture = _texture;
}

//�N���A�^�C���̎擾
void UResultWidget::SetClearTime(float _clearTime)
{
	m_clearTime = _clearTime;
}

//�v���C���[�����̐ݒ�
void UResultWidget::SetPlayerStep(int _playerStep)
{
	m_playerStep = _playerStep;
}

//�J������]�񐔂̐ݒ�
void UResultWidget::SetCameraMove(int _cameraMove)
{
	m_cameraMove = _cameraMove;
}

//�A�C�e���e�N�X�`���̐ݒ�
void UResultWidget::SetItemTexture(UTexture2D* _newTexture)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(_newTexture); 
}