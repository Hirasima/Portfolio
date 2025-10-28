// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonMaker.h"
#include"Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

void ANeonMaker::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	UStaticMeshComponent* MeshComp = GetStaticMeshComponent();

	UMaterialInterface* BaseMat = MeshComp->GetMaterial(0);

	UMaterialInstanceDynamic* DyMat = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), Color);
}

void ANeonMaker::BeginPlay() {
	Super::BeginPlay();
	Destroy();
}