// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMarker.h"
#include "Enemy_Base.h"

FEnemyData AEnemyMarker::GetEnemyMarkerData() {
	FEnemyData myData;
	myData.EnemyType = m_EnemyType;
	if (!m_EnemyType) {
		UE_LOG(LogTemp, Warning, TEXT("NotTypeData"));
	}
	myData.EnemyTransform = GetActorTransform();
	return myData;
}