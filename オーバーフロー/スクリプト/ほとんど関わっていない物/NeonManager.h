// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeonManager.generated.h"

//�N���X�̑O���錾
class UHierarchicalInstancedStaticMeshComponent;
class ANeonMaker;

UCLASS()
class ORIGINALACTION_API ANeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeonManager();

	//�G�f�B�^�Ń}�[�J����̔��f���ĂԂ��߂�
	UFUNCTION(CallInEditor,Category="Neon")
	void ReBuildInstance();

	//�����u���ŃG�f�B�^����Ă�ł݂���:�_��
	UFUNCTION(CallInEditor, Category = "Neon")
	void BePointNeon();
	//�����u���ŃG�f�B�^����Ă�ł݂���:����
	UFUNCTION(CallInEditor, Category = "Neon")
	void OffPointNeon();

	//�x����ԂɃl�I���̐F��ς���֐�
	void BeAlert();

protected:
	virtual void OnConstruction(const FTransform& Transform)override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//�l�I���ɗ��p����StaticMesh
	UPROPERTY(EditAnywhere,Category="Neon")
	UStaticMesh* NeonSticMesh;

	UPROPERTY(EditAnywhere, Category = "Neon")
	TSubclassOf<AActor> NeonSticMakerClass;

	//�r���ɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Building")
	UStaticMesh* UnLightBillMesh;

	UPROPERTY(EditAnywhere, Category = "Building")
	TSubclassOf<AActor> UnLightBillClass;

	//�r���ɗ��p����StaticMesh2
	UPROPERTY(EditAnywhere, Category = "Building")
	UStaticMesh* BillMesh;

	UPROPERTY(EditAnywhere, Category = "Building")
	TSubclassOf<AActor> BillMakerClass;


	//�Ŕɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Boad")
	UStaticMesh* NeonBoadMesh;

	UPROPERTY(EditAnywhere, Category = "Boad")
	TSubclassOf<AActor> NeonBoadMakerClass;

	//�n�ʂɗ��p����StaticMesh
	UPROPERTY(EditAnywhere, Category = "Ground")
	UStaticMesh* GroundMesh;

	UPROPERTY(EditAnywhere, Category = "Ground")
	TSubclassOf<AActor> GroundMakerClass;
	
private:
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* NeonHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* BuildingHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* UnLightBuildingHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* NeonBoardHISM;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* GroundHISM;
	//�}�[�J�[���擾����HISM�ɔ��f����֐�
	void BuildInstance();
};
