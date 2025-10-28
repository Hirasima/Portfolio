// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPawn0332.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加
#include "SoundManagerSubsystem.h"
#include "StageSubSystem.h"

// Sets default values
ACameraPawn0332::ACameraPawn0332()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//カメラの初期化
	InitCamera();

	// 初期設定
	bool bIsOrthographic = false;
	GetCamera()->SetProjectionMode(ECameraProjectionMode::Perspective);
}

// Called when the game starts or when spawned
void ACameraPawn0332::BeginPlay()
{
	Super::BeginPlay();

	//カメラの初期角度設定
	SetCameraAngle(-35.26f, 315.0f, 0.0f);
	//位置を初期化
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));

	DefaultCameraPitch = GetActorRotation().Pitch;//初期化　大竹追加

	m_lerpRatio = 0.7f;
	//スプリングアームの長さ設定
	if (SpringArmLength <= 0.0f)
	{
		SpringArmLength = 1000.0f;
	}
	m_pSpringArm->TargetArmLength = SpringArmLength;

	// GameManagerを取得
	world = GetWorld();
	if (world) {
		gameinstance = Cast<UEnigmaGameInstance>(world->GetGameInstance());
		if (gameinstance) {
			gamemanagaer = gameinstance->GetSubsystem<UGameManager>();
		}
		stage = world->GetSubsystem<UStageSubSystem>();
	}

	//正投影にする
	GetCamera()-> SetProjectionMode(ECameraProjectionMode::Orthographic);

	//-------------------------大竹追加
	//SEを再生(無音)空撃ちさせる
	USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	if (SoundManager)
	{
		// パスで音源を再生
		SoundManager->PlaySoundFromPath(m_CameraRotSE_Path, 0.001f);

		UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
	}
	//-------------------------
}

// Called every frame
void ACameraPawn0332::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetCameraRotateInputX();

	UpdateCamera(DeltaTime);
	
}

// Called to bind functionality to input
void ACameraPawn0332::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//管理しているカメラを取得
UCameraComponent* ACameraPawn0332::GetCamera() const
{
	return m_pCamera;
}

//管理しているスプリングアームを取得
USpringArmComponent* ACameraPawn0332::GetSpringArm() const
{
	return m_pSpringArm;
}

//管理するカメラを設定
void ACameraPawn0332::SetCamera(UCameraComponent* _pCamera)
{
	m_pCamera = _pCamera;
}

//管理するスプリングアームを設定
void ACameraPawn0332::SetSpringArm(USpringArmComponent* _pSpringArm)
{
	m_pSpringArm = _pSpringArm;
}

//カメラの初期化
void ACameraPawn0332::InitCamera()
{
	if (!m_pSpringArm)
	{
		//スプリングアーム生成
		m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>("m_pSpringArm");
		m_pSpringArm->SetupAttachment(RootComponent);

		//スプリングアームの長さ設定
		m_pSpringArm->TargetArmLength = SpringArmLength;
		m_pSpringArm->bDoCollisionTest = false;
	}
	
	if (!m_pCamera)
	{
		//カメラ生成
		m_pCamera = CreateDefaultSubobject<UCameraComponent>("m_pCamera");
		m_pCamera->SetupAttachment(m_pSpringArm);
		m_pCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	}
}

//カメラの更新
void ACameraPawn0332::UpdateCamera(float _DeltaTime)
{

	//移動中出ないなら
	if (!m_cameraMoving)
	{
		//入力されている時
		if (m_inputRotationDireX != 0 && !stage->GetGoaling())
		{
			//回転方向を入力されている方向にする
			m_rotationDireX = m_inputRotationDireX;
			//目的地の設定
			m_targetAngle = GetActorRotation() + FRotator(0.0f, 90.0f * m_rotationDireX, 0.0f);
			//Lerpに使う割合が異常値なら補正
			if (m_lerpRatio <= 0 || m_lerpRatio > 1)
				m_lerpRatio = 0.06f;

			//UE_LOG(LogTemp, Log, TEXT("Start Move Target Yaw : %f"), m_targetAngle.Yaw);

			//移動中に変更
			m_cameraMoving = true;

			if (stage)
			{
				stage->SetCameraMoving(true);
			}
			//UE_LOG(LogTemp, Log, TEXT("m_cameraMoving Is %d"), (int)m_cameraMoving);
		    
			//-------------------------大竹追加
			USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
			if (SoundManager)
			{
				// パスで音源を再生
				SoundManager->PlaySoundFromPath(m_CameraRotSE_Path);

				UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
			}
			//
		}
		else
		{
			//フリーカメラ

			SetActorRotation(GetActorRotation() - m_FreeCameraRot_temp);//加算前の状態に戻す

			m_BaseCameraRot = GetActorRotation();//基本座標として記録しておく

			SetActorRotation(m_BaseCameraRot + m_FreeCameraRot);

			m_FreeCameraRot_temp = m_FreeCameraRot;
		}
	}
	else
	{
		//現在の回転量
		FRotator r1 = GetActorRotation();
		//移動後の予定している回転量
		FRotator r2 = FMath::Lerp(r1, m_targetAngle, m_lerpRatio * (_DeltaTime * 20.0f));
																		//↑大竹　異なるFPS禍でも同一の速度を維持するように
		//回転させる
		SetActorRotation(r2);

		//回転時間計測
		m_RotTimer += _DeltaTime;

		//目的地にとても近くなった又は移動量が小さくなりすぎたら  
		if (FMath::Abs(m_targetAngle.Yaw - r1.Yaw) <= 0.01f || FMath::Abs(r2.Yaw - r1.Yaw) < 0.01f ||//大竹　終端に違和感が生じるため更に範囲を細かく
			m_RotTimer > 0.5f || r1 == GetActorRotation())											 //大竹追加→更に一定時間経過したら 更に回転量が0であるならば
		{
			//目的地に移動
			SetActorRotation(m_targetAngle);

			//値を初期化
			m_rotationDireX = 0;
			m_cameraMoving = false;
			m_CameraInput.X = 0.f;
			m_RotTimer = 0.0f;//大竹追加

			// カメラの回転回数をManagerにカウントさせる
			if (stage)
			{
				stage->AddCameraMoveCount();
			}

			//UE_LOG(LogTemp, Log, TEXT("m_cameraMoving Is %d"), (int)m_cameraMoving);
		}
	}
}

