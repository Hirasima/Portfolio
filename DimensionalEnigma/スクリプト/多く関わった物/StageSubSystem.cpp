// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSubSystem.h"
#include "GameFramework/Actor.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "CameraPawn0332.h"
#include "camera2D.h"
#include "GoalTile.h"
#include "BlockActor.h"
#include "Blueprint/UserWidget.h"
#include "SoundManagerSubsystem.h"
#include "ItemManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputCoreTypes.h"
#include <limits>

#define QUOTADATA_DIRE FPaths::ProjectDir() + TEXT("CSVFile/QuotaData/")
#define RANKINGDATA_DIRE FPaths::ProjectDir() + TEXT("CSVFile/RankingData/")

//初期化時に呼び出される関数
void UStageSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_status = ThreeD;
	m_elapsedTime = 0.0f;
	m_clearTime = 0.0f;
	m_playerMoveCount = 0;
	m_cameraMoveCount = 0;
	m_playerMoving = false;
	m_cameraMpving = false;
	m_goaling = false;
	m_IsSelect = false;
	m_nextLevelName = TEXT("");

	//パス
	FString fileDir = ("");
	//読み取り用変数
	FString fileString("");

	//ファイルパス
	fileDir = FPaths::ProjectContentDir() + TEXT("CSVFile/QuotaData/");

	//ファイル読み込み
	fileString = ReadFile(QUOTADATA_DIRE);

	//ノルマデータを設定
	SetQuotaData(fileString);

	// ワールドの初期化完了後に呼ばれるイベントにバインド
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UStageSubSystem::OnWorldInitialized);

	UE_LOG(LogTemp, Warning, TEXT("Generated StageSubSystem"))
}


void UStageSubSystem::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
	//レベル名取得
	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	//ワールドセレクトでなければ
	if (levelName != "WorldSelect")
	{
		//アイテムウィジェット生成
		GetWorld()->GetSubsystem<UItemManager>()->GenerateItemWidget();
	}
}

//終了時に呼び出される関数
void UStageSubSystem::Deinitialize()
{
	for (int i = 0; i < m_widgetArray.Num(); ++i)
	{
		//ウィジェット削除
		m_widgetArray[i]->RemoveFromParent();
	}

	Super::Deinitialize();

	UE_LOG(LogTemp, Warning, TEXT("Deleted StageSubSystem"))
}


//--------------------------------------------
//Tick関数
//--------------------------------------------

void UStageSubSystem::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Log, TEXT("Stage DeltaTime Is %f"), DeltaTime)

	//経過時間に加算
	m_elapsedTime += DeltaTime;

	//ゴールしていれば
	if (m_goaling)
	{
		//リザルトへ
		OnResult();
	}
}

//Tick可能かどうか
bool UStageSubSystem::IsTickable() const
{
	return true;
}

// TickableオブジェクトのIDを取得する
TStatId UStageSubSystem::GetStatId()const
{
	//UE_LOG(LogTemp, Warning, TEXT("StageSubSystem :: GetStatId()"))
	RETURN_QUICK_DECLARE_CYCLE_STAT(UStageSubSystem, STATGROUP_Tickables);
}

//--------------------------------------------
//オリジナルメソッド
//--------------------------------------------

//プレイヤーが動いた回数を加算
void UStageSubSystem::AddPlayerMoveCount()
{
	//ゴールしていなければ
	if (!m_goaling)
	{
		//プレイヤー歩数加算
		++m_playerMoveCount;
		//プレイヤーの状態更新
		SetPlayerMoving(false);
		UE_LOG(LogTemp, Log, TEXT("Add Player Move Count : %d"), m_playerMoveCount)
	}
}

//プレイヤーが動いた回数を取得
int UStageSubSystem::GetPlayerMoveCount()
{
	return m_playerMoveCount;
}

//プレイヤーが動いているか設定
void UStageSubSystem::SetPlayerMoving(bool _status)
{
	m_playerMoving = _status;
}

//プレイヤーが動いているか取得
bool UStageSubSystem::GetPlayerMoving()
{
	return m_playerMoving;
}

