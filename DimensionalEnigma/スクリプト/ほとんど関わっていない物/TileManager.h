// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/StaticMeshActor.h"

#include "TileManager.generated.h"

UCLASS()
class MYPROJECT_API ATileManager : public APawn
{
    GENERATED_BODY()

public:
    ATileManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // アクターを非表示にする関数
    UFUNCTION(BlueprintCallable, Category = "ActorVisibility")
    void HideActor(AActor* TargetActor);

    // アクターを表示する関数
    UFUNCTION(BlueprintCallable, Category = "ActorVisibility")
    void ShowActor(AActor* TargetActor);

private:
    UPROPERTY()
    UStaticMesh* CubeMesh;

    void OnSpawnCubeInput();
    void SpawnCubeAtLocation(FVector Location, FRotator Rotation, FVector Scale);
    void Spawn2DTileAtLocation(FVector Location, FRotator Rotation, FVector Scale);
    void RemoveLastSpawnedActor();
    void RemoveAllSpawnedActors();
    TArray<TArray<TArray<int>>> GetCubeGrid(UWorld* World, FVector GridSize, FVector GridCenter, float GridSpacing);
    void TestCubeGrid();

    void TileGenerate(int SignX, int SignY);
    //void UpdateMap3DWithBlocks(TArray<TArray<TArray<int>>>& Map3D, int GridSizeX, int GridSizeY, int GridSizeZ, float BlockSize);


    TArray<TArray<TArray<int>>> MapData3D;

    

    //タイル生成中心位置を二次元配列として保存

    TArray<AActor*> SpawnedActors;
};