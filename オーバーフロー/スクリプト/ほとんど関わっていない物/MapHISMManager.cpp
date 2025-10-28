// Fill out your copyright notice in the Description page of Project Settings.


#include "MapHISMManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NeonMaker.h"
#include "Wire_GameState.h"

AMapHISMManager::AMapHISMManager()
{
	// Tickは不要と想定
	PrimaryActorTick.bCanEverTick = false;

	//HISMの合成

	ConcreateHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("ConcreateHISM"));
	RootComponent = ConcreateHISM;

	MetalHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("MetalHISM"));
	MetalHISM->SetupAttachment(ConcreateHISM);

	GroundHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GroundHISM"));
	GroundHISM->SetupAttachment(ConcreateHISM);

	BlocksHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("BlocksHISM"));
	BlocksHISM->SetupAttachment(ConcreateHISM);

	GlassHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GlassHISM"));
	GlassHISM->SetupAttachment(ConcreateHISM);
}

void AMapHISMManager::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	BuildInstance();
}

// Called when the game starts or when spawned
void AMapHISMManager::BeginPlay()
{
	Super::BeginPlay();
}

//マーカーを取得してHISMに反映する関数
void AMapHISMManager::BuildInstance() {

	//レベル上の全マーカーを検索
	TArray<AActor*> Markers;

	//コンクリ//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	ConcreateHISM->ClearInstances();
	ConcreateHISM->SetStaticMesh(ConcreateMesh);

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ConcreateClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = ConcreateHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//金属//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	MetalHISM->ClearInstances();
	MetalHISM->SetStaticMesh(MetalMesh);

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MetalClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = MetalHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//レンガ//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	BlocksHISM->ClearInstances();
	BlocksHISM->SetStaticMesh(BlocksMesh);

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BlocksClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = BlocksHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	//地面//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	GroundHISM->ClearInstances();
	GroundHISM->SetStaticMesh(GroundMesh);

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GroundMakerClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = GroundHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ガラス//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	GlassHISM->ClearInstances();
	GlassHISM->SetStaticMesh(GlassMesh);

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GlassMakerClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = GlassHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//マーカから反映をエディタから呼ぶために
void AMapHISMManager::ReBuildInstance() {
	BuildInstance();
}



