// Fill out your copyright notice in the Description page of Project Settings.


#include "HpExpressionActor.h"
#include"Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AHpExpressionActor::AHpExpressionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//メッシュの初期化
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = m_pStaticMeshComp;
}

// Called when the game starts or when spawned
void AHpExpressionActor::BeginPlay()
{
	Super::BeginPlay();

	//ヘッドセットのネオンマテリアルを取得
	m_pMaterial = m_pStaticMeshComp->GetMaterial(1);

	//上記のマテリアルに色を付与して登録
	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(1, m_pMaterial);
	DyMat->SetVectorParameterValue(FName("Color"), HeightHP_Color);
	
}

// Called every frame
void AHpExpressionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//HPパーセントを受け取って色を変化させる関数
void AHpExpressionActor::ChangeColor_ByHp(float _hpPercent) {
	if (_hpPercent < 0 || _hpPercent>1) {
		return;
	}
	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(1, m_pMaterial);

	if (_hpPercent <= 0.3f) {
		//HP1/3以下で点滅
		DyMat->SetScalarParameterValue(FName("IsPoint"), 1.0f);
	}

	if (_hpPercent >= 0.7f) {
		DyMat->SetVectorParameterValue(FName("Color"), HeightHP_Color);
		return;
	}

	if (_hpPercent >= 0.4f) {
		DyMat->SetVectorParameterValue(FName("Color"), MidHP_Color);
		return;
	}


	DyMat->SetVectorParameterValue(FName("Color"), LowHP_Color);
}

