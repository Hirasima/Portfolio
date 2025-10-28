// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NeonMaker.h"
#include "Wire_GameState.h"

// Sets default values
ANeonManager::ANeonManager()
{
 	// Tick�͕s�v�Ƒz��
	PrimaryActorTick.bCanEverTick = false;

	//HISM�̍���
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

//�}�[�J�[���擾����HISM�ɔ��f����֐�
void ANeonManager::BuildInstance() {
	if (!NeonSticMesh) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:NeonSticMesh In OnConstruction_NeonManager"));
		return;
	}
	if (!NeonSticMakerClass) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:NeonSticMakerClass In OnConstruction_NeonManager"));
		return;
	}

	//�l�I���_//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	NeonHISM->ClearInstances();
	NeonHISM->SetStaticMesh(NeonSticMesh);
	NeonHISM->NumCustomDataFloats = 3;

	//���x����̑S�}�[�J�[������
	TArray<AActor*> Markers;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), NeonSticMakerClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = NeonHISM->AddInstance(MarkerTransform);
		//�F�`�B
		ANeonMaker* NeonMark = Cast<ANeonMaker>(Marker);
		FLinearColor Color = NeonMark->Color;
		TArray<float> CustomData = { Color.R,Color.G,Color.B };
		NeonHISM->SetCustomData(Index, CustomData, true);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//�r��//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	BuildingHISM->ClearInstances();
	BuildingHISM->SetStaticMesh(BillMesh);

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BillMakerClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = BuildingHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�r��2//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	UnLightBuildingHISM->ClearInstances();
	UnLightBuildingHISM->SetStaticMesh(UnLightBillMesh);

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UnLightBillClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = UnLightBuildingHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�l�I���Ŕ�//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	NeonBoardHISM->ClearInstances();
	NeonBoardHISM->SetStaticMesh(NeonBoadMesh);
	NeonBoardHISM->NumCustomDataFloats = 3;

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), NeonBoadMakerClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = NeonBoardHISM->AddInstance(MarkerTransform);
		//�F�`�B
		ANeonMaker* NeonMark = Cast<ANeonMaker>(Marker);
		FLinearColor Color = NeonMark->Color;
		TArray<float> CustomData = { Color.R,Color.G,Color.B };
		NeonBoardHISM->SetCustomData(Index, CustomData, true);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�n��//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	GroundHISM->ClearInstances();
	GroundHISM->SetStaticMesh(GroundMesh);

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GroundMakerClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = GroundHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//�}�[�J���甽�f���G�f�B�^����ĂԂ��߂�
void ANeonManager::ReBuildInstance() {
	BuildInstance();
}

//�����u���ŃG�f�B�^����Ă�ł݂���:�_��
void ANeonManager::BePointNeon() {
	UMaterialInstanceDynamic* DMat = NeonBoardHISM->CreateDynamicMaterialInstance(0);
	UMaterialInstanceDynamic* DMat2 = NeonHISM->CreateDynamicMaterialInstance(0);
	if (DMat) {
		DMat->SetScalarParameterValue(FName("IsPoint"), 1.0f);
		DMat2->SetScalarParameterValue(FName("IsPoint"), 1.0f);
	}

	//�Q�[���X�e�[�g�ɓ_�����
	AWire_GameState* G_S = Cast<AWire_GameState>(GetWorld()->GetGameState());
	G_S->SetIsLight(true);
}
//�����u���ŃG�f�B�^����Ă�ł݂���:����
void ANeonManager::OffPointNeon() {
	UMaterialInstanceDynamic* DMat = NeonBoardHISM->CreateDynamicMaterialInstance(0);
	UMaterialInstanceDynamic* DMat2 = NeonHISM->CreateDynamicMaterialInstance(0);
	if (DMat) {
		DMat->SetScalarParameterValue(FName("IsPoint"), 0.0f);
		DMat2->SetScalarParameterValue(FName("IsPoint"), 0.0f);
	}

	//�Q�[���X�e�[�g�ɏ������
	AWire_GameState* G_S = Cast<AWire_GameState>(GetWorld()->GetGameState());
	G_S->SetIsLight(false);
}

//�x����ԂɃl�I���̐F��ς���֐�
void ANeonManager::BeAlert() {
	UMaterialInstanceDynamic* DMat = NeonBoardHISM->CreateDynamicMaterialInstance(0);
	UMaterialInstanceDynamic* DMat2 = NeonHISM->CreateDynamicMaterialInstance(0);
	if (DMat) {
		DMat->SetScalarParameterValue(FName("IsCausion"), 1.0f);
		DMat2->SetScalarParameterValue(FName("IsCausion"), 1.0f);
	}
}


