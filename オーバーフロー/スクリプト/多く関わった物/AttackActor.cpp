// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackActor.h"
#include "FighterCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAttackActor::AAttackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAttackActor::BeginPlay()
{
	Super::BeginPlay();
	
	m_attackTimer = 0.0f;

	m_attackCount = 0;
}

// Called every frame
void AAttackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_attackTimer += DeltaTime;

	if (m_attackTimer >= AttackPeriod)
	{
		++m_attackCount;

		m_attackTimer = 0.0f;

		Damage();
	}

	if (m_attackCount >= AttackNum)
	{
		m_damageTargets.Empty();
		this->Destroy();
	}
}

//ターゲットにダメージを与える
void AAttackActor::Damage()
{
	//ダメージターゲットの数繰り返す
	for (int i = 0; i < m_damageTargets.Num(); ++i)
	{
		//ファイターにキャスト
		AFighterCharacter* fighter = Cast<AFighterCharacter>(m_damageTargets[i]);

		//ファイターだったら
		if (fighter && !fighter->Tags.Contains(FName("Dead")))
		{
			//ダメージを与える
			fighter->Damaged(AttackPoint);

			//エフェクトを出す
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, fighter->GetCapsuleComponent()->GetComponentLocation());

			//UE_LOG(LogTemp, Log, TEXT("Attack Actor : Deal Damage To %s . Attack Point Is : %d"), *fighter->GetName(), AttackPoint);
		}
	}
}

//ダメージ対象を設定
void AAttackActor::SetDamageTarget(TArray<AActor*> _targets)
{
	m_damageTargets = _targets;
}

//ダメージを与える回数を設定
void AAttackActor::SetAttackNum(int _attackNum)
{
	AttackNum = _attackNum;
}

//ダメージ量を設定
void AAttackActor::SetAttackPoint(int _attackPoint)
{
	AttackPoint = _attackPoint;
}