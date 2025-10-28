// Fill out your copyright notice in the Description page of Project Settings.


#include "FogController.h"
#include"Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include"Engine/StaticMeshActor.h"
#include"PlayPawn.h"
#include"Engine/TriggerBox.h"
#include "Components/BoxComponent.h"

// Sets default values
AFogController::AFogController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFogController::BeginPlay()
{
	Super::BeginPlay();

	m_IsUsed = false;
	m_FogTimer = 0.0f;

	if (!m_pTrrigerCollision) {
		return;
	}
	
	m_pTrrigerCollision->OnActorBeginOverlap.AddDynamic(this, &AFogController::ReveilFog);
	m_pTrrigerCollision->OnActorEndOverlap.AddDynamic(this, &AFogController::VeilFog);
}

// Called every frame
void AFogController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsUsed == false) {
		return;
	}

	//濃さをタイマーで決定
	if (m_FogTimer < 3.0f) {
		m_FogTimer += DeltaTime;
		float Alpha = FMath::Clamp(m_FogTimer / 3.0f, 0.0f, 1.0f);
		float Fog = FMath::Lerp(1.0f, 0.0f, Alpha);

		//マテリアルに反映
		UStaticMeshComponent* MeshComp = m_pTargetFog->GetStaticMeshComponent();

		UMaterialInterface* BaseMat = MeshComp->GetMaterial(0);

		UMaterialInstanceDynamic* DyMat = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);

		DyMat->SetScalarParameterValue(FName("Is_WhiteOut"), Fog);

	}
}

void AFogController::ReveilFog(AActor* _a, AActor* _b) {

	if (!_b->IsA(APlayPawn::StaticClass())) {
		return;
	}

	if (m_IsUsed == true) {
		return;
	}
	else {
		m_IsUsed = true;
	}
}

void AFogController::VeilFog(AActor* _a, AActor* _b) {

	if (!_b->IsA(APlayPawn::StaticClass())) {
		return;
	}
	if (m_IsUsed == false) {
		return;
	}
	//マテリアルに反映
	UStaticMeshComponent* MeshComp = m_pTargetFog->GetStaticMeshComponent();

	UMaterialInterface* BaseMat = MeshComp->GetMaterial(0);

	UMaterialInstanceDynamic* DyMat = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);

	DyMat->SetScalarParameterValue(FName("Is_WhiteOut"), 1);
	m_IsUsed = false;

	m_FogTimer = 0.0f;
}