//カメラが動いた回数を加算
void UStageSubSystem::AddCameraMoveCount()
{
	//ゴールしていなければ
	if (!m_goaling)
	{
		//カメラの回転回数加算
		++m_cameraMoveCount;
		SetCameraMoving(false);
		UE_LOG(LogTemp, Log, TEXT("Add Camera Move Count : %d"), m_cameraMoveCount)
	}
}

//プレイヤーが動いた回数を取得
int UStageSubSystem::GetCameraMoveCount()
{
	return m_cameraMoveCount;
}

//カメラが動いているか設定
void UStageSubSystem::SetCameraMoving(bool _status)
{
	m_cameraMpving = _status;
}

//カメラが動いているか取得
bool UStageSubSystem::GetCameraMoving()
{
	return m_cameraMpving;
}

void UStageSubSystem::SetDimensionType(const EDimensionType _DimensionType)
{
	DimensionType = _DimensionType;
}

EDimensionType UStageSubSystem::GetDimensionType()
{
	return DimensionType;
}

void UStageSubSystem::SetIsDimensionChange(const bool _DimensionChange)
{
	m_isDimensionChange = _DimensionChange;
}

bool UStageSubSystem::GetIsDimensionChange()
{
	return m_isDimensionChange;
}

//ステージの経過時間を取得
float UStageSubSystem::GetStageElapsedTime()
{
	return m_elapsedTime;
}

//クリアタイムの取得
float UStageSubSystem::GetClearTime()
{
	if (!m_goaling)
	{
		m_clearTime = m_elapsedTime;
	}

	return m_clearTime;
}

//ランクのテクスチャを取得
FString UStageSubSystem::GetRankTexturePath()
{
	//テクスチャのパス
	TArray<FString> texturePath =
	{
		"/Game/Texture/UI_Rank_C.UI_Rank_C",
		"/Game/Texture/UI_Rank_B.UI_Rank_B",
		"/Game/Texture/UI_Rank_A.UI_Rank_A",
		"/Game/Texture/UI_Rank_S.UI_Rank_S"
	};

	//読み込むパス番号
	int pathNum = 0;

	//クリアタイムがノルマ以下なら
	if (m_clearTime <= m_quotaData.clearTime)
	{
		++pathNum;
	}
	//プレイヤー歩数がノルマ以下なら
	if (m_playerMoveCount <= m_quotaData.playerMoveCount)
	{
		++pathNum;
	}
	//カメラ回転回数がノルマ以下なら
	if (m_cameraMoveCount <= m_quotaData.cameraMoveCount)
	{
		++pathNum;
	}

	//UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *texturePath[pathNum]);

	return texturePath[pathNum];
}

TArray<FString> UStageSubSystem::GetClearTimeRanking()
{
	//クリアタイムデータ
	TArray<FString> timeArray;
	//初期化
	timeArray.Init(FString("No Data"), 5);

	//上位5位まで
	for (int i = 0; i < 5; ++i)
	{
		//データが有効なら
		if (m_clearTimeRanking.IsValidIndex(i))
		{
			//データを読み込み
			timeArray[i] = FString::SanitizeFloat(m_resultDataArray[m_clearTimeRanking[i]].clearTime);
		}
	}

	return timeArray;
}

TArray<FString> UStageSubSystem::GetPlayerMoveCountRanking()
{
	//歩数データ
	TArray<FString> stepArray;
	stepArray.SetNum(5);

	//上位5位まで
	for (int i = 0; i < 5; ++i)
	{
		//データが有効なら
		if (m_playerMoveRanking.IsValidIndex(i))
		{
			//データを読み込み
			stepArray[i] = FString::FromInt(m_resultDataArray[m_playerMoveRanking[i]].playerMoveCount);
		}
		//有効でないなら
		else
		{
			stepArray[i] = FString("No Data");
		}
	}

	return stepArray;
}

TArray<FString> UStageSubSystem::GetCameraMoveCountRanking()
{
	//カメラ回転回数データ
	TArray<FString> cameraArray;

	cameraArray.SetNum(5);

	//上位5位まで
	for (int i = 0; i < 5; ++i)
	{
		//データが有効なら
		if (m_cameraMoveRanking.IsValidIndex(i))
		{
			//データを読み込み
			cameraArray[i] = FString::FromInt(m_resultDataArray[m_cameraMoveRanking[i]].cameraMoveCount);
		}
		//有効でないなら
		else
		{
			cameraArray[i] = FString("No Data");
		}
	}

	return cameraArray;
}

