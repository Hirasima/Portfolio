// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorSwitcher.h"
#include "LineLight.h"
#include"NavigationSystem.h"
#include"Components/BrushComponent.h"
#include "NavModifierVolume.h"
#include "EnemySpawner.h"
#include"NavigationSystemTypes.h"

ADoorSwitcher::ADoorSwitcher() {
	PrimaryActorTick.bCanEverTick = true;
}

void ADoorSwitcher::BeginPlay() {
	Super::BeginPlay();
	m_IsActive = false;
	m_IsUsedLight = false;
	m_isValiable = true;
	m_nowDoorTimer = 0.0f;
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetStaticMesh(m_pLockedStaticMesh);
}
void ADoorSwitcher::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//時間を加算
	//ドア開け状態ならドアを動かす
	if (m_isValiable == false) {
		m_nowDoorTimer += DeltaTime;
		for (int i = m_TargetInformations.Num() - 1; i >= 0; i--) {
			//待機時間＋開閉時間より経過時間が立っているなら処理を流す
			if (m_TargetInformations[i].CloseTime+ m_TargetInformations[i].DelayTime < m_nowDoorTimer) {
				continue;
			}

			

			//ドアを開閉する
			float Alpha = FMath::Clamp((m_nowDoorTimer-m_TargetInformations[i].DelayTime) / m_TargetInformations[i].CloseTime, 0.f, 1.f);
			FVector NextPos = FMath::Lerp(m_TargetInformations[i].OpenTargetPos.GetLocation(), m_TargetInformations[i].CloseTargetPos.GetLocation(), Alpha);
			m_TargetInformations[i].TargetDoor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			m_TargetInformations[i].TargetDoor->SetActorLocation(NextPos);
			if (!m_pDoorNav) {
				continue;
			}
			/*if (FMath::IsNearlyEqual(Alpha, 1.0f, 0.1f)) {
				m_pDoorNav->SetActorEnableCollision(false);
				UBrushComponent* Bru = Cast <UBrushComponent>( m_pDoorNav->GetComponentByClass(UBrushComponent::StaticClass()));
				if (Bru) {
					Bru->SetCanEverAffectNavigation(false);
				}
				UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
				if (Nav) {
					Nav->AddDirtyArea(m_pDoorNav->GetComponentsBoundingBox(), ENavigationDirtyFlag::All);
					UE_LOG(LogTemp, Warning, TEXT("OOOPEM"));
				}
			}*/
		}
		return;
	}

	bool Activater = true;
	for (int i = m_childPackage.Num() - 1; i >= 0; i--) {
		//子機が非アクティブなら
		if (m_childPackage[i].ChildMachine->GetIsValiable()==false) {
			//対応したラインを消す
			for (int j = m_childPackage[i].TargetLineLights.Num() - 1; j >= 0; j--) {
				m_childPackage[i].TargetLineLights[j]->BeLight(false);
			}
		}
		else {
			Activater = false;
		}
	}
	if (m_IsActive == false && Activater == true) {
		GetStaticMeshComponent()->SetStaticMesh(m_pLockedStaticMesh);
	}
	m_IsActive = Activater;
}

void ADoorSwitcher::UseInteraction() {
	//★子機の状態をまだ保存できていない。ここで子機の状態をfor文で探索
	if (m_IsActive) {
		UE_LOG(LogTemp, Warning, TEXT("DoorCalled"));
		//もう使えなくする
		m_isValiable = false;
		GetStaticMeshComponent()->SetStaticMesh(m_pUnLockedStaticMesh);

		//ターゲットスポナーを全てスポーンさせる
		if (m_IsUseSpawner) {
			for (int i = m_pTargetSpawners.Num() - 1; i >= 0; i--) {
				AEnemySpawner* Sp = m_pTargetSpawners[i];
				if (!Sp) {
					continue;
				}
				Sp->DoSpawnEnemy();
			}
		}

		return;
	}

	//初回接触時なら
	if (!m_IsUsedLight) {
		//対象の導線を光らせる
		for (int i = m_childPackage.Num() - 1; i >= 0; i--) {
			for (int j = m_childPackage[i].TargetLineLights.Num() - 1; j >= 0; j--) {
				m_childPackage[i].TargetLineLights[j]->BeLight(true);
			}
		}
		//初回接触達成を保存
		m_IsUsedLight = true;
		GetStaticMeshComponent()->SetStaticMesh(m_pCausionStaticMesh);
		return;
	}
}

