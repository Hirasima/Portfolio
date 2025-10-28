// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHpBarWidget.h"
#include "Materials/MaterialInstanceDynamic.h" 
#include "Components/Image.h"


void UEnemyHpBarWidget::NativeConstruct() {
	//HPバー
	if (!HPBarImage) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar Image In Constructer_EnemyHPBarWidget "));
		return;
	}
	if (HPBarImage->GetBrush().GetResourceObject() == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar MateralResource In Constructer_EnemyHPBarWidget "));
		return;
	}
	UMaterialInterface* BaseMat = Cast<UMaterialInterface>(HPBarImage->GetBrush().GetResourceObject());
	BarMat = UMaterialInstanceDynamic::Create(BaseMat, this);
	HPBarImage->SetBrushFromMaterial(BarMat);

	//フローゲージ
	if (!FlowBarImage) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar Image In Constructer_EnemyHPBarWidget "));
		return;
	}
	if (FlowBarImage->GetBrush().GetResourceObject() == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar MateralResource In Constructer_EnemyHPBarWidget "));
		return;
	}
	UMaterialInterface* BaseFlowMat = Cast<UMaterialInterface>(FlowBarImage->GetBrush().GetResourceObject());
	FlowBarMat = UMaterialInstanceDynamic::Create(BaseFlowMat, this);
	FlowBarImage->SetBrushFromMaterial(FlowBarMat);
}

//HPの割合を受け取る関数
void UEnemyHpBarWidget::SetHpPercent(float _percent) {
	if (!BarMat) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HPBar Materal In SetHpPercent_EnemyHPBarWidget"));
		return;
	}
	BarMat->SetScalarParameterValue(TEXT("BarValue"), _percent);
}

//Flowゲージの割合を受け取る関数
void UEnemyHpBarWidget::SetFlowPercent(float _percent) {
	if (!FlowBarMat) {
		//UE_LOG(LogTemp, Warning, TEXT("Not Found: FlowBar Materal In SetFlowPercent_EnemyHPBarWidget"));
		return;
	}
	FlowBarMat->SetScalarParameterValue(TEXT("FlowValue"), _percent);
}