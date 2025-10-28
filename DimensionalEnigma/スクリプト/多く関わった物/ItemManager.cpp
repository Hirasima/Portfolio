// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "BluePrint/UserWidget.h"
#include "ItemWidget.h"

#define ITEM_BUFFER 10


void UItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
	m_itemStatusArray.Init(none, ITEM_BUFFER);

	m_maxItemCount = 0;
	m_getItemCount = 0;

	UE_LOG(LogTemp, Warning, TEXT("Generated ItemManager"))
}

void UItemManager::Deinitialize()
{

}

//アイテムの登録
void UItemManager::RegisterItem(AItemBase* _item)
{
	int id = _item->GetItemID();

	if (id > 9) return;

	m_itemStatusArray[id] = idle;

	if (m_itemWidget)
	{
		Cast<UItemWidget>(m_itemWidget)->SetItemTexturePath(id, "/Game/Texture/UI_Item_off.UI_Item_off");
	}
	++m_maxItemCount;
}

//アイテム取得イベント関数
void UItemManager::OnGetItem(AItemBase* _item)
{
	int id = _item->GetItemID();

	if (id > 9) return;
	m_itemStatusArray[id] = get;

	if (m_itemWidget)
	{
		Cast<UItemWidget>(m_itemWidget)->SetItemTexturePath(id, "/Game/Texture/UI_Item_on.UI_Item_on");
	}
	++m_getItemCount;
}

//アイテムウィジェットの生成
void UItemManager::GenerateItemWidget()
{
	FString path = ("/Game/UI/ItemWidget_BP.ItemWidget_BP_C");
	TSubclassOf<UUserWidget> widget = TSoftClassPtr<UUserWidget>(FSoftClassPath(*path)).LoadSynchronous();

	if (widget)
	{
		m_itemWidget = CreateWidget<UUserWidget>(GetWorld(), *widget);
		m_itemWidget->AddToViewport();
		m_itemWidget->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Log, TEXT("Create Item Widget"))
	}
	else UE_LOG(LogTemp, Warning, TEXT("False Create Item Widget"))
}

//アイテムウィジェットの破棄
void UItemManager::RemoveItemWidget()
{
	if (m_itemWidget)
	{
		m_itemWidget->RemoveFromParent();
		m_itemWidget = nullptr;

		UE_LOG(LogTemp, Log, TEXT("Remove Item Widget"))
	}
}

//アイテムテクスチャパスを取得
TArray<FString> UItemManager::GetItemTexturePath()
{
	TArray<FString> texArray;

	texArray.Init("/Game/Texture/UI_Item_Empty.UI_Item_Empty", ITEM_BUFFER);

	for (int i = 0; i < ITEM_BUFFER; ++i)
	{
		if (m_itemStatusArray[i] == idle)
		{
			texArray[i] = "/Game/Texture/UI_Item_off.UI_Item_off";
		}
		else if (m_itemStatusArray[i] == get)
		{
			texArray[i] = "/Game/Texture/UI_Item_on.UI_Item_on";
		}
	}

	return texArray;
}

//アイテム取得状況の取得
FString UItemManager::GetItemString()
{
	FString s("");

	s = FString::FromInt(m_getItemCount) + TEXT(" / ") + FString::FromInt(m_maxItemCount);

	return s;
}