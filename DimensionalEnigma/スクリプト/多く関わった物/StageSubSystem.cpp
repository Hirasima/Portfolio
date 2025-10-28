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

//���������ɌĂяo�����֐�
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

	//�p�X
	FString fileDir = ("");
	//�ǂݎ��p�ϐ�
	FString fileString("");

	//�t�@�C���p�X
	fileDir = FPaths::ProjectContentDir() + TEXT("CSVFile/QuotaData/");

	//�t�@�C���ǂݍ���
	fileString = ReadFile(QUOTADATA_DIRE);

	//�m���}�f�[�^��ݒ�
	SetQuotaData(fileString);

	// ���[���h�̏�����������ɌĂ΂��C�x���g�Ƀo�C���h
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UStageSubSystem::OnWorldInitialized);

	UE_LOG(LogTemp, Warning, TEXT("Generated StageSubSystem"))
}


void UStageSubSystem::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
	//���x�����擾
	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	//���[���h�Z���N�g�łȂ����
	if (levelName != "WorldSelect")
	{
		//�A�C�e���E�B�W�F�b�g����
		GetWorld()->GetSubsystem<UItemManager>()->GenerateItemWidget();
	}
}

//�I�����ɌĂяo�����֐�
void UStageSubSystem::Deinitialize()
{
	for (int i = 0; i < m_widgetArray.Num(); ++i)
	{
		//�E�B�W�F�b�g�폜
		m_widgetArray[i]->RemoveFromParent();
	}

	Super::Deinitialize();

	UE_LOG(LogTemp, Warning, TEXT("Deleted StageSubSystem"))
}


//--------------------------------------------
//Tick�֐�
//--------------------------------------------

void UStageSubSystem::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Log, TEXT("Stage DeltaTime Is %f"), DeltaTime)

	//�o�ߎ��Ԃɉ��Z
	m_elapsedTime += DeltaTime;

	//�S�[�����Ă����
	if (m_goaling)
	{
		//���U���g��
		OnResult();
	}
}

//Tick�\���ǂ���
bool UStageSubSystem::IsTickable() const
{
	return true;
}

// Tickable�I�u�W�F�N�g��ID���擾����
TStatId UStageSubSystem::GetStatId()const
{
	//UE_LOG(LogTemp, Warning, TEXT("StageSubSystem :: GetStatId()"))
	RETURN_QUICK_DECLARE_CYCLE_STAT(UStageSubSystem, STATGROUP_Tickables);
}

//--------------------------------------------
//�I���W�i�����\�b�h
//--------------------------------------------

//�v���C���[���������񐔂����Z
void UStageSubSystem::AddPlayerMoveCount()
{
	//�S�[�����Ă��Ȃ����
	if (!m_goaling)
	{
		//�v���C���[�������Z
		++m_playerMoveCount;
		//�v���C���[�̏�ԍX�V
		SetPlayerMoving(false);
		UE_LOG(LogTemp, Log, TEXT("Add Player Move Count : %d"), m_playerMoveCount)
	}
}

//�v���C���[���������񐔂��擾
int UStageSubSystem::GetPlayerMoveCount()
{
	return m_playerMoveCount;
}

//�v���C���[�������Ă��邩�ݒ�
void UStageSubSystem::SetPlayerMoving(bool _status)
{
	m_playerMoving = _status;
}

//�v���C���[�������Ă��邩�擾
bool UStageSubSystem::GetPlayerMoving()
{
	return m_playerMoving;
}

//�J�������������񐔂����Z
void UStageSubSystem::AddCameraMoveCount()
{
	//�S�[�����Ă��Ȃ����
	if (!m_goaling)
	{
		//�J�����̉�]�񐔉��Z
		++m_cameraMoveCount;
		SetCameraMoving(false);
		UE_LOG(LogTemp, Log, TEXT("Add Camera Move Count : %d"), m_cameraMoveCount)
	}
}

//�v���C���[���������񐔂��擾
int UStageSubSystem::GetCameraMoveCount()
{
	return m_cameraMoveCount;
}

//�J�����������Ă��邩�ݒ�
void UStageSubSystem::SetCameraMoving(bool _status)
{
	m_cameraMpving = _status;
}

//�J�����������Ă��邩�擾
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

//�X�e�[�W�̌o�ߎ��Ԃ��擾
float UStageSubSystem::GetStageElapsedTime()
{
	return m_elapsedTime;
}

