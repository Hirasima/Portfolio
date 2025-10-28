// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "FighterCharacter.h"
#include "Wire_HUD.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	////親アクター取得
	//AActor* actor = GetOwner();

	//if (!actor)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ItemComponent : Owner Is None"));
	//	return;
	//}

	//m_pParentFighter = Cast<AFighterCharacter>(actor);

	HUD = Cast<AWire_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	//アイテム数初期化
	m_hpRecoveryItemNum = HPRecoveryItemNumMax;
	
	//リキャストタイマー初期化
	m_hpRecoveryItemRecastTimer = 0.0f;

	//HUDに伝達
	CommunicateToHUD();
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// アイテムリキャスト処理 ////////////////////////////////
	//アイテム数が最大でなければ
	if (m_hpRecoveryItemNum < HPRecoveryItemNumMax)
	{
		m_hpRecoveryItemRecastTimer += DeltaTime;
	}

	//もしリキャストタイムをタイマーが超えていれば
	if (m_hpRecoveryItemRecastTimer >= HpRecoveryItemRecastTime)
	{
		//リキャストタイム分減らす
		m_hpRecoveryItemRecastTimer -= HpRecoveryItemRecastTime;

		//アイテム数追加
		++m_hpRecoveryItemNum;

		UE_LOG(LogTemp, Log, TEXT("ItemComponet : Item Add"));
	}

	//HUDに伝達
	CommunicateToHUD();
}

//アイテムを使用する際に呼ばれる関数
void UItemComponent::UseItem(EItemKinds _itemKinds)
{
	switch (_itemKinds)
	{
	case EItemKinds::HpRecovery:

		RecoveryHp();

		break;

	default:

		break;
	}
}

//HP回復処理
void UItemComponent::RecoveryHp()
{
	//対象のヌルチェック
	if (!m_pParentFighter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemComponet : Fighter Is None"));
		return;
	}

	//回復アイテム数がなかったら
	if (m_hpRecoveryItemNum <= 0)
	{
		return;
	}

	//HPを回復
	int hp = m_pParentFighter->m_hitPoint;

	//HPが上限に達しないように制限
	m_pParentFighter->m_hitPoint = FMath::Clamp(hp + HPRecoveryAmount, 0, m_pParentFighter->m_Max_HitPoint);

	//アイテム数消費
	--m_hpRecoveryItemNum;

	UE_LOG(LogTemp, Log, TEXT("ItemComponent : Recovery %d"), m_pParentFighter->m_hitPoint - hp);
}

//HUDに伝達する
void UItemComponent::CommunicateToHUD()
{
	if (!HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemComponent : HUD is None"));
		return;
	}

	HUD->SetRecoveryItemState(m_hpRecoveryItemNum, m_hpRecoveryItemRecastTimer / HpRecoveryItemRecastTime);

	//UE_LOG(LogTemp, Log, TEXT("ItemComponent : HUD"));
}