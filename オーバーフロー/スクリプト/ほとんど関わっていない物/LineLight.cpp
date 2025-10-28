// Fill out your copyright notice in the Description page of Project Settings.


#include "LineLight.h"
#include"Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

void ALineLight::BeginPlay() {
	Super::BeginPlay();
}

void ALineLight::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);
	BeLight(m_IsfirstActivate);
}

void ALineLight::BeLight(bool _switch) {
	//–¾‚©‚è‚ð‚Â‚¯‚é
	if (_switch) {
		ChangeColor(Color);
	}else{
		ChangeColor(FLinearColor(0, 0, 0));
	}
}

void ALineLight::ChangeColor(FLinearColor _color) {
	UStaticMeshComponent* MeshComp = GetStaticMeshComponent();
	if (!MeshComp) {
		return;
	}

	UMaterialInterface* BaseMat = MeshComp->GetMaterial(0);
	if (!BaseMat) {
		return;
	}

	UMaterialInstanceDynamic* DyMat = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), _color);
}