// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakingGlass.h"
#include "Components/BoxComponent.h"
#include "PlayPawn.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ABreakingGlass::ABreakingGlass() {
	//インタラクション用コリジョンの作成
	m_pNiagaraTarget = CreateDefaultSubobject<UBoxComponent>(TEXT("NiagaraTargetCollision"));
	m_pNiagaraTarget->SetupAttachment(RootComponent);

	//上記コリジョンの設定///////////////////////////////////////////////////////////////////////

	//物理無し、判定のみ
	m_pNiagaraTarget->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//全チャンネル
	m_pNiagaraTarget->SetCollisionObjectType(ECC_WorldDynamic);
	m_pNiagaraTarget->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_pNiagaraTarget->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	m_pNiagaraTarget->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//オーバーラップイベントあり
	m_pNiagaraTarget->SetGenerateOverlapEvents(false);

	////////////////////////////////////////////////////////////////////////////////////////////////
}

void ABreakingGlass::BeginPlay() {
	Super::BeginPlay();
	//最初はインタラクション有効
	m_isValiable = true;
}

void ABreakingGlass::UseInteraction() {
	if (!m_pBreakingGlassNia) {
		return;
	}

	Super::UseInteraction();

	//☆試験的に壁向こうにワープさせてみる
	//座標を強制変更
	APlayPawn* Pl=Cast<APlayPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayPawn::StaticClass()));
	if (!Pl) { 
		return; 
	}	
	Pl->GetCharacterMovement()->DisableMovement();
	Pl->TeleportTo(m_pNiagaraTarget->GetComponentLocation(), Pl->GetActorRotation(), false, true);
	Pl->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	Pl->ConsumeMovementInputVector();
	Pl->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	//落下用ガラスならプレイヤーを落下用アニメーションに設定
	Pl->SetPlayerAnimState(EPlayerAnimState::Fall_GlassBreak);
	Pl->ReverceCamYaw();
		
	//スロウ
	Pl->BeSlow(1.0f,0.5f);

	FVector SpawnOffset = m_pNiagaraTarget->GetComponentTransform().GetLocation();
	//ガラスの破片を出現させて自壊
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, m_pBreakingGlassNia, SpawnOffset, GetActorRotation());

	GetStaticMeshComponent()->SetVisibility(false);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_isValiable = false;
	this->Destroy();
}

