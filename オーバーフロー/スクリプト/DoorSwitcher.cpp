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
	//���Ԃ����Z
	//�h�A�J����ԂȂ�h�A�𓮂���
	if (m_isValiable == false) {
		m_nowDoorTimer += DeltaTime;
		for (int i = m_TargetInformations.Num() - 1; i >= 0; i--) {
			//�ҋ@���ԁ{�J���Ԃ��o�ߎ��Ԃ������Ă���Ȃ珈���𗬂�
			if (m_TargetInformations[i].CloseTime+ m_TargetInformations[i].DelayTime < m_nowDoorTimer) {
				continue;
			}

			

			//�h�A���J����
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
		//�q�@����A�N�e�B�u�Ȃ�
		if (m_childPackage[i].ChildMachine->GetIsValiable()==false) {
			//�Ή��������C��������
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
	//���q�@�̏�Ԃ��܂��ۑ��ł��Ă��Ȃ��B�����Ŏq�@�̏�Ԃ�for���ŒT��
	if (m_IsActive) {
		UE_LOG(LogTemp, Warning, TEXT("DoorCalled"));
		//�����g���Ȃ�����
		m_isValiable = false;
		GetStaticMeshComponent()->SetStaticMesh(m_pUnLockedStaticMesh);

		//�^�[�Q�b�g�X�|�i�[��S�ăX�|�[��������
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

	//����ڐG���Ȃ�
	if (!m_IsUsedLight) {
		//�Ώۂ̓��������点��
		for (int i = m_childPackage.Num() - 1; i >= 0; i--) {
			for (int j = m_childPackage[i].TargetLineLights.Num() - 1; j >= 0; j--) {
				m_childPackage[i].TargetLineLights[j]->BeLight(true);
			}
		}
		//����ڐG�B����ۑ�
		m_IsUsedLight = true;
		GetStaticMeshComponent()->SetStaticMesh(m_pCausionStaticMesh);
		return;
	}
}

