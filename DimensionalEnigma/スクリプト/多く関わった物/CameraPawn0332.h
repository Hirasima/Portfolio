// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "SaveableObject.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnigmaGameInstance.h"
#include "StageSubSystem.h"
#include "CameraPawn0332.generated.h"

//仮
//class UTimelineComponent;

UCLASS()
class MYPROJECT_API ACameraPawn0332 : public APawn, public ISaveableObject
{
	GENERATED_BODY()

public:
	//コンストラクタ
	ACameraPawn0332();

protected:
	//開始フレーム処理
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* CameraInput;

public:
	//毎フレーム処理
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	//管理しているカメラを取得
	UCameraComponent* GetCamera() const;

	//管理しているスプリングアームを取得
	USpringArmComponent* GetSpringArm() const;

	//管理するカメラを設定
	void SetCamera(UCameraComponent* _pCamera);

	//管理するスプリングアームを設定
	void SetSpringArm(USpringArmComponent* _pSpringArm);

	//カメラの初期設定
	void InitCamera();

	//カメラの更新
	void UpdateCamera(float _DeltaTime);

	//スプリングアームの長さを設定
	void SetArmLength(float _length);

	//カメラの角度を設定
	void SetCameraAngle(float _pitch, float _yaw, float _roll);

	//カメラの角度に加算
	void AddCameraAngle(float _pitch, float _yaw, float _roll);

	// Inputの設定
	void SetupInput();

	void PressedCamera(const FInputActionValue& Value);

	void SetCameraInput(FVector2D _Input);

	void SetCameraRotateInputX();

	void SetIsPlayerMoving(bool _IsPlayerMoving);

	void SetCameraAngle(float _Roll);//大竹追加
	void SetTargetCameraRoll();//大竹追加
	void SetCameraMode(bool mode);
	FRotator GetFreeCameraRot();
	void SetFreeCameraRot(FRotator _Rot);

private:
	//メソッド

	//入力用

public:
	//String型のセーブデータを取得
	virtual FString GetSaveDataToString();
	//String型のセーブデータを設定
	virtual void SetSaveDataToString(const FString _saveData);

private:
	//プロパティ
	UPROPERTY(EditAnywhere)
	float SpringArmLength;					//スプリングアームの長さ

	UPROPERTY(EditAnywhere)
	UCameraComponent* m_pCamera;		//カメラ
	UPROPERTY(EditAnywhere)
	USpringArmComponent* m_pSpringArm;  //スプリングアーム

private:

	//カメラの回転するX方向の入力
	float m_inputRotationDireX;

	//カメラが回転している方向
	int m_rotationDireX;

	//カメラ移動中
	bool m_cameraMoving;

	//カメラ回転の目的地
	FRotator m_targetAngle;

	//イージング速度
	float m_lerpRatio;

	FVector2D m_CameraInput;

	bool m_IsPlayerMoving;

	float DefaultCameraPitch;			//大竹追加
	float CameraAngleUpValue = 30.0f;	//大竹追加　カメラを上げる量
	float TargetCameraRoll = 0.0f;		//大竹追加
	float m_RotTimer = 0.0f;			//大竹追加　回転時間計測
	FString m_CameraRotSE_Path = TEXT("/Game/Sound/RotateCameraSE02.RotateCameraSE02");			//大竹追加　カメラ回転SEのパス

	FRotator m_FreeCameraRot;
	FRotator m_FreeCameraRot_temp;
	FRotator m_BaseCameraRot;
	
	bool m_CameraMode = true;//false透視投影 true平行投影


private:

	UWorld* world;
	UEnigmaGameInstance* gameinstance;
	UGameManager* gamemanagaer;
	UStageSubSystem* stage;

};
