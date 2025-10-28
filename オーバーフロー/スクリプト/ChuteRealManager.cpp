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

	//widgetの初期化
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

	//widgetの表示準備
	m_KeyWidget->SetWidgetClass(m_KeyWidgetClass);
	m_KeyWidget->InitWidget();
	m_KeyWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_KeyWidget->SetVisibility(false);
}

// Called every frame
void AChuteRealManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ストップしていないならタイマー加算
	if (!m_IsTimerStop) {
		ChuterealTimer += DeltaTime;
	}

	SpawnTimer += DeltaTime;

	//時間ごと処理///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (stage == 0&&m_pFirstSpawner) {
		if (SpawnTimer >= 1.0f) {
			m_pFirstSpawner->DoSpawnEnemy();
			stage++;
		}
	}
	//攻撃チュートリアルWidgetを出す
	if (stage == 1) {
		if (ChuterealTimer >= SpawnTimeOf_AttackChuterealWidget) {
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}
	//攻撃チュートリアルwidgetを閉じる
	if (stage == 2) {
		if (ChuterealTimer >= SurviveTimeOf_AttackChuterealWidget) {
			WindowRef->PlayClose();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}
	
	//防御チュートリアルタブを開く
	if (stage == 3) {
		if (ChuterealTimer >= SpawnTimeOf_GuardChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), GuardChuterealWindow);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//防御チュートリアルwidgetを閉じる
	if (stage == 4) {
		if (ChuterealTimer >= SurviveTimeOf_GuardChuterealWidget) {
			WindowRef->PlayClose();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//バフ説明を開く
	if (stage == 5) {
		if (DeadCount!=0) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), BuffChuterealWindows);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//回復チュートリアルタブを開く
	if (stage == 7) {
		if (ChuterealTimer >= SpawnTimeOf_HealChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), HealChuterealWindow);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;

			//マルチスポナー起動
			m_pMultipleSpawner->DoSpawnEnemy();
			IsSpawn = true;
		}
	}

	//回復チュートリアルwidgetを閉じる
	if (stage == 8) {
		if (ChuterealTimer >= SurviveTimeOf_HealChuterealWidget) {
			WindowRef->PlayClose();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//空中コンボ説明を開く
	if (stage == 9) {
		if (ChuterealTimer >= SpawnTimeOf_AirAttackChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), AirAttackChuterealWindows);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
		}
	}

	//パリィ説明を開く
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
			//ボス湧き
			m_pBossSpawner->DoSpawnEnemy();
			IsSpawn = true;
			stage++;
		}
	}

	//フロー説明を開く
	if (stage == 14) {
		if (ChuterealTimer >= SpawnTimeOf_FlowChuterealWidget) {
			WindowRef = CreateWidget<UChuteRealMiniWidget>(GetWorld(), FlowChuterealWindows);
			WindowRef->PlayOpen();
			stage++;
			ChuterealTimer = 0.0f;
			DeadCount = 0.0f;
		}
	}
	//widgetの表示
	if (stage == 16) {
		UE_LOG(LogTemp, Warning, TEXT("INIIIIIII"));
		if (DeadCount > 0) {
			m_KeyWidget->SetVisibility(true);
			stage++;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//敵の無限補充

	if (IsSpawn == false) {
		if (stage > 0 && stage < 5) {
			if (DeadCount >= 1) {
				m_pFirstSpawner->DoSpawnEnemy();
				IsSpawn = true;
			}
		}
		//敵の無限補充
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