//ゴールしているかを取得
bool UStageSubSystem::GetGoaling()
{
	return m_goaling;
}

void UStageSubSystem::SetIsSelect(bool _IsSelect) {
	m_IsSelect = _IsSelect;
}

bool UStageSubSystem::GetIsSelect() {
	return m_IsSelect;
}

//ゴールした時に呼ばれる
void UStageSubSystem::OnGoal(FName& _nextLevelName)
{
	//ゴール済みに変更
	m_goaling = true;
	//次のレベルの取得
	m_nextLevelName = _nextLevelName;
	//クリアタイムに経過時間を代入
	m_clearTime = m_elapsedTime;

	m_widgetNum = -1;

	//パス
	FString fileDir = FPaths::ProjectContentDir() + TEXT("CSVFile/RankingData/");
	//読み取り用変数
	FString fileString("");
	//ファイル読み込み
	fileString = ReadFile(RANKINGDATA_DIRE);
	//リザルトデータ設定
	SetResultSaveData(fileString);

	//今回のリザルトデータを追加
	ResultData resultData(m_clearTime, m_playerMoveCount, m_cameraMoveCount);
	m_resultDataArray.Add(resultData);
	//リザルトデータをセーブ
	SaveResultSaveData();
	//ランキングデータ設定
	SetRankingData();
	//ウィジェット読み込み
	LoadWedget();
	//アイテムウィジェット削除
	GetWorld()->GetSubsystem<UItemManager>()->RemoveItemWidget();
}

//リザルト時に呼ばれる
void UStageSubSystem::OnResult()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resulting"))

	//クリア時間から一定時間経ったら無視
	if (m_elapsedTime - m_clearTime < 1.5f) return;
	//一定時間ないなら
	else
	{
		//レベルの取得
		UWorld* world = GetWorld();
		//ヌルチェック
		if (!world) return;

		//プレイヤーコントローラー取得
		APlayerController* playerController = world->GetFirstPlayerController();
		//ヌルチェック
		if (!playerController) return;

		if (playerController->WasInputKeyJustPressed(EKeys::Enter)
		|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
		{
			//次のレベルへ
			UGameplayStatics::OpenLevel(world, m_nextLevelName);
		}

		//ランキング表示時
		if (m_widgetNum == 0)
		{
			if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
			{
				//ウィジェット変更
				ChangeWidget(1);
			}
		}
		else if (m_widgetNum == 1)
		{
			if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
			{
				//ウィジェット変更
				ChangeWidget(0);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Right)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_RightShoulder))
			{
				//ウィジェット変更
				ChangeWidget(2);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Left)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftShoulder))
			{
				//ウィジェット変更
				ChangeWidget(3);
			}
		}
		else if(m_widgetNum == 2)
		{
			if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
			{
				ChangeWidget(0);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Right)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_RightShoulder))
			{
				ChangeWidget(3);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Left)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftShoulder))
			{
				ChangeWidget(1);
			}
		}
		else if (m_widgetNum == 3)
		{
			if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Bottom))
			{
				ChangeWidget(0);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Right)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_RightShoulder))
			{
				ChangeWidget(1);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Left)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftShoulder))
			{
				ChangeWidget(2);
			}
		}
		else
		{
			ChangeWidget(0);
		}
	}
}

//
void UStageSubSystem::SetResultSaveData(FString _string)
{
	//リザルトデータ
	TArray<FString> resultDataArray;

	//文字列ごとに分割
	_string.ParseIntoArrayLines(resultDataArray);

	//初期化
	m_resultDataArray.Init(ResultData(), resultDataArray.Num());

	//データ数分繰り返し
	for (int i = 0; i < resultDataArray.Num(); ++i)
	{
		//一つ一つデータ保存用
		TArray<FString> resultData;
		//","で分ける
		resultDataArray[i].ParseIntoArray(resultData, TEXT(","));

		//データが3つ以上なら
		if (resultData.Num() >= 3)
		{
			//データを読み込み
			ResultData data(FCString::Atof(*resultData[0]), FCString::Atoi(*resultData[1]), FCString::Atoi(*resultData[2]));
			m_resultDataArray[i] = data;
		}
	}
}

