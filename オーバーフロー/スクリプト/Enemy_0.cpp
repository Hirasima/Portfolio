// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_0.h"
#include "Weapon.h"
#include "PlayPawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "EnemyHpBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include"ChuteRealManager.h"
#include "BossHpBarWidget.h"

AEnemy_0::AEnemy_0() {
	
}

void AEnemy_0::BeginPlay()
{
	Super::BeginPlay();

	m_CanKnockUp = false;

	//致命不能にする
	m_bEnableCritical = false;

	m_DefaultMaxMp = Max_Mp;

	//ボスのゲージ類Widgetを合成、描画//////////////////////////////////////////////////////////
	if (!BossGageWidgetClass) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:PlayerGageWidgetClass In BeginPlay_WireHUD"));
		return;
	}
	//合成
	m_BossGageWidget = CreateWidget<UBossHpBarWidget>(GetWorld(), BossGageWidgetClass);
	if (!m_BossGageWidget) {
		UE_LOG(LogTemp, Warning, TEXT("Create Faild:BossGageWidget In BeginPlay_Enemy-0"));
		return;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void AEnemy_0::EnemyTick(float _DeltaTime, FVector _PlayerPos) {
	if (m_Situation == EEnemySituationState::Dead && IsSendedDead == false) {
		//チュートリアルマネージャを探す
		AChuteRealManager* Mana = Cast<AChuteRealManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ChuterealManagerClass));
		if (!Mana) {
		}
		else {
			Mana->AddDeadCount();
			IsSendedDead = true;
		}
	}

	//フローの増減を伝達
	m_BossGageWidget->SetFlowPercent((float)Mp / (float)m_DefaultMaxMp);
	if (IsViewGage == false&&m_Situation==EEnemySituationState::Battle) {
		if (PlayerFindCheck()&&m_BossGageWidget) {
			//描画☆後で戦闘開始時に移動
			m_BossGageWidget->AddToViewport();
			IsViewGage = true;
		}
	}

	if (m_hitPoint <= 0) {
		m_BossGageWidget->RemoveFromParent();
	}
	Super::EnemyTick(_DeltaTime, _PlayerPos);
}

//エネミーのアニメーションステート更新
void AEnemy_0::EnemyBattleAnimTick() {
	Super::EnemyBattleAnimTick();
}

//☆彡下間変更関数
//攻撃時関数☆時間が来ていたらの条件はTick側にしておいて
void AEnemy_0::Attack() {
	//ステートをAttackに変更
	Super::Attack();
	//☆きもい
	At = 0;
	
	//手札が空なら補充
	if (m_nowHandIndexies.Num() <= 0) {
		for (int i = 0;i < m_MaxCosts;i++) {
			m_nowHandIndexies.Add(FMath::RandRange(0, AttackUnders.Num() - 1));
		}
		m_nowHand = 0;
	}

	m_combo = m_nowHandIndexies[m_nowHand];
	m_AttackAnimState = EAttackAnimState::UnderAttack;
	//現在の利用手札番号を加算
	m_nowHand++;

	//クールダウン入り判定
	if (m_nowHand >= m_nowHandIndexies.Num()) {
		At = Max_At;
		m_nowHandIndexies.Empty();
	}

	PlayAnimMantage(GetAttackAsset().AnimMantage);
}

//アニメーションステート取得関数
UFUNCTION(BlueprintCallable)
EAttackAnimState AEnemy_0::GetAttackAnimState() {
	return m_AttackAnimState;
}

//攻撃時のアニメーションモンタージュを取得する関数
UFUNCTION(BlueprintCallable)

//☆彡下間変更関数
UAnimMontage* AEnemy_0::GetAttackMontage() {
	UAnimMontage* monta = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Enemy AttackState : %s"), *UEnum::GetValueAsString(m_AttackAnimState));
	//攻撃によって変える
	switch (m_AttackAnimState)
	{
	case EAttackAnimState::Wait:
		break;
	case EAttackAnimState::Dash:
		break;
	case EAttackAnimState::BackStep:
		break;
	case EAttackAnimState::UpAttack:
		monta = m_pUpAttackMontage;
		break;
	case EAttackAnimState::UnderAttack:
		monta = m_pUnderAttackMontage;
		break;
	case EAttackAnimState::CombAttack:
		monta = m_pCombAttack;
		break;
	case EAttackAnimState::UpGuard:
		break;
	case EAttackAnimState::UnderGuard:
		break;
	case EAttackAnimState::Parry:
		break;
	case EAttackAnimState::Parried:
		break;
	case EAttackAnimState::Damage:
		break;
	case EAttackAnimState::Dead:
		break;
	default:
		break;
	}

	return monta;
}

//雑魚の状態に対応するAttackAssetを返す
FAttackAsset AEnemy_0::GetAttackAsset()
{
	FAttackAsset attackAsset;

	switch (m_AttackAnimState)
	{
	case EAttackAnimState::UpAttack:
		if (AttackUps.IsValidIndex(m_combo))
		{
			attackAsset = AttackUps[m_combo];
		}
		else
		{
			attackAsset = AttackUps[0];
		}

		break;
	case EAttackAnimState::UnderAttack:
		if (AttackUnders.IsValidIndex(m_combo))
		{
			attackAsset = AttackUnders[m_combo];
		}
		else
		{
			attackAsset = AttackUnders[0];
		}

		break;

	default:
		break;
	}

	return attackAsset;
}


//攻撃終了時
void AEnemy_0::OnFinishAttack()
{
	Super::OnFinishAttack();
	//攻撃状態を待機に
	m_AttackAnimState = EAttackAnimState::Wait;
}


//攻撃の種類の取得
EAttackPattarnState AEnemy_0::GetAttackPattarnState()
{
	//戻り値用
	EAttackPattarnState state = EAttackPattarnState::None;

	//現在の状態が上段攻撃だったら
	if (m_AttackAnimState == EAttackAnimState::UpAttack)
	{
		//戻り値を上段攻撃に
		state = EAttackPattarnState::UpAttack;
	}
	//現在の状態が下段攻撃だったら
	else if (m_AttackAnimState == EAttackAnimState::UnderAttack)
	{
		//戻り値を下段攻撃に
		state = EAttackPattarnState::DownAttack;
	}

	//戻り値を返す
	return state;
}

EAttackReaction AEnemy_0::OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp) {
	EAttackReaction returner;

	//上段攻撃ならMP上限に被害を受ける
	if (_attackState == EAttackPattarnState::UpAttack&&m_IsFlow) {
		Max_Mp -= _damage * 0.2f;
	}

	//HPバーに伝達
	returner=Super::OnHitAttack(_damage, _breakPower, _attackState, _attacker);
	m_BossGageWidget->SetHpPercent((float)m_hitPoint / (float)m_Max_HitPoint);
	m_BossGageWidget->SetBossFlowPercent((float)Mp / (float)m_DefaultMaxMp,1-(float)(Max_Mp) / (float)m_DefaultMaxMp);

	return returner;
}

