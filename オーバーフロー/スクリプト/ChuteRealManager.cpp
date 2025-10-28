// Fill out your copyright notice in the Description page of Project Settings.


#include "ChuteRealManager.h"
#include "Blueprint/UserWidget.h"
#include "EnemySpawner.h"
#include "ChuteRealMiniWidget.h"
#include"Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AChuteRealManager::AChuteRealManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//widget�̏�����
	m_KeyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	m_KeyWidget->SetupAttachment(RootComponent);

	//
	m_KeyWidget->SetWidgetSpace(EWidgetSpace::World);
	m_KeyWidget->SetDrawAtDesiredSize(true);
	m_KeyWidget->SetTwoSided(true);
}

// Called when the game starts or when spawned
void AChuteRealManager::BeginPlay()
{
	Super::BeginPlay();

	if (ChuterealWindows) {
		WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), ChuterealWindows);
	}

	//widget�̕\������
	m_KeyWidget->SetWidgetClass(m_KeyWidgetClass);
	m_KeyWidget->InitWidget();
	m_KeyWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_KeyWidget->SetVisibility(false);
}

// Called every frame
void AChuteRealManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�X�g�b�v���Ă��Ȃ��Ȃ�^�C�}�[���Z
	if (!m_IsTimerStop) {
		ChuterealTimer += DeltaTime;
	}

	SpawnTimer += DeltaTime;

	//���Ԃ��Ə���///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (stage == 0&&m_pFirstSpawner) {
		if (SpawnTimer >= 1.0f) {
			m_pFirstSpawner->DoSpawnEnemy();
			stage++;
		}
	}
	//�U���`���[�g���A��Widget���o��
	if (stage == 1) {
		if (ChuterealTimer >= SpawnTimeOf_AttackChuterealWidget) {
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}
	//�U���`���[�g���A��widget�����
	if (stage == 2) {
		if (ChuterealTimer >= SurviveTimeOf_AttackChuterealWidget) {
			WindowRef->PlayClose();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}
	
	//�h��`���[�g���A���^�u���J��
	if (stage == 3) {
		if (ChuterealTimer >= SpawnTimeOf_GuardChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), GuardChuterealWindow);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//�h��`���[�g���A��widget�����
	if (stage == 4) {
		if (ChuterealTimer >= SurviveTimeOf_GuardChuterealWidget) {
			WindowRef->PlayClose();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//�o�t�������J��
	if (stage == 5) {
		if (DeadCount!=0) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), BuffChuterealWindows);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//�񕜃`���[�g���A���^�u���J��
	if (stage == 7) {
		if (ChuterealTimer >= SpawnTimeOf_HealChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), HealChuterealWindow);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;

			//�}���`�X�|�i�[�N��
			m_pMultipleSpawner->DoSpawnEnemy();
			IsSpawn = true;
		}
	}

	//�񕜃`���[�g���A��widget�����
	if (stage == 8) {
		if (ChuterealTimer >= SurviveTimeOf_HealChuterealWidget) {
			WindowRef->PlayClose();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//�󒆃R���{�������J��
	if (stage == 9) {
		if (ChuterealTimer >= SpawnTimeOf_AirAttackChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), AirAttackChuterealWindows);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//�p���B�������J��
	if (stage == 11) {
		if (ChuterealTimer >= SpawnTimeOf_ParryChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), ParryChuterealWindows);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	if (stage == 13) {
		UE_LOG(LogTemp, Warning, TEXT("DeadCount:%d"),DeadCount);
		if (DeadCount >= 4) {
			//�{�X�N��
			m_pBossSpawner->DoSpawnEnemy();
			IsSpawn = true;
			stage++;
		}
	}

	//�t���[�������J��
	if (stage == 14) {
		if (ChuterealTimer >= SpawnTimeOf_FlowChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), FlowChuterealWindows);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
			DeadCount = 0.0f;
		}
	}
	//widget�̕\��
	if (stage == 16) {
		UE_LOG(LogTemp, Warning, TEXT("INIIIIIII"));
		if (DeadCount > 0) {
			m_KeyWidget->SetVisibility(true);
			stage++;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�G�̖�����[

	if (IsSpawn == false) {
		if (stage > 0 && stage < 5) {
			if (DeadCount >= 1) {
				m_pFirstSpawner->DoSpawnEnemy();
				IsSpawn = true;
			}
		}
		//�G�̖�����[
		if (stage >= 7 && stage < 14) {
			if (DeadCount >= 4) {
				m_pMultipleSpawner->DoSpawnEnemy();
				IsSpawn = true;
			}
		}
	}

	if (IsSpawn == true) {
		DeadCount = 0;
		IsSpawn = false;
	}
}

void AChuteRealManager::AddDeadCount() {
	DeadCount++;
}

void AChuteRealManager::AddStageCount() {
	stage++;
}

