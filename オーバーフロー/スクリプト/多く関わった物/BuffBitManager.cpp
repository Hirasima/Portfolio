// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffBitManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "BuffBitActor.h"

// Sets default values
ABuffBitManager::ABuffBitManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuffBitManager::BeginPlay()
{
	Super::BeginPlay();

	//ワールドの取得
	UWorld* world = GetWorld();

	for (int i = 0; i < 5; ++i)
	{
		m_pBuffBitActors.Add(world->SpawnActor<ABuffBitActor>(BuffBitActorClass));
	}
}

// Called every frame
void ABuffBitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateBitMove();
}

//初期化
void ABuffBitManager::Init(USkeletalMeshComponent* _parentMesh)
{
	if (!_parentMesh)
	{
		return;
	}

	m_pParentMeshComp = _parentMesh;

	//ワールドの取得
	UWorld* world = GetWorld();

	//ビットの初期化


	//アタッチルール設定
	FAttachmentTransformRules attachRules(EAttachmentRule::SnapToTarget, true);

	//ソケット名
	FName activeSocketNames[5] =
	{
		"BitSocket0",
		"BitSocket1",
		"BitSocket2",
		"BitSocket3",
		"BitSocket4"
	};
	FName idolSoketNames[5] =
	{
		"IdolBitSocket0",
		"IdolBitSocket1",
		"IdolBitSocket2",
		"IdolBitSocket3",
		"IdolBitSocket4"
	};

	for (int i = 0; i < 5; ++i)
	{
		m_pBuffBitActors[i]->SetParentSkeltalMeshComp(_parentMesh);
		m_pBuffBitActors[i]->SetSocketName(idolSoketNames[i], activeSocketNames[i]);

		m_pBuffBitActors[i]->AttachToComponent(_parentMesh, attachRules, idolSoketNames[i]);
		m_pBuffBitActors[i]->SetActorRelativeLocation(FVector::ZeroVector);
	}
}

//バフビットの移動処理
void ABuffBitManager::UpdateBitMove()
{
	for (int i = 0; i < m_pBuffBitActors.Num(); ++i)
	{
		FVector v = m_pBuffBitActors[i]->GetRootComponent()->GetRelativeLocation();

		v /= 1.002;

		m_pBuffBitActors[i]->SetActorRelativeLocation(v);
	}
}

//バフの状態設定
void ABuffBitManager::SetBuffs(TArray<EBuffPattern>& _buffs)
{
	//もしバフ配列の数が足りなかったら余りのビットはバフなし状態にする
	if (_buffs.Num() < m_pBuffBitActors.Num())
	{
		for (int i = _buffs.Num(); i < m_pBuffBitActors.Num(); ++i)
		{
			m_pBuffBitActors[i]->SetBuffType(EBuffPattern::None);
		}
	}

	//バフを設定
	for (int i = 0; i < _buffs.Num(); ++i)
	{
		m_pBuffBitActors[i]->SetBuffType(_buffs[i]);
	}
}

//ビットの点滅状態切り替え
void ABuffBitManager::SetIsPoint(bool _isPoint)
{
	IsPoint = _isPoint;

	//ビットの数繰り返し
	for (int i = 0; i < m_pBuffBitActors.Num(); ++i)
	{
		m_pBuffBitActors[i]->SetBitIsPoint(_isPoint);
	}
}

//点滅状態か取得
bool ABuffBitManager::GetIsPoint()
{
	return IsPoint;
}

//ビットの追撃
void ABuffBitManager::Attack(AActor* _actor)
{
	if (!_actor)
	{
		//UE_LOG(LogTemp, Log, TEXT("BitManager Actor Is Null"));
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("BitManagerAttack"));

	for (int i = 0; i < m_pBuffBitActors.Num(); ++i)
	{
		if (m_pBuffBitActors[i]->m_eBitState == EBuffPattern::AdditionalAttack)
		{
			m_pBuffBitActors[i]->AttackTargetActor(_actor);
		}
	}
}