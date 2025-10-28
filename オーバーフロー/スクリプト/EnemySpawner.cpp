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

	//�J�E���^�[�A�^�C�}�[�̃��Z�b�g
	m_nowSpawnCount = 0;
	m_nowSpawnCountTime = 0.0f;
	//���p��Ԃ̃��Z�b�g
	m_IsUsed = false;

	if (m_pSpwnTrrigerCollision&&m_IsUseCollision) {
		m_pSpwnTrrigerCollision->OnActorBeginOverlap.AddDynamic(this, &AEnemySpawner::DoSpawnEnemy);
	}

	//�Ώۂ̃}�[�J�[���擾����
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

	//�^�C�}�[�̉��Z
	m_nowSpawnCountTime += DeltaTime;

	if (m_nowSpawnCountTime < m_SpawnLag) {
		return;
	}

	//�X�|�[���̂̏��쐬
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Pos = GetActorLocation();
	FRotator Rot = SpawnEnemyDatas[m_nowSpawnCount].EnemyTransform.Rotator();

	//�X�|�[��
	AEnemy_Base* NewEnemy = GetWorld()->SpawnActor<AEnemy_Base>(SpawnEnemyDatas[m_nowSpawnCount].EnemyType, Pos, Rot, Params);

	if (!NewEnemy) {
		UE_LOG(LogTemp, Warning, TEXT("Cant Spawn Enemy"));
		return;
	}
	//�}�l�[�W�����擾
	AEnemyManager* _Mn = Cast<AWire_GameMode>(GetWorld()->GetAuthGameMode())->GetEnemyManager();
	if (!_Mn) {
		return;
	}
	//�}�l�[�W���ɓo�^
	_Mn->AddNewEnemy(NewEnemy);
	//�����ҋ@���W��`�B
	NewEnemy->m_IsAnotherFirstPos = true;
	NewEnemy->SetWaitPosition(SpawnEnemyDatas[m_nowSpawnCount].EnemyTransform);

	//�X�|�[���J�E���g��i�߂�
	++m_nowSpawnCount;
	//�^�C�}�[���Z�b�g
	m_nowSpawnCountTime = 0.0f;

}