//スプリングアームの長さを設定
void ACameraPawn0332::SetArmLength(float _length)
{
	if (m_pSpringArm != nullptr)
	{
		m_pSpringArm->TargetArmLength = _length;
	}
}

//カメラの角度を設定
void ACameraPawn0332::SetCameraAngle(float _pitch, float _yaw, float _roll)
{
	SetActorRotation(FRotator(_pitch, _yaw, _roll));
	//UE_LOG(LogTemp, Log, TEXT("Set Camera Rotation"));
	/*FTransform t = GetActorTransform();
	FRotator r = t.Rotator();*/
	/*UE_LOG(LogTemp, Log, TEXT("Set Camera Rotation : p %d y %d r %d"), r.Pitch, r.Yaw, r.Roll);*/
}

//カメラの角度に加算
void ACameraPawn0332::AddCameraAngle(float _pitch, float _yaw, float _roll)
{
	AddActorWorldRotation(FRotator(_pitch, _yaw, _roll));
}

//入力値を代入
void ACameraPawn0332::SetCameraRotateInputX()
{
	if (m_CameraInput.X == 1.f)
	{
		m_inputRotationDireX = -1;
		//UE_LOG(LogTemp, Log, TEXT("Input Right"));
	}
	else if (m_CameraInput.X == -1.f)
	{
		m_inputRotationDireX =  1;
		//UE_LOG(LogTemp, Log, TEXT("Input Left"));
	}
	else
	{
		m_inputRotationDireX = 0;
		//UE_LOG(LogTemp, Log, TEXT("Input None"));
	}
}

//String型のセーブデータを取得
FString ACameraPawn0332::GetSaveDataToString()
{
	return FString("");
}
//String型のセーブデータを設定
void ACameraPawn0332::SetSaveDataToString(const FString _saveData)
{

}

void ACameraPawn0332::SetupInput()
{

}

void ACameraPawn0332::PressedCamera(const FInputActionValue& Value)
{

}

void ACameraPawn0332::SetCameraInput(FVector2D _Input)
{
	m_CameraInput = _Input;
}

void ACameraPawn0332::SetIsPlayerMoving(bool _IsPlayerMoving)
{
	m_IsPlayerMoving = _IsPlayerMoving;
}

//-------------------------------大竹が追加した関数
void ACameraPawn0332::SetCameraAngle(float _Roll)
{
	FRotator CameraRot = GetActorRotation();
	CameraRot.Pitch = -_Roll+ DefaultCameraPitch;
	SetActorRotation(CameraRot);
}
void ACameraPawn0332::SetTargetCameraRoll()
{

}

//仮実装
void ACameraPawn0332::SetCameraMode(bool mode)
{
	if (mode)
	{
		GetCamera()->SetProjectionMode(ECameraProjectionMode::Perspective);
		m_CameraMode = true;

		//スプリングアームの長さ設定
		SpringArmLength = 300.0f;
		m_pSpringArm->TargetArmLength = SpringArmLength;
	}
	else
	{
		GetCamera()->SetProjectionMode(ECameraProjectionMode::Orthographic);
		m_CameraMode = false;

		//スプリングアームの長さ設定
		SpringArmLength = 1000.0f;
		m_pSpringArm->TargetArmLength = SpringArmLength;
	}


}


void ACameraPawn0332::SetFreeCameraRot(FRotator _Rot)
{
	m_FreeCameraRot = _Rot;
}
FRotator ACameraPawn0332::GetFreeCameraRot()
{
	return m_FreeCameraRot;
}

//-------------------------------