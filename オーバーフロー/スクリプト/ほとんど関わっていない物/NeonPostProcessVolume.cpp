// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonPostProcessVolume.h"

//�R���X�g���N�^
ANeonPostProcessVolume::ANeonPostProcessVolume() {
	
}

//����
void ANeonPostProcessVolume::BeginPlay() {
	Super::BeginPlay();
	//�L���͈́F���x���S��
	bUnbound = true;

	//bloom��L�����A������///////////////////////////////////
	//�ɂ��݋��x
	Settings.bOverride_BloomIntensity = true;
	Settings.BloomIntensity = 2.0f;
	//�ΏۍŒ�P�x
	Settings.bOverride_BloomThreshold = true;
	Settings.BloomThreshold = 1.0f;
	//�ɂ��݂̕i��
	Settings.bOverride_BloomMethod = true;
	Settings.BloomMethod = EBloomMethod::BM_FFT;
	/////////////////////////////////////////////////////////////
}