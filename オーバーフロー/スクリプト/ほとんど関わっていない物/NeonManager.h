// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeonManager.generated.h"

//クラスの前方宣言
class UHierarchicalInstancedStaticMeshComponent;
class ANeonMaker;

UCLASS()
class ORIGINALACTION_API ANeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeonManager();

	//エディタでマーカからの反映を呼ぶために
	UFUNCTION(CallInEditor,Category="Neon")
	void ReBuildInstance();

	//★仮置きでエディタから呼んでみたい:点灯
	UFUNCTION(CallInEditor, Category = "Neon")
	void BePointNeon();
	//★仮置きでエディタから呼んでみたい:消灯
	UFUNCTION(CallInEditor, Category = "Neon")
	void OffPointNeon();

	//警戒状態にネオンの色を変える関数
	void BeAlert();

protected:
	virtual void OnConstruction(const FTransform& Transform)override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//ネオンに利用するStaticMesh
	UPROPERTY(EditAnywhere,Category="Neon")
	UStaticMesh* NeonSticMesh;

	UPROPERTY(EditAnywhere, Category = "Neon")
	TSubclassOf<AActor> NeonSticMakerClass;

	//ビルに利用するStaticMesh
	UPROPERTY(EditAnywhere, Category = "Building")
	UStaticMesh* UnLightBillMesh;

	UPROPERTY(EditAnywhere, Category = "Building")
	TSubclassOf<AActor> UnLightBillClass;

	//ビルに利用するStaticMesh2
	UPROPERTY(EditAnywhere, Category = "Building")
	UStaticMesh* BillMesh;

	UPROPERTY(EditAnywhere, Category = "Building")
	TSubclassOf<AActor> BillMakerClass;


	//看板に利用するStaticMesh
	UPROPERTY(EditAnywhere, Category = "Boad")
	UStaticMesh* NeonBoadMesh;

	UPROPERTY(EditAnywhere, Category = "Boad")
	TSubclassOf<AActor> NeonBoadMakerClass;

	//地面に利用するStaticMesh
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
	//マーカーを取得してHISMに反映する関数
	void BuildInstance();
};