//�N���A�^�C���̎擾
float UStageSubSystem::GetClearTime()
{
	if (!m_goaling)
	{
		m_clearTime = m_elapsedTime;
	}

	return m_clearTime;
}

//�����N�̃e�N�X�`�����擾
FString UStageSubSystem::GetRankTexturePath()
{
	//�e�N�X�`���̃p�X
	TArray<FString> texturePath =
	{
		"/Game/Texture/UI_Rank_C.UI_Rank_C",
		"/Game/Texture/UI_Rank_B.UI_Rank_B",
		"/Game/Texture/UI_Rank_A.UI_Rank_A",
		"/Game/Texture/UI_Rank_S.UI_Rank_S"
	};

	//�ǂݍ��ރp�X�ԍ�
	int pathNum = 0;

	//�N���A�^�C�����m���}�ȉ��Ȃ�
	if (m_clearTime <= m_quotaData.clearTime)
	{
		++pathNum;
	}
	//�v���C���[�������m���}�ȉ��Ȃ�
	if (m_playerMoveCount <= m_quotaData.playerMoveCount)
	{
		++pathNum;
	}
	//�J������]�񐔂��m���}�ȉ��Ȃ�
	if (m_cameraMoveCount <= m_quotaData.cameraMoveCount)
	{
		++pathNum;
	}

	//UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *texturePath[pathNum]);

	return texturePath[pathNum];
}

TArray<FString> UStageSubSystem::GetClearTimeRanking()
{
	//�N���A�^�C���f�[�^
	TArray<FString> timeArray;
	//������
	timeArray.Init(FString("No Data"), 5);

	//���5�ʂ܂�
	for (int i = 0; i < 5; ++i)
	{
		//�f�[�^���L���Ȃ�
		if (m_clearTimeRanking.IsValidIndex(i))
		{
			//�f�[�^��ǂݍ���
			timeArray[i] = FString::SanitizeFloat(m_resultDataArray[m_clearTimeRanking[i]].clearTime);
		}
	}

	return timeArray;
}

TArray<FString> UStageSubSystem::GetPlayerMoveCountRanking()
{
	//�����f�[�^
	TArray<FString> stepArray;
	stepArray.SetNum(5);

	//���5�ʂ܂�
	for (int i = 0; i < 5; ++i)
	{
		//�f�[�^���L���Ȃ�
		if (m_playerMoveRanking.IsValidIndex(i))
		{
			//�f�[�^��ǂݍ���
			stepArray[i] = FString::FromInt(m_resultDataArray[m_playerMoveRanking[i]].playerMoveCount);
		}
		//�L���łȂ��Ȃ�
		else
		{
			stepArray[i] = FString("No Data");
		}
	}

	return stepArray;
}

TArray<FString> UStageSubSystem::GetCameraMoveCountRanking()
{
	//�J������]�񐔃f�[�^
	TArray<FString> cameraArray;

	cameraArray.SetNum(5);

	//���5�ʂ܂�
	for (int i = 0; i < 5; ++i)
	{
		//�f�[�^���L���Ȃ�
		if (m_cameraMoveRanking.IsValidIndex(i))
		{
			//�f�[�^��ǂݍ���
			cameraArray[i] = FString::FromInt(m_resultDataArray[m_cameraMoveRanking[i]].cameraMoveCount);
		}
		//�L���łȂ��Ȃ�
		else
		{
			cameraArray[i] = FString("No Data");
		}
	}

	return cameraArray;
}

//�S�[�����Ă��邩���擾
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

//�S�[���������ɌĂ΂��
void UStageSubSystem::OnGoal(FName& _nextLevelName)
{
	//�S�[���ς݂ɕύX
	m_goaling = true;
	//���̃��x���̎擾
	m_nextLevelName = _nextLevelName;
	//�N���A�^�C���Ɍo�ߎ��Ԃ���
	m_clearTime = m_elapsedTime;

	m_widgetNum = -1;

	//�p�X
	FString fileDir = FPaths::ProjectContentDir() + TEXT("CSVFile/RankingData/");
	//�ǂݎ��p�ϐ�
	FString fileString("");
	//�t�@�C���ǂݍ���
	fileString = ReadFile(RANKINGDATA_DIRE);
	//���U���g�f�[�^�ݒ�
	SetResultSaveData(fileString);

	//����̃��U���g�f�[�^��ǉ�
	ResultData resultData(m_clearTime, m_playerMoveCount, m_cameraMoveCount);
	m_resultDataArray.Add(resultData);
	//���U���g�f�[�^���Z�[�u
	SaveResultSaveData();
	//�����L���O�f�[�^�ݒ�
	SetRankingData();
	//�E�B�W�F�b�g�ǂݍ���
	LoadWedget();
	//�A�C�e���E�B�W�F�b�g�폜
	GetWorld()->GetSubsystem<UItemManager>()->RemoveItemWidget();
}

