// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffUpActor.h"
#include "TimerManager.h"


//Widget‚Ì‰ÂŽ‹‰»
void ABuffUpActor::BeVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("BUFFFF"));
	if (m_isValiable == false) {
		return;
	}

	APlayPawn* Pl = Cast<APlayPawn>(_otherActor);
	if (!Pl) {
		return;
	}
	for (int i = 0;i < BuffSize;i++) {
		Pl->AddBuff(BuffKind);
	}
	m_isValiable = false;
	UStaticMeshComponent* Com = GetStaticMeshComponent();
	Com->SetStaticMesh(m_pLockedStaticMesh);

	FTimerHandle ReValiableHandle;
	GetWorld()->GetTimerManager().SetTimer(ReValiableHandle, [this]()
		{
			//s“®Œ ‚ð‰ñ•œ
			m_isValiable = true;
			UStaticMeshComponent* Com = GetStaticMeshComponent();
			Com->SetStaticMesh(m_pUseableStaticMesh);
		}, 5.0f, false);
}

