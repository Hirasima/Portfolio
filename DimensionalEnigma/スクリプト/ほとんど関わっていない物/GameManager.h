
// 23cu0221 Tsuchiya
// レベルが遷移しても値を維持するためのManager

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h" // Log用

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:

	// 初期化
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	virtual void Deinitialize();

	//// エラー回避
	//TStatId GetStatId() const;

	// Playerの位置を設定
	void SetLocation(FVector _location);

	// Playerの位置を返す
	FVector GetLocation();


private:

	FVector m_PlayerLocation;	// ワールドセレクト時の位置

	int m_Steps;				// 移動した回数

	int m_CameraCount;			// カメラの回転回数

};
