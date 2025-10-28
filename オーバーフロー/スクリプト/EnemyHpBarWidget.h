// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHpBarWidget.generated.h"

//クラスの前方宣言
class UImage;
class UMaterialInstanceDynamic;
UCLASS()
class ORIGINALACTION_API UEnemyHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//コンストラクタ
	virtual void NativeConstruct()override;

public:
	//HPの割合を受け取る関数
	void SetHpPercent(float _percent);
	//Flowゲージの割合を受け取る関数
	virtual void SetFlowPercent(float _percent);

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* HPBarImage;
	UPROPERTY(meta = (BindWidget))
	UImage* FlowBarImage;

	//動的マテリアルインスタンス
	UPROPERTY()
	UMaterialInstanceDynamic* BarMat;
	UPROPERTY()
	UMaterialInstanceDynamic* FlowBarMat;

};
