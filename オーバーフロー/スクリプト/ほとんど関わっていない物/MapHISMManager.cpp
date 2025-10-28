// Fill out your copyright notice in the Description page of Project Settings.


#include "MapHISMManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NeonMaker.h"
#include "Wire_GameState.h"

AMapHISMManager::AMapHISMManager()
{
	// Tick�͕s�v�Ƒz��
	PrimaryActorTick.bCanEverTick = false;

	//HISM�̍���

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

//�}�[�J�[���擾����HISM�ɔ��f����֐�
void AMapHISMManager::BuildInstance() {

	//���x����̑S�}�[�J�[������
	TArray<AActor*> Markers;

	//�R���N��//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	ConcreateHISM->ClearInstances();
	ConcreateHISM->SetStaticMesh(ConcreateMesh);

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ConcreateClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = ConcreateHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//����//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	MetalHISM->ClearInstances();
	MetalHISM->SetStaticMesh(MetalMesh);

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MetalClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = MetalHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�����K//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	BlocksHISM->ClearInstances();
	BlocksHISM->SetStaticMesh(BlocksMesh);

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BlocksClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = BlocksHISM->AddInstance(MarkerTransform);
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

	//�K���X//////////////////////////////////////////////////////////////////////////////////////////////
	//HISM���N���A
	GlassHISM->ClearInstances();
	GlassHISM->SetStaticMesh(GlassMesh);

	//���x����̑S�}�[�J�[������
	Markers.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GlassMakerClass, Markers);
	//�C���X�^���X�ɍ���
	for (AActor* Marker : Markers) {
		FTransform MarkerTransform = Marker->GetActorTransform();
		int32 Index = GlassHISM->AddInstance(MarkerTransform);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//�}�[�J���甽�f���G�f�B�^����ĂԂ��߂�
void AMapHISMManager::ReBuildInstance() {
	BuildInstance();
}



