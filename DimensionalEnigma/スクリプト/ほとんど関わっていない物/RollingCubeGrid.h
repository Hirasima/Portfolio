#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RollingCubeGrid.generated.h"

UCLASS()
class MYPROJECT_API ARollingCubeGrid : public AActor
{
    GENERATED_BODY()

public:
    ARollingCubeGrid();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    // 立方体メッシュ
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* CubeMesh;

    // マス目のサイズ
    float GridSize;

    // 移動中のフラグ
    bool bIsMoving;

    // 現在の角度
    FQuat CurrentRotation;

    // 回転中心（軸）の位置
    FVector RotationAxis;

    // 目標位置と回転
    FVector TargetPosition;
    FQuat TargetRotation;

    // 移動関数
    void MoveForward();
    void MoveRight();

    // 移動補間
    void InterpolateMovement(float DeltaTime);

    // 回転中心を計算
    FVector CalculateRotationAxis(FVector Direction);
};
