// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "StageSubSystem.generated.h"

#define STAGE_SIZE 100

UENUM()
enum class EDimensionType : uint8
{
	ThreeD = 0,
	TwoD = 1,
};

/**
 *
 */
UCLASS(Blueprintable)
class MYPROJECT_API UStageSubSystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	//初期化時に呼び出される関数
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);

	//終了時に呼び出される関数
	virtual void Deinitialize() override;
	
	//--------------------------------------------
	//Tick関数
	//--------------------------------------------
	//毎フレーム処理
	virtual void Tick(float DeltaTime) override;

	//Tick可能かどうか
	virtual bool IsTickable() const override;

	// TickableオブジェクトのIDを取得する
	virtual TStatId GetStatId() const override;

	//--------------------------------------------
	//オリジナルメソッド
	//--------------------------------------------

	//プレイヤーが動いた回数を加算
	void AddPlayerMoveCount();

	//プレイヤーが動いた回数を取得
	UFUNCTION(BlueprintCallable)
	int GetPlayerMoveCount();

	//プレイヤーが動いているか設定
	void SetPlayerMoving(bool _status);

	//プレイヤーが動いているか取得
	bool GetPlayerMoving();

	//カメラが動いた回数を加算
	void AddCameraMoveCount();

	//カメラが動いた回数を取得
	UFUNCTION(BlueprintCallable)
	int GetCameraMoveCount();

	//カメラが動いているか設定
	void SetCameraMoving(bool _status);

	//カメラが動いているか取得
	bool GetCameraMoving();	

	void SetDimensionType(const EDimensionType _DimensionType);

	EDimensionType GetDimensionType();

	void SetIsDimensionChange(const bool _DimensionChange);

	bool GetIsDimensionChange();

	//ステージの経過時間を取得
	UFUNCTION(BlueprintCallable)
	float GetStageElapsedTime();

	//クリアタイムの取得
	UFUNCTION(BlueprintCallable)
	float GetClearTime();

	//ランクのテクスチャを取得
	FString GetRankTexturePath();

	//クリアタイムのランキングデータを取得
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetClearTimeRanking();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetPlayerMoveCountRanking();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetCameraMoveCountRanking();

	//ゴールしているかを取得
	bool GetGoaling();

	void SetIsSelect(bool _IsSelect);

	bool GetIsSelect();

	EDimensionType DimensionType;

	int DirectionCamNumber;

	//ゴールした時に呼ばれる
	void OnGoal(FName& _nextLevelName);


//--------------------------------------------------------
//プライベートメソッド
//--------------------------------------------------------
private:
	//リザルト時に呼ばれる
	void OnResult();

	//リザルトデータを初期化
	void SetResultSaveData(FString _string);

	//ノルマデータを設定
	void SetQuotaData(FString _string);

	//リザルトデータをセーブ
	void SaveResultSaveData();

	//ランキングデータを設定
	void SetRankingData();

	//最も小さいデータを返す
	int GetLeastIntNum(TArray<int> _array);

	int GetLeastFloatNum(TArray<float> _array);

	//ウィジェットを切り替える
	void ChangeWidget(int _widgetNum);

	//ファイルの読み込み
	FString ReadFile(FString _fileDir);

	//ファイルの書き込み
	void WriteFile(FString _fileDir, FString _saveString);

	//ウィジェットを読み込み
	void LoadWedget();



private:

	int m_widgetNum;

	TArray<UUserWidget*> m_widgetArray;

	//ステージの状態の列挙型
	enum StageStatus
	{
		ThreeD,
		TwoD,
	};

	//ステージの状態
	StageStatus m_status;

	//経過時間
	float m_elapsedTime;

	//ランクのテクスチャ
	UTexture2D* m_rankTexture;

	//クリアタイム
	float m_clearTime;

	//プレイヤーの動いた回数
	int m_playerMoveCount;

	//カメラが動いた回数
	int m_cameraMoveCount;

	bool m_playerMoving;

	bool m_cameraMpving;

	bool m_isDimensionChange;

	//ゴールしているか
	bool m_goaling;

	bool m_IsSelect;

	//次のレベルの名前
	FName m_nextLevelName;


	struct ResultData
	{
		float clearTime;
		int playerMoveCount;
		int cameraMoveCount;

		ResultData(float _clearTime = 0.0f,int _playerMoveCount = 0,int _cameraMoveCount = 0)
			:clearTime(_clearTime)
			,playerMoveCount(_playerMoveCount)
			,cameraMoveCount(_cameraMoveCount)
		{

		}
	};
	TArray<ResultData> m_resultDataArray;

	ResultData m_quotaData;

	TArray<int> m_clearTimeRanking;

	TArray<int> m_playerMoveRanking;

	TArray<int> m_cameraMoveRanking;

	//-------------------------------------------
	//インプット関連プロパティ
	//-------------------------------------------

	/*UInputMappingContext* m_inputMappingContext;

	UInputAction* m_BAction;
	UInputAction* m_StartAction;
	UInputAction* m_RBAction;
	UInputAction* m_LBAction;*/
};
