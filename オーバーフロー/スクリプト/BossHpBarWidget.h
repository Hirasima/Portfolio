// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyHpBarWidget.h"
#include "BossHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UBossHpBarWidget : public UEnemyHpBarWidget
{
	GENERATED_BODY()
	
public:
	//コンストラクタ
	virtual void NativeConstruct()override;

	//Flowゲージの割合を受け取る関数(上限の伝達のためオーバーライド)
	void SetBossFlowPercent(float _percent,float _flowLimitGein);

	//HPゲージが表か否かの切り替え関数
	void SetBeFrontVitalGage(bool _flag);

protected:
	//上限減少のやつ
	UPROPERTY(meta = (BindWidget))
	UImage* FlowLimitGage;

	UPROPERTY()
	UMaterialInstanceDynamic* FlowLimitMat;
};
