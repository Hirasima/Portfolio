#include "RollingCubeGrid.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"

ARollingCubeGrid::ARollingCubeGrid()
{
    PrimaryActorTick.bCanEverTick = true;

    // メッシュの作成
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    RootComponent = CubeMesh;

    // 初期値
    GridSize = 100.0f; // マス目のサイズ
    bIsMoving = false;

    TargetPosition = GetActorLocation();
    CurrentRotation = GetActorQuat();
    TargetRotation = CurrentRotation;
}

void ARollingCubeGrid::BeginPlay()
{
    Super::BeginPlay();
}

void ARollingCubeGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 移動中でない場合、入力をチェックして動作を開始
    if (!bIsMoving)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            if (PlayerController->IsInputKeyDown(EKeys::W))
            {
                MoveForward();
            }
            else if (PlayerController->IsInputKeyDown(EKeys::D))
            {
                MoveRight();
            }
        }
    }

    // 移動中の補間
    if (bIsMoving)
    {
        InterpolateMovement(DeltaTime);
    }
}

void ARollingCubeGrid::MoveForward()
{
    if (!bIsMoving)
    {
        FVector Direction = FVector(GridSize, 0, 0); // 前方向
        RotationAxis = CalculateRotationAxis(Direction);

        TargetPosition = GetActorLocation() + Direction;
        TargetRotation = FQuat(RotationAxis, FMath::DegreesToRadians(90)) * CurrentRotation;

        bIsMoving = true;
    }
}

void ARollingCubeGrid::MoveRight()
{
    if (!bIsMoving)
    {
        FVector Direction = FVector(0, GridSize, 0); // 右方向
        RotationAxis = CalculateRotationAxis(Direction);

        TargetPosition = GetActorLocation() + Direction;
        TargetRotation = FQuat(RotationAxis, FMath::DegreesToRadians(90)) * CurrentRotation;

        bIsMoving = true;
    }
}

void ARollingCubeGrid::InterpolateMovement(float DeltaTime)
{
    FVector CurrentPosition = GetActorLocation();
    FQuat CurrentQuat = GetActorQuat();

    // 位置の補間
    FVector NewPosition = FMath::VInterpTo(CurrentPosition, TargetPosition, DeltaTime, 5.0f);

    // 回転の補間
    FQuat NewRotation = FMath::QInterpTo(CurrentQuat, TargetRotation, DeltaTime, 5.0f);

    // アクターの更新
    SetActorLocation(NewPosition);
    SetActorRotation(NewRotation);

    // 移動完了の確認
    if (NewPosition.Equals(TargetPosition, 1.0f) && NewRotation.Equals(TargetRotation, 1.0f))
    {
        bIsMoving = false;
        CurrentRotation = TargetRotation;
    }
}

FVector ARollingCubeGrid::CalculateRotationAxis(FVector Direction)
{
    // 移動方向に基づいて回転中心を計算
    FVector Offset = Direction * 0.5f; // マス目の半分だけオフセット
    return (GetActorLocation() + FVector(0, 0, -GridSize * 0.5f) + Offset);
}
