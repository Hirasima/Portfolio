// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "StageSubSystem.h"
#include "ItemManager.h"
#include "Components/Widget.h"

//コンストラクタ
void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//現レベルの取得
	UStageSubSystem* stage = GetWorld()->GetSubsystem<UStageSubSystem>();

	if (stage)
	{
		//初期化
		m_rankTexture = LoadObject<UTexture2D>(nullptr, *(stage->GetRankTexturePath()));
		m_rankBrush.SetResourceObject(m_rankTexture);
		m_clearTime = stage->GetClearTime();
		m_playerStep = stage->GetPlayerMoveCount();
		m_cameraMove = stage->GetCameraMoveCount();
	}

	//アイテムマネージャー取得
	UItemManager* itemMng = GetWorld()->GetSubsystem<UItemManager>();

	if (itemMng)
	{
		m_itemTexturePathArray = itemMng->GetItemTexturePath();
		m_itemString = itemMng->GetItemString();
	}

	//アイテムテクスチャの読み込み
	for (int i = 0; i < m_itemTexturePathArray.Num(); ++i)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *m_itemTexturePathArray[i]);
		FSlateBrush brush;
		brush.SetResourceObject(texture);
		m_itemBrush.Add(brush);
	}
}

//デストラクタ
void UResultWidget::NativeDestruct()
{
	m_rankBrush.SetResourceObject(nullptr);

	for(int i = 0; i < m_itemBrush.Num(); ++i)
	{
		m_itemBrush[i].SetResourceObject(nullptr);
	}
	

	Super::NativeDestruct();
}

//ランクのテクスチャの取得
void UResultWidget::SetRank(UTexture2D* _texture)
{
	m_rankTexture = _texture;
}

//クリアタイムの取得
void UResultWidget::SetClearTime(float _clearTime)
{
	m_clearTime = _clearTime;
}

//プレイヤー歩数の設定
void UResultWidget::SetPlayerStep(int _playerStep)
{
	m_playerStep = _playerStep;
}

//カメラ回転回数の設定
void UResultWidget::SetCameraMove(int _cameraMove)
{
	m_cameraMove = _cameraMove;
}

//アイテムテクスチャの設定
void UResultWidget::SetItemTexture(UTexture2D* _newTexture)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(_newTexture); 
}