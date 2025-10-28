// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectroPawl.h"
#include"NeonManager.h"
#include "Components/BoxComponent.h"
#include "EnemyManager.h"
#include "Kismet/GameplayStatics.h"
#include "Wire_GameMode.h"	
#include "TimerManager.h"
#include"Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

//電源がオンかオフか
bool AElectroPawl::m_neonSwich = true;

//コンストラクタ
AElectroPawl::AElectroPawl() {
	
}

void AElectroPawl::BeginPlay() {
	//最初はインタラクション有効
	m_isValiable = true;
}

void AElectroPawl::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	//自身のを反映
	SetMyColor(Color);
}

//インタラクション時呼ばれる関数
void AElectroPawl::UseInteraction() {
	SwitchNeon();
}

//ネオンを警告色で復旧する関数
void AElectroPawl::BePointAlertNeon() {
	if (!m_targetNeonMng) {
		return;
	}
	//警告色にする
	m_targetNeonMng->BeAlert();
	m_targetNeonMng->BePointNeon();
	m_neonSwich = true;

	//自身の色も警戒色にする
	SetMyColor(FLinearColor::Red);

	//エネミーマネージャに警告を伝達する
	AWire_GameMode* _World= Cast<AWire_GameMode>(GetWorld()->GetAuthGameMode());
	_World->GetEnemyManager()->BeAlart(true, GetActorLocation());
}

//自身の色を変える関数
void AElectroPawl::SetMyColor(FLinearColor _color) {
	//メッシュからネオンマテリアルを持ってくる

	UStaticMeshComponent* MeshComp = GetStaticMeshComponent();

	UMaterialInterface* BaseMat = MeshComp->GetMaterial(NeonMatNum);

	UMaterialInstanceDynamic* DyMat = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(NeonMatNum, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), _color);
}

void AElectroPawl::SwitchNeon() {
	if (!m_targetNeonMng) {
		return;
	}
	
	if (m_neonSwich) {
		// 指定時間後に元に戻すタイマーをセット
		GetWorld()->GetTimerManager().SetTimer(
			TimeSlowTimerHandle,
			this,
			&AElectroPawl::BePointAlertNeon,
			m_BlindTime, // ← 実時間で何秒にするか
			false
		);

		//消灯
		m_targetNeonMng->OffPointNeon();
		m_neonSwich = false;
		//自身も消灯
		SetMyColor(FLinearColor::Black);

		//自身を非有効化
		m_isValiable = false;
	}
}



