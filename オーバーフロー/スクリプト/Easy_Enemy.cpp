// Fill out your copyright notice in the Description page of Project Settings.


#include "Easy_Enemy.h"
#include "ChuteRealManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


void AEasy_Enemy::EnemyTick(float _DeltaTime, FVector _PlayerPos) {
	if (m_Situation == EEnemySituationState::Dead&&IsSendedDead==false) {
		//チュートリアルマネージャを探す
		AChuteRealManager* Mana= Cast<AChuteRealManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ChuterealManagerClass));
		if (!Mana) {
			//UE_LOG(LogTemp, Warning, TEXT("INIIIIIII"));
		}
		else {
			Mana->AddDeadCount();
			IsSendedDead = true;
		}
	}

	Super::EnemyTick(_DeltaTime, _PlayerPos);
}


EAttackState AEasy_Enemy::GetAttackState() {
	return m_NowAttackState;
}

//★こいつもう呼ばれん（AttackAssetに変更した）
UAnimMontage* AEasy_Enemy::GetAttackMontage() {

	UAnimMontage* ret = nullptr;

	switch (m_NowAttackState)
	{
	case EAttackState::Wait:
		break;
	case EAttackState::UpAttack:
		ret = m_pUpAttackMontage;
		break;
	case EAttackState::UnderAttack:
		ret = m_pUnderAttackMontage;
		break;
	default:
		break;
	}

	return ret;
}

EAttackPattarnState AEasy_Enemy::GetAttackPattarnState() {
	EAttackPattarnState ret = EAttackPattarnState::None;

	if (m_NowAttackState == EAttackState::UpAttack) {
		ret = EAttackPattarnState::UpAttack;
	}
	else if (m_NowAttackState == EAttackState::UnderAttack) {
		ret = EAttackPattarnState::DownAttack;
	}

	return ret;
}

//雑魚の状態に対応するAttackAssetを返す
FAttackAsset AEasy_Enemy::GetAttackAsset()
{
	FAttackAsset attackAsset;

	switch (m_NowAttackState)
	{
	case EAttackState::UpAttack:
		if (AttackUps.IsValidIndex(m_combo))
		{
			attackAsset = AttackUps[m_combo];
		}
		else
		{
			if (m_combo == 99&&GuardCounterAttack.AnimMantage) {
				attackAsset = GuardCounterAttack;
			}
			else {
				attackAsset = AttackUps[0];
			}
		}

		break;
	case EAttackState::UnderAttack:
		if (AttackUnders.IsValidIndex(m_combo))
		{
			attackAsset = AttackUnders[m_combo];
		}
		else
		{
			if (m_combo == 99 && GuardCounterAttack.AnimMantage) {
				attackAsset = GuardCounterAttack;
			}
			else {
				attackAsset = AttackUnders[0];
			}
		}

		break;

	default:
		break;
	}

	return attackAsset;
}

//攻撃終了時
void AEasy_Enemy::OnFinishAttack() {
	Super::OnFinishAttack();
	m_NowAttackState = EAttackState::Wait;
}

//攻撃関数
void AEasy_Enemy::Attack() {
	if (m_IsAttackLock) {
		return;
	}

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
	m_NowAttackState = EAttackState::UnderAttack;
	//現在の利用手札番号を加算
	m_nowHand++;

	//クールダウン入り判定
	if (m_nowHand >= m_nowHandIndexies.Num()) {
		At = Max_At;
		m_nowHandIndexies.Empty();
	}
	PlayAnimMantage(GetAttackAsset().AnimMantage);
}

