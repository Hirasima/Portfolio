// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapHISMManager.generated.h"


//�N���X�̑O���錾
class UHierarchicalInstancedStaticMeshComponent;
class ANeonMaker;

UCLASS()
class ORIGINALACTION_API AMapHISMManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapHISMManager();

	//�G�f�B�^�Ń}�[�J����̔��f���ĂԂ��߂�
	UFUNCTION(CallInEditor, Category = "Neon")
	void ReBuildInstance();


protected:
	virtual void OnConstruction(const FTransform& Transform)override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//�R���N���ɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Concreate")
	UStaticMesh* ConcreateMesh;

	UPROPERTY(EditAnywhere, Category = "Concreate")
	TSubclassOf<AActor> ConcreateClass;

	//�����ɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Metal")
	UStaticMesh* MetalMesh;

	UPROPERTY(EditAnywhere, Category = "Metal")
	TSubclassOf<AActor> MetalClass;

	//�����K�ɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Blocks")
	UStaticMesh* BlocksMesh;

	UPROPERTY(EditAnywhere, Category = "Blocks")
	TSubclassOf<AActor> BlocksClass;

	//�n�ʂɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Ground")
	UStaticMesh* GroundMesh;

	UPROPERTY(EditAnywhere, Category = "Ground")
	TSubclassOf<AActor> GroundMakerClass;

	//�K���X�ɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Glass")
	UStaticMesh* GlassMesh;

	UPROPERTY(EditAnywhere, Category = "Glass")
	TSubclassOf<AActor> GlassMakerClass;

private:
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* ConcreateHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* MetalHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* BlocksHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* GroundHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* GlassHISM;
	//�}�[�J�[���擾����HISM�ɔ��f����֐�
	void BuildInstance();

};
