// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonPostProcessVolume.h"

//コンストラクタ
ANeonPostProcessVolume::ANeonPostProcessVolume() {
	
}

//初期
void ANeonPostProcessVolume::BeginPlay() {
	Super::BeginPlay();
	//有効範囲：レベル全域
	bUnbound = true;

	//bloomを有効化、初期化///////////////////////////////////
	//にじみ強度
	Settings.bOverride_BloomIntensity = true;
	Settings.BloomIntensity = 2.0f;
	//対象最低輝度
	Settings.bOverride_BloomThreshold = true;
	Settings.BloomThreshold = 1.0f;
	//にじみの品質
	Settings.bOverride_BloomMethod = true;
	Settings.BloomMethod = EBloomMethod::BM_FFT;
	/////////////////////////////////////////////////////////////
}