//ノルマデータを設定
void UStageSubSystem::SetQuotaData(FString _string)
{
	//ノルマデータ
	TArray<FString> quotaArray;

	//行データ
	TArray<FString> lineArray;

	//行ごとに分割
	_string.ParseIntoArrayLines(lineArray);

	for (int i = 0; i < lineArray.Num(); ++i)
	{
		//一つ一つのデータ
		TArray<FString> dotArray;

		//","で分ける
		lineArray[i].ParseIntoArray(dotArray, TEXT(","));

		//結合
		quotaArray += dotArray;
	}
	//データが3つ以上なら
	if (quotaArray.Num() >= 3)
	{
		//ノルマデータセット
		m_quotaData = ResultData(FCString::Atof(*quotaArray[0]), FCString::Atoi(*quotaArray[1]), FCString::Atoi(*quotaArray[2]));
		UE_LOG(LogTemp, Log, TEXT("Set Quota Data : %f %d %d"), m_quotaData.clearTime, m_quotaData.playerMoveCount, m_quotaData.cameraMoveCount);
	}
	else
	{
		//空データセット
		m_quotaData = ResultData(0.0f, 0, 0);
	}
}

//リザルトデータをセーブ
void UStageSubSystem::SaveResultSaveData()
{
	//セーブするデータ
	FString saveData = TEXT("");

	UE_LOG(LogTemp, Log, TEXT("Saveing Result Data"))

	for (int i = 0; i < m_resultDataArray.Num(); ++i)
	{
		//リザルトデータ
		FString resultData = "";
		resultData.Append(FString::SanitizeFloat(m_resultDataArray[i].clearTime) + TEXT(","));
		resultData.Append(FString::FromInt(m_resultDataArray[i].playerMoveCount) + TEXT(","));
		resultData.Append(FString::FromInt(m_resultDataArray[i].cameraMoveCount) + TEXT("\n"));

		UE_LOG(LogTemp, Log, TEXT("%s"), *resultData)

		//セーブデータに追加
		saveData.Append(resultData);
	}

	//ファイルパス
	FString filePath = FPaths::ProjectContentDir() + TEXT("CSVFile/RankingData/");

	//セーブ
	WriteFile(RANKINGDATA_DIRE, saveData);
}

//ランキングデータを設定
void UStageSubSystem::SetRankingData()
{
	//有効チェック
	if (m_resultDataArray.IsEmpty())
	{
		return;
	}

	//ランキングデータ
	TArray<float> timeRanking;
	TArray<int> stepRanking;
	TArray<int> angleRankig;
	
	//データを仕分け
	for (int i = 0; i < m_resultDataArray.Num(); ++i)
	{
		timeRanking.Add(m_resultDataArray[i].clearTime);
		stepRanking.Add(m_resultDataArray[i].playerMoveCount);
		angleRankig.Add(m_resultDataArray[i].cameraMoveCount);
	}

	//ランキング順位分繰り返す
	for (int i = 0; i < 5; ++i)
	{
		//----------------------------------------------------
		//クリアタイム
		//----------------------------------------------------

		int leastNum = GetLeastFloatNum(timeRanking);
		if (timeRanking[leastNum] != MAX_flt)
		{
			m_clearTimeRanking.Add(leastNum);
			timeRanking[leastNum] = MAX_flt;
		}

		//----------------------------------------------------
		//歩数カウンター
		//----------------------------------------------------

		leastNum = GetLeastIntNum(stepRanking);
		if (stepRanking[leastNum] != INT_MAX)
		{
			m_playerMoveRanking.Add(leastNum);
			stepRanking[leastNum] = INT_MAX;
		}

		//----------------------------------------------------
		//カメラ回転カウンター
		//----------------------------------------------------

		leastNum = GetLeastIntNum(angleRankig);
		if (angleRankig[leastNum] != INT_MAX)
		{
			m_cameraMoveRanking.Add(leastNum);
			angleRankig[leastNum] = INT_MAX;
		}
	}
}

