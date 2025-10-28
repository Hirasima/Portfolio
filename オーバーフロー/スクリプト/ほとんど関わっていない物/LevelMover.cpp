// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMover.h"
#include "Kismet/GameplayStatics.h"
#include "PlayPawn.h"
#include "Components/BoxComponent.h"
#include"Engine/TriggerBox.h"
#include "UObject/ConstructorHelpers.h"

void ALevelMover::BeginPlay() {
	//最初はインタラクション有効
	m_isValiable = true;
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	//コリジョンに入ったら移動したい場合はコリジョンに効果を付与
	if (m_pSpwnTrrigerCollision && m_IsUseCollision) {
		m_pSpwnTrrigerCollision->OnActorBeginOverlap.AddDynamic(this, &ALevelMover::HitMoveLevel);
	}
}

void ALevelMover::UseInteraction() {
	if (m_pTargetLevel.IsNull()) {
		return;
	}

	FName LevelName = FPackageName::GetShortFName(m_pTargetLevel.GetAssetName());

	UGameplayStatics::OpenLevel(this, LevelName);

}

void ALevelMover::HitMoveLevel(AActor* _a,AActor* _b) {
	if (!_b->IsA(APlayPawn::StaticClass())) {
		return;
	}
	if (m_pTargetLevel.IsNull()) {
		return;
	}

	FName LevelName = FPackageName::GetShortFName(m_pTargetLevel.GetAssetName());

	UGameplayStatics::OpenLevel(this, LevelName);
}

