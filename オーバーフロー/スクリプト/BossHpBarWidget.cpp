// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHpBarWidget.h"
#include "Components/Image.h"

void UBossHpBarWidget::NativeConstruct() {
	Super::NativeConstruct();

	UMaterialInterface* BaseFlowMat = Cast<UMaterialInterface>(FlowLimitGage->GetBrush().GetResourceObject());
	FlowLimitMat = UMaterialInstanceDynamic::Create(BaseFlowMat, this);
	FlowLimitGage->SetBrushFromMaterial(FlowLimitMat);
}

void UBossHpBarWidget::SetBossFlowPercent(float _percent, float _flowLimitGein) {

	//���ʂ̃t���[�Q�[�W�̓`�B
	Super::SetFlowPercent(_percent);
	//����̓`�B
	if (!FlowBarMat) {
		//UE_LOG(LogTemp, Warning, TEXT("Not Found: FlowBar Materal In SetFlowPercent_EnemyHPBarWidget"));
		return;
	}
	FlowBarMat->SetScalarParameterValue(TEXT("FlowValue"), _percent);
	FlowLimitMat->SetScalarParameterValue(TEXT("BarValue"), _flowLimitGein);

}

//HP�Q�[�W���\���ۂ��̐؂�ւ��֐�
void UBossHpBarWidget::SetBeFrontVitalGage(bool _flag) {

}