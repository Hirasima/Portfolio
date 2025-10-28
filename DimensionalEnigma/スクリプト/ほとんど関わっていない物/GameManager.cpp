// 23cu0221 Tsuchiya


#include "Gamemanager.h"


// 初期化
void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
    UE_LOG(LogTemp, Warning, TEXT("UGameInstanceSubsystem::Initialize()"));

    m_Steps = 0;

    Super::Initialize(Collection);
}

// 破棄
void UGameManager::Deinitialize()
{
    UE_LOG(LogTemp, Warning, TEXT("UGameInstanceSubsystem::Deinitialize()"));

    Super::Deinitialize();
}

// Playerの位置を設定
// ワールドマップで選択したワールドの位置を保存しておくため
void UGameManager::SetLocation(FVector _location)
{
    m_PlayerLocation = _location;
}

// Playerの位置を返す
// ワールドマップで選択したワールドの前に移動するため
FVector UGameManager::GetLocation()
{
    return m_PlayerLocation;
}

// --------------------------------------------------------
//// Tickを使用するために必須
//TStatId UGameManager::GetStatId() const
//{
//    RETURN_QUICK_DECLARE_CYCLE_STAT(UMyTickableWorldSubsystem, STATGROUP_Tickables);
//}