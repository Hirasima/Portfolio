// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemBase.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class UItemManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	//初期化時に呼び出される関数
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//終了時に呼び出される関数
	virtual void Deinitialize() override;

	//---------------------------------------------------
	//オリジナルメソッド
	//---------------------------------------------------

	//アイテムの登録
	void RegisterItem(AItemBase* _item);

	//アイテムがされたときに呼ばれる
	void OnGetItem(AItemBase* _item);

	//アイテムウィジェットの生成
	void GenerateItemWidget();

	//アイテムウィジェットの破棄
	void RemoveItemWidget();

	//アイテムテクスチャパスを取得
	TArray<FString> GetItemTexturePath();

	//アイテム取得状況の取得
	FString GetItemString();

	//---------------------------------------------------
	//プロパティ
	//---------------------------------------------------

private:
	//アイテム
	TArray<AItemBase*> m_itemArray;

	//アイテム取得済みテクスチャ
	UTexture2D* m_itemTexture_on;

	//アイテム未取得テクスチャ
	UTexture2D* m_itemTexture_off;

	//空アイテム
	UTexture2D* m_itemTexture_empty;

	//アイテムウィジェット
	UUserWidget* m_itemWidget;

	//アイテムの状態
	enum ItemStatus
	{
		none,
		idle,
		get,
	};
	TArray<ItemStatus> m_itemStatusArray;

	//ステージアイテム数
	int m_maxItemCount;

	int m_getItemCount;
};
