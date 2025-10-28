// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "EnemyMarker.h"
#include"Wire_GameMode.h"
#include "Enemy_Base.h"
#include "EnemyManager.h"
#include "Components/BoxComponent.h"
#include"PlayPawn.h"
#include"Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	//カウンター、タイマーのリセット
	m_nowSpawnCount = 0;
	m_nowSpawnCountTime = 0.0f;
	//利用状態のリセット
	m_IsUsed = false;

	if (m_pSpwnTrrigerCollision&&m_IsUseCollision) {
		m_pSpwnTrrigerCollision->OnActorBeginOverlap.AddDynamic(this, &AEnemySpawner::DoSpawnEnemy);
	}

	//対象のマーカーを取得して
	if (SpawnTargets.Num() <= 0) {
		return;
	}
	for (int i = SpawnTargets.Num() - 1; i >= 0; i--) {
		AEnemyMarker* Mk = SpawnTargets[i];
		if (!Mk) {
			continue;
		}
		SpawnEnemyDatas.Add(Mk->GetEnemyMarkerData());
		Mk->Destroy();
	}
	
}

void AEnemySpawner::DoSpawnEnemy(AActor* _a,AActor* _b) {
	if (!_b->IsA(APlayPawn::StaticClass())) {
		return;
	}
	DoSpawnEnemy();
}

void AEnemySpawner::DoSpawnEnemy(int _SpawnCount) {
	if (m_IsUsed == true||m_IsLocked==true) {
		return;
	}
	if (m_IsSpawnOnlyOnce == true) {
		m_IsLocked = true;
	}
	m_IsUsed = true;

	m_nowSpawnCount = _SpawnCount;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsUsed == false) {
		return;
	}
	if (m_nowSpawnCount > SpawnEnemyDatas.Num() - 1) {
		m_IsUsed = false;
		return;
	}

	//タイマーの加算
	m_nowSpawnCountTime += DeltaTime;

	if (m_nowSpawnCountTime < m_SpawnLag) {
		return;
	}

	//スポーン体の情報作成
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Pos = GetActorLocation();
	FRotator Rot = SpawnEnemyDatas[m_nowSpawnCount].EnemyTransform.Rotator();

	//スポーン
	AEnemy_Base* NewEnemy = GetWorld()->SpawnActor<AEnemy_Base>(SpawnEnemyDatas[m_nowSpawnCount].EnemyType, Pos, Rot, Params);

	if (!NewEnemy) {
		UE_LOG(LogTemp, Warning, TEXT("Cant Spawn Enemy"));
		return;
	}
	//マネージャを取得
	AEnemyManager* _Mn = Cast<AWire_GameMode>(GetWorld()->GetAuthGameMode())->GetEnemyManager();
	if (!_Mn) {
		return;
	}
	//マネージャに登録
	_Mn->AddNewEnemy(NewEnemy);
	//初期待機座標を伝達
	NewEnemy->m_IsAnotherFirstPos = true;
	NewEnemy->SetWaitPosition(SpawnEnemyDatas[m_nowSpawnCount].EnemyTransform);

	//スポーンカウントを進める
	++m_nowSpawnCount;
	//タイマーリセット
	m_nowSpawnCountTime = 0.0f;

}

