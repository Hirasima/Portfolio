// WarpTile.cpp
// 23cu0228 hanawa

#include "WarpTile.h"
#include "Player3D.h"
#include "MappingCamera3D.h"

// Sets default values
AWarpTile::AWarpTile()
	: m_WarpType(EWarpType::UP)
	, m_WarpDirectionNums()
	, m_pWarpTiles()
	, m_WarpDistination(FVector::ZeroVector)
	, m_pCameraActor(nullptr)
	, m_IsWarpable(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWarpTile::BeginPlay()
{
	Super::BeginPlay();
	
	// Tag を設定
	Tags.Add(FName(TEXT("WarpTile")));

	// GameManagerを取得
	world = GetWorld();
	if (world) {
		gameinstance = Cast<UEnigmaGameInstance>(world->GetGameInstance());
		if (gameinstance) {
			gamemanager = world->GetSubsystem<UStageSubSystem>();
		}
	}
}

// Called every frame
void AWarpTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// プレイヤーを錯視移動(ワープ)させる処理
// 引数には Player3D で指定
void AWarpTile::WarpPlayer(AActor* Player)
{
	m_IsWarpable = false;
	AMappingCamera3D* pCamera = Cast<AMappingCamera3D>(m_pCameraActor) ;

	if (pCamera)
	{
		for (int j = 0; j < sizeof(m_WarpDirectionNums)/sizeof(m_WarpDirectionNums[0]); ++j)
		{
			m_IsWarpable = false;
			m_IsWarped = false;

			// カメラの画角番号とワープ方向番号が一致する場合処理を行う
			if (gamemanager->DirectionCamNumber == m_WarpDirectionNums[j])
			{
				if (m_WarpDirectionNums[j] == j)
				{
					if (!m_pWarpTiles[j]) { return; }

					// プレイヤーを移動(ワープ)可能とし、移動対象のワープタイルの座標を得る
					m_IsWarpable = true;
					m_WarpDistination = m_pWarpTiles[j]->GetActorLocation();

					//大竹の修正コード:02/02
					//自身と移動先の高さを比べて分岐させる
					EWarpType ResultWarpType = EWarpType::UP;
					if (this->GetActorLocation().Z >= m_WarpDistination.Z)
					{
						ResultWarpType = EWarpType::DOWN;
					}
					else
					{
						ResultWarpType= EWarpType::UP;
					}

					//結果を入れる↓
					switch (ResultWarpType)//旧→switch (m_WarpType)
					{
					case EWarpType::UP:
						// プレイヤーを移動対象のタイル上に移動させる
						m_IsWarped = true;
						Player->SetActorLocation(FVector(m_WarpDistination.X, m_WarpDistination.Y, m_WarpDistination.Z + 10.f));
						UE_LOG(LogTemp, Warning, TEXT("UP Warpable"));
						break;

					case EWarpType::DOWN:
						// プレイヤーを移動対象のタイル上に移動させる
						m_IsWarped = true;

						//Player->SetActorLocation(FVector(m_WarpDistination.X, m_WarpDistination.Y, m_WarpDistination.Z + 10.f));
						UE_LOG(LogTemp, Warning, TEXT("DOWN Warpable %s"), *m_WarpDistination.ToString());
						break;
					}
					return;
				}
				UE_LOG(LogTemp, Log, TEXT("Not DirectionNums[%d] == %d"), m_WarpDirectionNums[j], j)
			}
			// 画角番号と方向番号が一致しない場合処理を行わない
			else
			{
				// ワープ不可能とする
				m_IsWarpable = false;
				m_IsWarped = false;

				UE_LOG(LogTemp, Log, TEXT("Not Warpable, GDNum:%d WDNum:%d"), pCamera->GetDirectionNumber(), m_WarpDirectionNums[j]);
			}
			UE_LOG(LogTemp, Log, TEXT("DirectionNumber is %d"), m_WarpDirectionNums[j]);
		}
	}
	
}

// 移動(ワープ)可能かどうか返す処理
bool AWarpTile::GetIsWarp() const
{
	return m_IsWarpable;
}

bool AWarpTile::GetIsWarped() const
{
	return m_IsWarped;
}

FVector AWarpTile::GetDownWarpLocation()
{
	return FVector(m_WarpDistination.X, m_WarpDistination.Y, m_WarpDistination.Z + 10.f);
}