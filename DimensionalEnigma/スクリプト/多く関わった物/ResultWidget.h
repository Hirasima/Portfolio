// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	//ランクのテクスチャ取得
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetRank(UTexture2D* _texture);

	//クリアタイムの取得
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetClearTime(float _clearTime);

	//プレイヤー歩数の取得
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPlayerStep(int _playerStep);

	//カメラ回転回数取得
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetCameraMove(int _cameraMove);

	//アイテムテクスチャの取得
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetItemTexture(UTexture2D* _newTexture);

public:
	//ランクのテクスチャ
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* m_rankTexture;

	//アイテムテクスチャの配列
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> m_itemTexturePathArray;

	//アイテムブラッシュ
	UPROPERTY(BlueprintReadOnly)
	TArray<FSlateBrush> m_itemBrush;

	//ランクブラッシュ
	UPROPERTY(BlueprintReadOnly)
	FSlateBrush m_rankBrush;

	//クリアタイム
	UPROPERTY(BlueprintReadOnly)
	float m_clearTime;

	//プレイヤー歩数
	UPROPERTY(BlueprintReadOnly)
	int m_playerStep;

	//カメラ回転回数
	UPROPERTY(BlueprintReadOnly)
	int m_cameraMove;

	//アイテムの取得状況文字列
	UPROPERTY(BlueprintReadOnly)
	FString m_itemString;
};