//���U���g���ɌĂ΂��
void UStageSubSystem::OnResult()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resulting"))

	//�N���A���Ԃ����莞�Ԍo�����疳��
	if (m_elapsedTime - m_clearTime < 1.5f) return;
	//��莞�ԂȂ��Ȃ�
	else
	{
		//���x���̎擾
		UWorld* world = GetWorld();
		//�k���`�F�b�N
		if (!world) return;

		//�v���C���[�R���g���[���[�擾
		APlayerController* playerController = world->GetFirstPlayerController();
		//�k���`�F�b�N
		if (!playerController) return;

		if (playerController->WasInputKeyJustPressed(EKeys::Enter)
		|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
		{
			//���̃��x����
			UGameplayStatics::OpenLevel(world, m_nextLevelName);
		}

		//�����L���O�\����
		if (m_widgetNum == 0)
		{
			if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
			{
				//�E�B�W�F�b�g�ύX
				ChangeWidget(1);
			}
		}
		else if (m_widgetNum == 1)
		{
			if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
			{
				//�E�B�W�F�b�g�ύX
				ChangeWidget(0);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Right)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_RightShoulder))
			{
				//�E�B�W�F�b�g�ύX
				ChangeWidget(2);
			}
			else if (playerController->WasInputKeyJustPressed(EKeys::Left)
			|| playerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftShoulder))
			{
				//�E�B�W�F�b�g�ύX
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
	//���U���g�f�[�^
	TArray<FString> resultDataArray;

	//�����񂲂Ƃɕ���
	_string.ParseIntoArrayLines(resultDataArray);

	//������
	m_resultDataArray.Init(ResultData(), resultDataArray.Num());

	//�f�[�^�����J��Ԃ�
	for (int i = 0; i < resultDataArray.Num(); ++i)
	{
		//���f�[�^�ۑ��p
		TArray<FString> resultData;
		//","�ŕ�����
		resultDataArray[i].ParseIntoArray(resultData, TEXT(","));

		//�f�[�^��3�ȏ�Ȃ�
		if (resultData.Num() >= 3)
		{
			//�f�[�^��ǂݍ���
			ResultData data(FCString::Atof(*resultData[0]), FCString::Atoi(*resultData[1]), FCString::Atoi(*resultData[2]));
			m_resultDataArray[i] = data;
		}
	}
}

//�m���}�f�[�^��ݒ�
void UStageSubSystem::SetQuotaData(FString _string)
{
	//�m���}�f�[�^
	TArray<FString> quotaArray;

	//�s�f�[�^
	TArray<FString> lineArray;

	//�s���Ƃɕ���
	_string.ParseIntoArrayLines(lineArray);

	for (int i = 0; i < lineArray.Num(); ++i)
	{
		//���̃f�[�^
		TArray<FString> dotArray;

		//","�ŕ�����
		lineArray[i].ParseIntoArray(dotArray, TEXT(","));

		//����
		quotaArray += dotArray;
	}
	//�f�[�^��3�ȏ�Ȃ�
	if (quotaArray.Num() >= 3)
	{
		//�m���}�f�[�^�Z�b�g
		m_quotaData = ResultData(FCString::Atof(*quotaArray[0]), FCString::Atoi(*quotaArray[1]), FCString::Atoi(*quotaArray[2]));
		UE_LOG(LogTemp, Log, TEXT("Set Quota Data : %f %d %d"), m_quotaData.clearTime, m_quotaData.playerMoveCount, m_quotaData.cameraMoveCount);
	}
	else
	{
		//��f�[�^�Z�b�g
		m_quotaData = ResultData(0.0f, 0, 0);
	}
}

