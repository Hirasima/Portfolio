// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
//#include "EngineUtils.h" // For TActorIterator
//#include "Block.cpp"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"        // GetWorld用
#include "EngineUtils.h"        // TActorIterator用
#include "TriangleTile.h"
#include "TriangleTile_Up.h"
#include "GameFramework/PlayerController.h"

// コンストラクタ
ATileManager::ATileManager()
{
    PrimaryActorTick.bCanEverTick = true;

    // メッシュをロード
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMeshFinder.Succeeded())
    {
        CubeMesh = CubeMeshFinder.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Cube mesh in constructor!"));
    }
    
    
    /* // 正三角形のタイルメッシュをロード
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TriangleMeshFinder(TEXT("StaticMesh'/Game/Path/To/Your/TriangleTile.TriangleTile'"));
    if (TriangleMeshFinder.Succeeded())
    {
        CubeMesh = TriangleMeshFinder.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Triangle tile mesh in constructor!"));
    }
    */
   
}

void ATileManager::BeginPlay()
{
    Super::BeginPlay();

    MapData3D.SetNum(100); // 外側の配列を2要素に設定

    for (int32 i = 0; i < MapData3D.Num(); i++) {
        MapData3D[i].SetNum(100); // 中間の配列を2要素に設定
        for (int32 j = 0; j < MapData3D[i].Num(); j++) {
            MapData3D[i][j].SetNum(100); // 内側の配列を2要素に設定
            for (int32 k = 0; k < MapData3D[i][j].Num(); k++) {
                MapData3D[i][j][k] = 0; // 値を0で初期化
            }
        }
    }
}

void ATileManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
    {
        SpawnCubeAtLocation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(0.1f, 0.1f, 0.1f));
    }
    if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
    {
        //RemoveLastSpawnedActor();
        TestCubeGrid();
    }
   
    if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
    {
    }

    if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
    {
        TileGenerate(1,1);
    }
    if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::I))
    {
        TileGenerate(1,-1);
    }
    if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::O))
    {
        TileGenerate(-1,1);
    }
    if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
    {
        TileGenerate(-1,-1);
    }
}

void ATileManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("SpawnCube", IE_Pressed, this, &ATileManager::OnSpawnCubeInput);
}

void ATileManager::OnSpawnCubeInput()
{
    SpawnCubeAtLocation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(0.1f, 0.1f, 0.1f));
}

