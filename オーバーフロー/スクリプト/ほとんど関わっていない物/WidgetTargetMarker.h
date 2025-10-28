// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetTargetMarker.generated.h"

//前方宣言
class UImage;

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UWidgetTargetMarker : public UUserWidget
{
	GENERATED_BODY()

	//メソッド/////////////////////////////////////////////////////////////
public:

	//コンストラクタ
	virtual void NativeConstruct()override;

	//ターゲットの残りHP割合設定
	void SetTargetHPRatio(float _ratio);

	//ターゲットの残りHP割合取得
	UFUNCTION(BlueprintCallable)
	float GetTargetHPRatio();

	//ターゲット変更時に呼びたい関数
	//UFUNCTION(BlueprintCallable)
	virtual void OnChangeTarget();

	//プロパティ//////////////////////////////////////////////////////////

private :
	//ターゲットの残りHP割合
	float m_targetHPRatio;

};
