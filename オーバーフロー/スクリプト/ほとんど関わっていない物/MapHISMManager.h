// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapHISMManager.generated.h"


//クラスの前方宣言
class UHierarchicalInstancedStaticMeshComponent;
class ANeonMaker;

UCLASS()
class ORIGINALACTION_API AMapHISMManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapHISMManager();

	//エディタでマーカからの反映を呼ぶために
	UFUNCTION(CallInEditor, Category = "Neon")
	void ReBuildInstance();


protected:
	virtual void OnConstruction(const FTransform& Transform)override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//コンクリに利用するStaticMesh
	UPROPERTY(EditAnywhere, Category = "Concreate")
	UStaticMesh* ConcreateMesh;

	UPROPERTY(EditAnywhere, Category = "Concreate")
	TSubclassOf<AActor> ConcreateClass;

	//金属に利用するStaticMesh
	UPROPERTY(EditAnywhere, Category = "Metal")
	UStaticMesh* MetalMesh;

	UPROPERTY(EditAnywhere, Category = "Metal")
	TSubclassOf<AActor> MetalClass;

	//レンガに利用するStaticMesh
	UPROPERTY(EditAnywhere, Category = "Blocks")
	UStaticMesh* BlocksMesh;

	UPROPERTY(EditAnywhere, Category = "Blocks")
	TSubclassOf<AActor> BlocksClass;

	//地面に利用するStaticMesh
	UPROPERTY(EditAnywhere, Category = "Ground")
	UStaticMesh* GroundMesh;

	UPROPERTY(EditAnywhere, Category = "Ground")
	TSubclassOf<AActor> GroundMakerClass;

	//ガラスに利用するStaticMesh
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
	//マーカーを取得してHISMに反映する関数
	void BuildInstance();

};