void ATileManager::SpawnCubeAtLocation(FVector Location, FRotator Rotation, FVector Scale) {
    UWorld* World = GetWorld();
    if (!World) {
        UE_LOG(LogTemp, Error, TEXT("World is nullptr!"));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //AStaticMeshActor* Cube = World->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator, SpawnParams);

    AStaticMeshActor* Cube = Cast<AStaticMeshActor>(GetWorld()->SpawnActor<AActor>(ATriangleTile::StaticClass(), Location, Rotation, SpawnParams));

    //AStaticMeshActor* NewActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Location, Rotation, SpawnParams);
    if (!Cube) {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn AStaticMeshActor!"));
        return;
    }

    UStaticMeshComponent* MeshComp = Cube->GetStaticMeshComponent();
    if (!MeshComp) {
        UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent is nullptr!"));
        return;
    }

    if (CubeMesh) {
        MeshComp->SetMobility(EComponentMobility::Movable); // 可動性をMovableに設定
        //MeshComp->SetStaticMesh(CubeMesh);
        MeshComp->SetWorldScale3D(Scale); // スケールを設定
    }

    // リストに追加
    SpawnedActors.Add(Cube);
    UE_LOG(LogTemp, Log, TEXT("Spawned Cube at Location: %s"), *Location.ToString());
}

void ATileManager::Spawn2DTileAtLocation(FVector Location, FRotator Rotation, FVector Scale) {
    UWorld* World = GetWorld();
    if (!World) {
        UE_LOG(LogTemp, Error, TEXT("World is nullptr!"));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //AStaticMeshActor* Cube = World->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator, SpawnParams);

    AStaticMeshActor* Cube = Cast<AStaticMeshActor>(GetWorld()->SpawnActor<AActor>(ATriangleTile::StaticClass(), Location, Rotation, SpawnParams));

    //AStaticMeshActor* NewActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Location, Rotation, SpawnParams);
    if (!Cube) {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn AStaticMeshActor!"));
        return;
    }

    UStaticMeshComponent* MeshComp = Cube->GetStaticMeshComponent();
    if (!MeshComp) {
        UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent is nullptr!"));
        return;
    }

    if (CubeMesh) {
        MeshComp->SetMobility(EComponentMobility::Movable); // 可動性をMovableに設定
        //MeshComp->SetStaticMesh(CubeMesh);
        MeshComp->SetWorldScale3D(Scale); // スケールを設定
    }

    // リストに追加
    SpawnedActors.Add(Cube);
    UE_LOG(LogTemp, Log, TEXT("Spawned Cube at Location: %s"), *Location.ToString());
}

void ATileManager::RemoveLastSpawnedActor()
{
    if (SpawnedActors.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No actors to remove!"));
        return;
    }

    // 配列の最後のアクターを取得して削除
    AActor* ActorToRemove = SpawnedActors.Pop(); // 配列から削除しつつ取得
    if (ActorToRemove)
    {
        ActorToRemove->Destroy(); // アクターの削除
        UE_LOG(LogTemp, Log, TEXT("Removed actor: %s"), *ActorToRemove->GetName());
    }
}
void ATileManager::RemoveAllSpawnedActors()
{
    for (AActor* Actor : SpawnedActors)
    {
        if (Actor)
        {
            Actor->Destroy();
            UE_LOG(LogTemp, Log, TEXT("Removed actor: %s"), *Actor->GetName());
        }
    }

    // 配列をクリア
    SpawnedActors.Empty();
    UE_LOG(LogTemp, Log, TEXT("Removed all spawned actors."));
}

void ATileManager::TileGenerate(int SignX,int SignY)
{
    //一度出したものを削除
    RemoveAllSpawnedActors();

    //三次元地形情報を取得
    //UpdateMap3DWithBlocks(Map3D, 61, 61, 61, 30);
    FVector GridSize(100, 100, 100);   // グリッドサイズ
    FVector GridOrigin(50, 50, 50);   // 原点
    float GridSpacing = 10.0f;    // グリッドセルの間隔

    UWorld* World = GetWorld();    // 現在のワールドを取得
    if (!World) {
        UE_LOG(LogTemp, Error, TEXT("NoWorld!"));
        return;
    }

    MapData3D = GetCubeGrid(World, GridSize, GridOrigin, GridSpacing);

    //三次元情報を補正した物からタイル生成中心位置を算出しタイル生成座標として二次元情報に記録

    ////まず6枚タイルを置く中心点を記録する
    TArray<TArray<int>> TileSetPoint2D;
    // グリッドのサイズを初期化
    TileSetPoint2D.SetNum((GridSize.X * 4) + 4);
    for (int x = 0; x < (GridSize.X * 4) + 4; ++x)
    {
        TileSetPoint2D[x].SetNum((GridSize.Y * 4) + 4);
        for (int y = 0; y < (GridSize.Y * 4) + 4; ++y)
        {
            TileSetPoint2D[x][y] = 0; // 初期化：すべて0
        }
    }

    //---------------------ここまではOK

    //これはカメラの向きに合わせて変える

    for (int x = 0; x < GridSize.X; ++x)
    {
        for (int y = 0; y < GridSize.Y; ++y)
        {
            for (int z = 0; z < GridSize.Z; ++z)
            {
                if (MapData3D[x][y][z] == 1)
                {
                    //配列外の恐れがある場合はスキップ
                    if (x + (z * SignX) +50>= 0 && y + (z * SignY) +50>=0)
                    {
                        //特殊な補正を掛ける
                                            //高さZの値分XY軸をずらす
                        TileSetPoint2D[x + (z * SignX)+50][y + (z * SignY)+50] = z + 1;
                    }

                    //テストとして元の座標にブロックを設置
                    //SpawnCubeAtLocation(FVector(x * 10, y * 10, z*1.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(1.1f, 1.1f, 1.1f));
                    //範囲内は正しく取得できています

                    //テストとしてずらした座標にブロックを設置
                    SpawnCubeAtLocation(FVector((x + z * SignX) * 10, (y + z * SignY) * 10, -50+z * 10.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(1.1f, 1.1f, 1.1f));

                    //補正なしで記録
                    //TileSetPoint2D[x ][y ] = z + 1;
                }
            }
        }
    }
    //タイル設置位置を決めていく
    TArray<TArray<int>> TilePoint2D;
    // グリッドのサイズを初期化
    TilePoint2D.SetNum((GridSize.X * 10) + 4);
    for (int x = 0; x < (GridSize.X * 10) + 4; ++x)
    {
        TilePoint2D[x].SetNum((GridSize.Y * 5) + 2);
        for (int y = 0; y < (GridSize.Y * 5) + 2; ++y)
        {
            TilePoint2D[x][y] = 0; // 初期化：すべて0
        }
    }

    //個別のタイルを決めていく
    
    //低いとこから優先的に埋めていく
    for (int z = 0; z < GridSize.Z*2; ++z)
    {

        //行列ループ
        for (int x = 0; x < (GridSize.X * 2) ; ++x)
        {
            for (int y = 0; y < (GridSize.Y * 2) ; ++y)
            {

                //0は省く
                if (TileSetPoint2D[x][y] == z&& TileSetPoint2D[x][y]!=0)
                {
                    //両者の符号が不一致(+-,-+)の場合代入を変えなければならない

                    TilePoint2D[(x * 2)][y] = 2;
                    TilePoint2D[(x * 2) + 1][y] = 2;
                    TilePoint2D[(x * 2) + 2][y] = 2;
                    TilePoint2D[(x * 2) + 1][y + 1] = 2;
                    TilePoint2D[(x * 2) + 2][y + 1] = 1;    //
                    TilePoint2D[(x * 2) + 3][y + 1] = 1;    //上面三角形
                    
                    //確認するためにアクターを置いてみる
                    SpawnCubeAtLocation(FVector(x * 10, y * 10, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(1.1f, 1.1f, 1.1f));
                }
            }
        }
    }




    //タイル生成　
    // //二次元情報のXが2ごとにX軸を+√3/2 Y軸を+0.5加える
    // //二次元配列のYが1ごとにX軸を-√3/2 Y軸を+0.5加える
    // //タイルが逆三角として配置するものは180度の回転とX軸に+√3/3加える(X軸奇数&&Y軸偶数==逆三角形||X軸偶数&&Y軸奇数==逆三角形)
    for (int x = 0; x < (GridSize.X * 10) + 2; ++x)
    {
        for (int y = 0; y < (GridSize.Y * 5) + 2; ++y)
        {
            if (TilePoint2D[x][y] == 1|| TilePoint2D[x][y] == 2)
            {
                /*
                 //設置する座標を計算
                float PosX = 0.0f;
                float PosY = 0.0f;
                int i = x / 2;
                PosX += (std::sqrt(3.0f) / 2.0f) * (float)i;
                PosY += 0.5f * (float)i;
                PosX -= (std::sqrt(3.0f) / 2.0f) * y;
                PosY += 0.5f * y;
                */

                //設置する座標を計算
                float PosX = 0.0f;
                float PosY = 0.0f;
                int i = x + 1 / 2;

                PosX += (std::sqrt(3.0f) / 2.0f) * (float)i;
                //PosY += (0.5f * x)- (0.25f * x);

                PosX -= (std::sqrt(3.0f) / 2.0f) * y;
                PosY += 1.0f * y;


                //回転させる値を生成
                float Rot = 90;
                if ((int)x % 2 == 0)
                {
                    Rot = -90;
                }


                //設置
                Spawn2DTileAtLocation(FVector(PosX * 10, PosY * 10, 0.0f), FRotator(0.0f, Rot, 0.0f), FVector(0.5f, 0.5f, 0.5f));
            }
            if (TilePoint2D[x][y] == 2&&false)
            {
                /*
                 //設置する座標を計算
                float PosX = 0.0f;
                float PosY = 0.0f;
                int i = x / 2;
                PosX += (std::sqrt(3.0f) / 2.0f) * (float)i;
                PosY += 0.5f * (float)i;
                PosX -= (std::sqrt(3.0f) / 2.0f) * y;
                PosY += 0.5f * y;
                */

                //設置する座標を計算
                float PosX = 0.0f;
                float PosY = 0.0f;
                int i = x + 1 / 2;

                PosX += (std::sqrt(3.0f) / 2.0f) * (float)i;
                //PosY += (0.5f * x)- (0.25f * x);

                PosX -= (std::sqrt(3.0f) / 2.0f) * y;
                PosY += 1.0f * y;


                //回転させる値を生成
                float Rot = 90;
                if ((int)x % 2 == 0)
                {
                    Rot = -90;
                }


                //設置
                Spawn2DTileAtLocation(FVector(PosX * 10, PosY * 10, -1.0f), FRotator(0.0f, Rot, 0.0f), FVector(0.5f, 0.5f, 0.5f));
            }
        }
    }
    
    

}

TArray<TArray<TArray<int>>> ATileManager::GetCubeGrid(UWorld* World, FVector GridSize, FVector GridCenter, float GridSpacing)
{
    // 結果を格納する3次元配列
    TArray<TArray<TArray<int>>> CubeGrid;

    // グリッドのサイズを初期化
    CubeGrid.SetNum(GridSize.X);
    for (int x = 0; x < GridSize.X; ++x)
    {
        CubeGrid[x].SetNum(GridSize.Y);
        for (int y = 0; y < GridSize.Y; ++y)
        {
            CubeGrid[x][y].SetNum(GridSize.Z);
            for (int z = 0; z < GridSize.Z; ++z)
            {
                CubeGrid[x][y][z] = 0; // 初期化：すべて0
            }
        }
    }

    // ワールドが無効な場合は終了
    if (!World) return CubeGrid;

    // グリッドのオフセットを計算（中心を原点として座標を調整）
    FVector GridOrigin = GridCenter - FVector(GridSize.X / 2.0f, GridSize.Y / 2.0f, GridSize.Z / 2.0f) * GridSpacing;

    // レベル内のすべてのアクターを取得
    for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
    {
        AActor* Actor = *ActorItr;
        if (//(Actor && Actor->GetClass()->GetName().Contains(TEXT("Cube"))) ||
            //(Actor && Actor->GetClass()->GetName().Contains(TEXT("Block"))) ||
            (Actor && Actor->GetClass()->GetName().Contains(TEXT("StaticMeshActor")))) // 名前で判別
        {
            // キューブのワールド座標を取得
            FVector ActorLocation = Actor->GetActorLocation();

            // グリッド座標に変換
            int32 GridX = FMath::FloorToInt((ActorLocation.X - GridOrigin.X) / GridSpacing);
            int32 GridY = FMath::FloorToInt((ActorLocation.Y - GridOrigin.Y) / GridSpacing);
            int32 GridZ = FMath::FloorToInt((ActorLocation.Z - GridOrigin.Z) / GridSpacing);

            // グリッド座標が範囲内である場合に記録
            if (GridX >= 0 && GridX < GridSize.X &&
                GridY >= 0 && GridY < GridSize.Y &&
                GridZ >= 0 && GridZ < GridSize.Z)
            {
                CubeGrid[GridX][GridY][GridZ] = 1; // キューブが存在する場所に1を設定
            }
        }
    }

    return CubeGrid;
}
void ATileManager::TestCubeGrid()
{
    FVector GridSize(100, 100, 100);   // グリッドサイズ
    FVector GridOrigin(50, 50, 50);   // 原点
    float GridSpacing = 10.0f;    // グリッドセルの間隔

    UWorld* World = GetWorld();    // 現在のワールドを取得
    if (!World) {
        UE_LOG(LogTemp, Error, TEXT("NoWorld!"));
        return;
    }

    TArray<TArray<TArray<int>>> CubeGrid = GetCubeGrid(World, GridSize, GridOrigin, GridSpacing);

    // 結果をログに出力
    for (int x = 0; x < GridSize.X; ++x)
    {
        for (int y = 0; y < GridSize.Y; ++y)
        {
            for (int z = 0; z < GridSize.Z; ++z)
            {
                if (CubeGrid[x][y][z] == 1)
                {
                    UE_LOG(LogTemp, Log, TEXT("Cube found at (%d, %d, %d)"), x, y, z);
                    SpawnCubeAtLocation(FVector(x * 10.0f - 450.0f, y * 10.0f - 450.0f, z * 10.0f - 450.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(0.11f, 0.11f, 0.11f));
                    //SpawnCubeAtLocation(FVector(x * 10.0f , y * 10.0f , z * 10.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(0.11f, 0.11f, 0.11f));
                }
                else
                    if (CubeGrid[x][y][z] == 0)
                    {
                        //UE_LOG(LogTemp, Log, TEXT("NoCube found at (%d, %d, %d)"), x, y, z);
                    }
            }
        }
    }
}

// アクターを非表示にする関数
void ATileManager::HideActor(AActor* TargetActor)
{
    if (TargetActor)
    {
        // アクターが持つすべてのメッシュコンポーネントを非表示にする
        TArray<UStaticMeshComponent*> MeshComponents;
        TargetActor->GetComponents<UStaticMeshComponent>(MeshComponents);

        for (UStaticMeshComponent* MeshComponent : MeshComponents)
        {
            MeshComponent->SetVisibility(false, true);  // メッシュを非表示にする
        }
    }
}

// アクターを表示する関数
void ATileManager::ShowActor(AActor* TargetActor)
{
    if (TargetActor)
    {
        // アクターが持つすべてのメッシュコンポーネントを表示する
        TArray<UStaticMeshComponent*> MeshComponents;
        TargetActor->GetComponents<UStaticMeshComponent>(MeshComponents);

        for (UStaticMeshComponent* MeshComponent : MeshComponents)
        {
            MeshComponent->SetVisibility(true, true);  // メッシュを表示する
        }
    }
}