int UStageSubSystem::GetLeastFloatNum(TArray<float> _array)
{
	//一番小さいデータの要素数
	int least = 0;
	if (!_array.IsEmpty())
	{
		for (int i = 1; i < _array.Num(); ++i)
		{
			if (_array[least] > _array[i])
			{
				least = i;
			}
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("Least Data Is : %d  %f"), least, _array[least])
	return least;
}

//最も小さいデータを返す
int UStageSubSystem::GetLeastIntNum(TArray<int> _array)
{
	//一番小さいデータの要素数
	int least = 0;
	if (!_array.IsEmpty())
	{
		for (int i = 1; i < _array.Num(); ++i)
		{
			if (_array[least] > _array[i])
			{
				least = i;
			}
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("Least Data Is : %d  %f"), least, _array[least])
	return least;
}

//ウィジェットを切り替える
void UStageSubSystem::ChangeWidget(int _widgetNum)
{
	if (m_widgetArray.IsValidIndex(m_widgetNum))
	{
		m_widgetArray[m_widgetNum]->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Now Widget Is Get Lost"));
	}

	if (m_widgetArray.IsValidIndex(_widgetNum))
	{
		m_widgetNum = _widgetNum;
		m_widgetArray[m_widgetNum]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("New Widget Is Get Lost"));
	}
}

//ファイルの読み込み
FString UStageSubSystem::ReadFile(FString _fileDir)
{
	FString fileString = TEXT("");

	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	FString filePath = _fileDir + levelName + TEXT(".csv");

	if (!FPaths::FileExists(filePath))
	{
		UE_LOG(LogTemp, Log, TEXT("CSV File None : %s"), *filePath);

		// ディレクトリが存在しない場合
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_fileDir))
		{
			//ディレクトリの作成
			FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*_fileDir);
			UE_LOG(LogTemp, Warning, TEXT("Create Directiry : %s"), *_fileDir);
		}

		//空ファイルの作成
		FFileHelper::SaveStringToFile(TEXT(""), *filePath);
	}

	//読み取り
	if (FFileHelper::LoadFileToString(fileString, *filePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Read Result csvFile Is : %s"), *fileString)
		UE_LOG(LogTemp, Log, TEXT("Read csvFile path : %s"), *filePath)
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Read csvFile path : %s"), *filePath)
	}

	return fileString;
}

//ファイルの書き込み
void UStageSubSystem::WriteFile(FString _fileDir, FString _saveString)
{
	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	FString filePath = _fileDir + levelName + TEXT(".csv");

	if (!FPaths::FileExists(filePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("CSV File None : %s"), *filePath);

		// ディレクトリが存在しない場合
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_fileDir))
		{
			//ディレクトリの作成
			FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*_fileDir);
			UE_LOG(LogTemp, Log, TEXT("Create Directiry : %s"), *_fileDir);
		}

		//空ファイルの作成
		FFileHelper::SaveStringToFile(TEXT(""), *filePath);
	}

	if (FFileHelper::SaveStringToFile(_saveString, *filePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Saved Result csvFile path : %s"), *filePath);
		UE_LOG(LogTemp, Log, TEXT("Saved Result Data Is : %s"), *_saveString);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Save Result csvFile"));
	}
}

//ウィジェットを読み込み
void UStageSubSystem::LoadWedget()
{
	// WidgetBluePrintのクラスを取得
	TArray<FString> widgetPathArray =
	{
		"/Game/UI/ResultWidget_BP.ResultWidget_BP_C",
		"/Game/UI/ClearTimeRanking.ClearTimeRanking_C",
		"/Game/UI/PlayerStepRanking.PlayerStepRanking_C",
		"/Game/UI/CameraMoveRanking.CameraMoveRanking_C"
	};
		
	m_widgetArray.Init(nullptr, widgetPathArray.Num());

	for (int i = 0; i < widgetPathArray.Num(); ++i)
	{
		TSubclassOf<UUserWidget> widget = TSoftClassPtr<UUserWidget>(FSoftClassPath(*widgetPathArray[i])).LoadSynchronous();

		if (widget)
		{
			m_widgetArray[i] = CreateWidget<UUserWidget>(GetWorld(), *widget);
			m_widgetArray[i]->AddToViewport();
			m_widgetArray[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Result Widget Is None"));
		}
	}
}