//���U���g�f�[�^���Z�[�u
void UStageSubSystem::SaveResultSaveData()
{
	//�Z�[�u����f�[�^
	FString saveData = TEXT("");

	UE_LOG(LogTemp, Log, TEXT("Saveing Result Data"))

	for (int i = 0; i < m_resultDataArray.Num(); ++i)
	{
		//���U���g�f�[�^
		FString resultData = "";
		resultData.Append(FString::SanitizeFloat(m_resultDataArray[i].clearTime) + TEXT(","));
		resultData.Append(FString::FromInt(m_resultDataArray[i].playerMoveCount) + TEXT(","));
		resultData.Append(FString::FromInt(m_resultDataArray[i].cameraMoveCount) + TEXT("\n"));

		UE_LOG(LogTemp, Log, TEXT("%s"), *resultData)

		//�Z�[�u�f�[�^�ɒǉ�
		saveData.Append(resultData);
	}

	//�t�@�C���p�X
	FString filePath = FPaths::ProjectContentDir() + TEXT("CSVFile/RankingData/");

	//�Z�[�u
	WriteFile(RANKINGDATA_DIRE, saveData);
}

//�����L���O�f�[�^��ݒ�
void UStageSubSystem::SetRankingData()
{
	//�L���`�F�b�N
	if (m_resultDataArray.IsEmpty())
	{
		return;
	}

	//�����L���O�f�[�^
	TArray<float> timeRanking;
	TArray<int> stepRanking;
	TArray<int> angleRankig;
	
	//�f�[�^���d����
	for (int i = 0; i < m_resultDataArray.Num(); ++i)
	{
		timeRanking.Add(m_resultDataArray[i].clearTime);
		stepRanking.Add(m_resultDataArray[i].playerMoveCount);
		angleRankig.Add(m_resultDataArray[i].cameraMoveCount);
	}

	//�����L���O���ʕ��J��Ԃ�
	for (int i = 0; i < 5; ++i)
	{
		//----------------------------------------------------
		//�N���A�^�C��
		//----------------------------------------------------

		int leastNum = GetLeastFloatNum(timeRanking);
		if (timeRanking[leastNum] != MAX_flt)
		{
			m_clearTimeRanking.Add(leastNum);
			timeRanking[leastNum] = MAX_flt;
		}

		//----------------------------------------------------
		//�����J�E���^�[
		//----------------------------------------------------

		leastNum = GetLeastIntNum(stepRanking);
		if (stepRanking[leastNum] != INT_MAX)
		{
			m_playerMoveRanking.Add(leastNum);
			stepRanking[leastNum] = INT_MAX;
		}

		//----------------------------------------------------
		//�J������]�J�E���^�[
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
	//��ԏ������f�[�^�̗v�f��
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

//�ł��������f�[�^��Ԃ�
int UStageSubSystem::GetLeastIntNum(TArray<int> _array)
{
	//��ԏ������f�[�^�̗v�f��
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

//�E�B�W�F�b�g��؂�ւ���
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

//�t�@�C���̓ǂݍ���
FString UStageSubSystem::ReadFile(FString _fileDir)
{
	FString fileString = TEXT("");

	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	FString filePath = _fileDir + levelName + TEXT(".csv");

	if (!FPaths::FileExists(filePath))
	{
		UE_LOG(LogTemp, Log, TEXT("CSV File None : %s"), *filePath);

		// �f�B���N�g�������݂��Ȃ��ꍇ
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_fileDir))
		{
			//�f�B���N�g���̍쐬
			FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*_fileDir);
			UE_LOG(LogTemp, Warning, TEXT("Create Directiry : %s"), *_fileDir);
		}

		//��t�@�C���̍쐬
		FFileHelper::SaveStringToFile(TEXT(""), *filePath);
	}

	//�ǂݎ��
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

//�t�@�C���̏�������
void UStageSubSystem::WriteFile(FString _fileDir, FString _saveString)
{
	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	FString filePath = _fileDir + levelName + TEXT(".csv");

	if (!FPaths::FileExists(filePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("CSV File None : %s"), *filePath);

		// �f�B���N�g�������݂��Ȃ��ꍇ
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_fileDir))
		{
			//�f�B���N�g���̍쐬
			FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*_fileDir);
			UE_LOG(LogTemp, Log, TEXT("Create Directiry : %s"), *_fileDir);
		}

		//��t�@�C���̍쐬
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

//�E�B�W�F�b�g��ǂݍ���
void UStageSubSystem::LoadWedget()
{
	// WidgetBluePrint�̃N���X���擾
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

