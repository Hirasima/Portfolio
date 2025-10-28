// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NeonMaker.h"
#include "Wire_GameState.h"

// Sets default values
ANeonManager::ANeonManager()
{
 	// Tickは不要と想定
	PrimaryActorTick.bCanEverTick = false;

	//HISMの合成
	NeonHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("NeonHISM"));
	RootComponent = NeonHISM;

	BuildingHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("BuildingHISM"));
	BuildingHISM->SetupAttachment(NeonHISM);

	UnLightBuildingHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("UnLightBuildingHISM"));
	UnLightBuildingHISM->SetupAttachment(NeonHISM);

	NeonBoardHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("BoadHISM"));
	NeonBoardHISM->SetupAttachment(NeonHISM);

	GroundHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GroundHISM"));
	GroundHISM->SetupAttachment(NeonHISM);
}

void ANeonManager::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	BuildInstance();
}

// Called when the game starts or when spawned
void ANeonManager::BeginPlay()
{
	Super::BeginPlay();
}

//マーカーを取得してHISMに反映する関数
void ANeonManager::BuildInstance() {
	if (!NeonSticMesh) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:NeonSticMesh In OnConstruction_NeonManager"));
		return;
	}
	if (!NeonSticMakerClass) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:NeonSticMakerClass In OnConstruction_NeonManager"));
		return;
	}

	//ネオン棒//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	NeonHISM->ClearInstances();
	NeonHISM->SetStaticMesh(NeonSticMesh);
	NeonHISM->NumCustomDataFloats = 3;

	//レベル上の全マーカーを検索
	TArray<AActor*> Markers;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), NeonSticMakerClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = NeonHISM->AddInstance(MarkerTransform);
		//色伝達
		ANeonMaker* NeonMark = Cast<ANeonMaker>(Marker);
		FLinearColor Color = NeonMark->Color;
		TArray<float> CustomData = { Color.R,Color.G,Color.B };
		NeonHISM->SetCustomData(Index, CustomData, true);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ビル//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	BuildingHISM->ClearInstances();
	BuildingHISM->SetStaticMesh(BillMesh);

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BillMakerClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = BuildingHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ビル2//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	UnLightBuildingHISM->ClearInstances();
	UnLightBuildingHISM->SetStaticMesh(UnLightBillMesh);

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UnLightBillClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = UnLightBuildingHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ネオン看板//////////////////////////////////////////////////////////////////////////////////////////////
	//HISMをクリア
	NeonBoardHISM->ClearInstances();
	NeonBoardHISM->SetStaticMesh(NeonBoadMesh);
	NeonBoardHISM->NumCustomDataFloats = 3;

	//レベル上の全マーカーを検索
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), NeonBoadMakerClass, Markers);
	//インスタンスに合成
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = NeonBoardHISM->AddInstance(MarkerTransform);
		//色伝達
		ANeonMaker* NeonMark = Cast<ANeonMaker>(Marker);
		FLinearColor Color = NeonMark->Color;
		TArray<float> CustomData = { Color.R,Color.G,Color.B };
		NeonBoardHISM->SetCustomData(Index, CustomData, true);
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
}

//マーカから反映をエディタから呼ぶために
void ANeonManager::ReBuildInstance() {
	BuildInstance();
}

//★仮置きでエディタから呼んでみたい:点灯
void ANeonManager::BePointNeon() {
	UMaterialInstanceDynamic* DMat = NeonBoardHISM->CreateDynamicMaterialInstance(0);
	UMaterialInstanceDynamic* DMat2 = NeonHISM->CreateDynamicMaterialInstance(0);
	if (DMat) {
		DMat->SetScalarParameterValue(FName("IsPoint"), 1.0f);
		DMat2->SetScalarParameterValue(FName("IsPoint"), 1.0f);
	}

	//ゲームステートに点灯を報告
	AWire_GameState* G_S = Cast<AWire_GameState>(GetWorld()->GetGameState());
	G_S->SetIsLight(true);
}
//★仮置きでエディタから呼んでみたい:消灯
void ANeonManager::OffPointNeon() {
	UMaterialInstanceDynamic* DMat = NeonBoardHISM->CreateDynamicMaterialInstance(0);
	UMaterialInstanceDynamic* DMat2 = NeonHISM->CreateDynamicMaterialInstance(0);
	if (DMat) {
		DMat->SetScalarParameterValue(FName("IsPoint"), 0.0f);
		DMat2->SetScalarParameterValue(FName("IsPoint"), 0.0f);
	}

	//ゲームステートに消灯を報告
	AWire_GameState* G_S = Cast<AWire_GameState>(GetWorld()->GetGameState());
	G_S->SetIsLight(false);
}

//警戒状態にネオンの色を変える関数
void ANeonManager::BeAlert() {
	UMaterialInstanceDynamic* DMat = NeonBoardHISM->CreateDynamicMaterialInstance(0);
	UMaterialInstanceDynamic* DMat2 = NeonHISM->CreateDynamicMaterialInstance(0);
	if (DMat) {
		DMat->SetScalarParameterValue(FName("IsCausion"), 1.0f);
		DMat2->SetScalarParameterValue(FName("IsCausion"), 1.0f);
	}
}


