// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayPawn.h"
#include "Wire_HUD.generated.h"

//クラスの前方宣言
class UPlayerGageWidget;

UCLASS()
class ORIGINALACTION_API AWire_HUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//プレイヤーのゲージ類Widgetのクラス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerGageWidgetClass;

	//プレイヤーのゲージ類Widgetの実体
	UPROPERTY(VisibleInstanceOnly)
	UPlayerGageWidget* m_PlayerGageWidget;

	//プレイヤーのHPを変更する関数
	void SetPlayerHpPercent(float _percent);

	//回復アイテムの状態を設定
	void SetRecoveryItemState(int _itemNum, float _recastRatio);

	//プレイヤーのバフ状況を設定
	void SetPlayerBuff(TArray<EBuffPattern>& _buffsArray);

	//バフの点滅するかを設定
	void SetBuffIsPoint(bool _isPoint);
};
