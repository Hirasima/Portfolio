// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGageWidget.h"
#include "Materials/MaterialInstanceDynamic.h" 
#include "Components/Image.h"

void UPlayerGageWidget::NativeConstruct() {

	//HPバー初期化--------------------------------------------------------------------------------------
	if (!HPBarImage) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar Image In Constructer_PlayerBarWidget "));
		return;
	}
	if (HPBarImage->GetBrush().GetResourceObject() == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar MateralResource In Constructer_PlayerBarWidget "));
		return;
	}
	//マテリアルインターフェイス取得
	UMaterialInterface* BaseMat = Cast<UMaterialInterface>(HPBarImage->GetBrush().GetResourceObject());
	//動的マテリアル取得
	BarMat = UMaterialInstanceDynamic::Create(BaseMat, this);
	HPBarImage->SetBrushFromMaterial(BarMat);
	BarMat->SetScalarParameterValue(TEXT("State"), 0.0f);
	//HPBarImage->SetVisibility(ESlateVisibility::Hidden);


	//フレームのマテリアル設定-----------------------------------------------------------------------------
	if (HP_Frame->GetBrush().GetResourceObject() == nullptr) {
		return;
	}
	UMaterialInterface* Frame = Cast<UMaterialInterface>(HP_Frame->GetBrush().GetResourceObject());
	HP_FrameMat = UMaterialInstanceDynamic::Create(Frame, this);
	HP_Frame->SetBrushFromMaterial(HP_FrameMat);
	HP_Frame->SetVisibility(ESlateVisibility::Hidden);

	//アイテムの初期化-------------------------------------------------------------------------------------

	if (!ItemNumber || !ItemNumber->GetBrush().GetResourceObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Number UI Is None"));
		return;
	}
	if (!ItemRatio || !ItemRatio->GetBrush().GetResourceObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Ratio UI Is None"));
		return;
	}

	UMaterialInterface* ItemNumInterface = Cast<UMaterialInterface>(ItemNumber->GetBrush().GetResourceObject());
	ItemNumMat = UMaterialInstanceDynamic::Create(ItemNumInterface, this);
	ItemNumber->SetBrushFromMaterial(ItemNumMat);

	UMaterialInterface* ItemRatioInterface = Cast<UMaterialInterface>(ItemRatio->GetBrush().GetResourceObject());
	ItemRatioMat = UMaterialInstanceDynamic::Create(ItemRatioInterface, this);
	ItemRatio->SetBrushFromMaterial(ItemRatioMat);

	//バフの初期化-----------------------------------------------------------------------------------------

	//バフアイコンを一つにまとめる
	Buff_In.Add(Buff_In_0);
	Buff_In.Add(Buff_In_1);
	Buff_In.Add(Buff_In_2);
	Buff_In.Add(Buff_In_3);
	Buff_In.Add(Buff_In_4);

	for (int i = 0; i < Buff_In.Num(); ++i)
	{
		if (!Buff_In[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Found : Buff Image"));
			return;
		}
		UMaterialInterface* Base = Cast<UMaterialInterface>(Buff_In[i]->GetBrush().GetResourceObject());

		if (!Base)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Found : Buff MAterialResource"));
			return;
		}

		BuffIcons.Add(UMaterialInstanceDynamic::Create(Base, this));
		Buff_In[i]->SetBrushFromMaterial(BuffIcons[i]);
		//Buff_In[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

//HPの割合を受け取る関数
void UPlayerGageWidget::SetHpPercent(float _percent) {
	if (!BarMat) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar Materal In SetHpPercent_PlayerBarWidget"));
		return;
	}
	if (_percent > 0.4f)
	{
		HP_Frame->SetVisibility(ESlateVisibility::Hidden);
		BarMat->SetScalarParameterValue(TEXT("State"), 0.0f);
	}
	if (_percent <= 0.4f) {
		HP_Frame->SetVisibility(ESlateVisibility::Visible);
		BarMat->SetScalarParameterValue(TEXT("State"), 1.0f);
	}
	if (_percent <= 0.2f) {
		HP_FrameMat->SetScalarParameterValue(TEXT("HeavyDamage"), 1);
		BarMat->SetScalarParameterValue(TEXT("State"), 2.0f);
	}
}

//平島追加

//回復アイテムの状態を設定
void UPlayerGageWidget::SetRecoveryItemState(int _itemNum, float _recastRatio)
{
	//UE_LOG(LogTemp, Log, TEXT("Widget : Item"));
	ItemNumMat->SetScalarParameterValue(TEXT("ItemNumber"), _itemNum);
	ItemRatioMat->SetScalarParameterValue(TEXT("RecastRatio"), _recastRatio);
}

//バフの状態を受け取る
void UPlayerGageWidget::SetBuffs(TArray<EBuffPattern>& _buffsArray)
{
	UE_LOG(LogTemp, Log, TEXT("Seting Buffs"));

	while (_buffsArray.Num() < 5)
	{
		_buffsArray.Add(EBuffPattern::None);
	}
	
	for (int i = 0; i < _buffsArray.Num(); ++i)
	{
		int buffKind;
		
		switch (_buffsArray[i])
		{
		case EBuffPattern::SpeedUp:
			buffKind = 1;
			break;

		case EBuffPattern::AdditionalAttack:
			buffKind = 2;
			break;

		default:
			buffKind = 0;
			break;
		}

		BuffIcons[i]->SetScalarParameterValue(TEXT("BuffKind"), buffKind);
	}
}

//バフの状態取得
TArray<EBuffPattern> UPlayerGageWidget::GetBuffs()
{
	UE_LOG(LogTemp, Log, TEXT("Geting Buffs"));
	return Buff;
}

//バフの点滅するかを設定
void UPlayerGageWidget::SetBuffIsPoint(bool _isPoint)
{
	for (int i = 0; i < BuffIcons.Num(); ++i)
	{
		BuffIcons[i]->SetScalarParameterValue(TEXT("IsPoint"), _isPoint);
	}
}