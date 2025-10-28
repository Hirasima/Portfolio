// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayPawn.h"
#include "PlayerGageWidget.generated.h"

//クラスの前方宣言
class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class ORIGINALACTION_API UPlayerGageWidget : public UUserWidget
{
	GENERATED_BODY()

	//コンストラクタ
	virtual void NativeConstruct()override;

public:
	//HPの割合を受け取る関数
	void SetHpPercent(float _percent);

	//平島追加

	//回復アイテムの状態を設定
	void SetRecoveryItemState(int _itemNum, float _recastRatio);

	//バフの状態を設定
	void SetBuffs(TArray<EBuffPattern>& _buffsArray);

	//バフの状態取得
	UFUNCTION(BlueprintCallable)
	TArray<EBuffPattern> GetBuffs();

	//バフの点滅するかを設定
	void SetBuffIsPoint(bool _isPoint);

protected:
	//---------------------------------------
	//バインド変数 (変数名とエディタ上の名前を一致させる)
	//---------------------------------------

	//HPバー
	UPROPERTY(meta = (BindWidget))
	UImage* HPBarImage;

	//ノイズフレームフレーム
	UPROPERTY(meta = (BindWidget))
	UImage* HP_Frame;

	//アイテムの数
	UPROPERTY(meta = (BindWidget))
	UImage* ItemNumber;

	//アイテムのリキャストタイム
	UPROPERTY(meta = (BindWidget))
	UImage* ItemRatio;

	//---------------------------------------
	//動的マテリアルインスタンス
	//---------------------------------------

	//フレームのマテリアルインスタンス
	UPROPERTY()
	UMaterialInstanceDynamic* HP_FrameMat;

	//動的マテリアルインスタンス
	UPROPERTY()
	UMaterialInstanceDynamic* BarMat;

	//アイテム数の動的マテリアル
	UPROPERTY()
	UMaterialInstanceDynamic* ItemNumMat;

	//アイテムのリキャストタイムの動的マテリアル
	UPROPERTY()
	UMaterialInstanceDynamic* ItemRatioMat;

	//バフのUIの画像
	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_0;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_1;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_2;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_3;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_4;
	//まとめて管理する為の配列
	TArray<UImage*> Buff_In;

	//動的マテリアルインスタンス
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> BuffIcons;
	
	//バフ保存用
	UPROPERTY(EditAnywhere)
	TArray<EBuffPattern> Buff;
};
