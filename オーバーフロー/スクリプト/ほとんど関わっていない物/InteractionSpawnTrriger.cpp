// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSpawnTrriger.h"
#include "EnemySpawner.h"

void AInteractionSpawnTrriger::BeginPlay() {
	Super::BeginPlay();
	//最初から有効
	m_isValiable = true;
}


void AInteractionSpawnTrriger::UseInteraction() {
	//ターゲットスポナーを全てスポーンさせる
	for (int i = m_pTargetSpawners.Num() - 1; i >= 0; i--) {
		AEnemySpawner* Sp = m_pTargetSpawners[i];
		if (!Sp) {
			continue;
		}
		Sp->DoSpawnEnemy();
	}
	//使用不可化
	m_isValiable = false;
}