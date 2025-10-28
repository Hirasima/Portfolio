// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSpawnTrriger.h"
#include "EnemySpawner.h"

void AInteractionSpawnTrriger::BeginPlay() {
	Super::BeginPlay();
	//�ŏ�����L��
	m_isValiable = true;
}


void AInteractionSpawnTrriger::UseInteraction() {
	//�^�[�Q�b�g�X�|�i�[��S�ăX�|�[��������
	for (int i = m_pTargetSpawners.Num() - 1; i >= 0; i--) {
		AEnemySpawner* Sp = m_pTargetSpawners[i];
		if (!Sp) {
			continue;
		}
		Sp->DoSpawnEnemy();
	}
	//�g�p�s��
	m_isValiable = false;
}