// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"
#include "ItemManager.h"

void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UItemManager* itemMng = GetWorld()->GetSubsystem<UItemManager>();

	if (itemMng)
	{
		m_itemTexturePathArray = itemMng->GetItemTexturePath();
	}

	for (int i = 0; i < m_itemTexturePathArray.Num(); ++i)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *m_itemTexturePathArray[i]);
		FSlateBrush brush;
		brush.SetResourceObject(texture);
		m_itemBrush.Add(brush);
	}
}

void UItemWidget::NativeDestruct()
{
	for (int i = 0; i < m_itemBrush.Num(); ++i)
	{
		m_itemBrush[i].SetResourceObject(nullptr);
	}

	Super::NativeDestruct();
}

void UItemWidget::SetItemTexturePath(int _indexNum, FString _path)
{
	if (m_itemBrush.Num() >= 10)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *_path);
		m_itemBrush[_indexNum].SetResourceObject(texture);
	}
}