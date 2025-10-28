#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "camera2D.generated.h"

UCLASS()
class Acamera2D
	: public APawn
{
	GENERATED_BODY()

public:
	//コンストラクタ
	Acamera2D();
	//コンストラクタ
	~Acamera2D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//カメラ関係
	//
	//管理しているカメラオブジェクトを取得
	UCameraComponent* GetCamera() const { return m_pCamera; }

	//管理するカメラオブジェクトを設定
	void SetCamera(UCameraComponent* _pCamera) { m_pCamera = _pCamera; }

	//カメラの座標を設定
	void SetCameraPos(float _x, float _y, float _z);

	//カメラの角度を設定
	void SetCameraRot(float _pitch, float _yaw, float _roll);

	void SetCameraDefaultCondition();

	//カメラの座標を取得
	FVector3d GetCameraPos();

	//カメラの角度を取得
	FRotator GetCameraRot();

	//プレイヤー関係
	void UpdateCamera();

	void UpdatePawn();

private:
	//メンバ変数
	UCameraComponent* m_pCamera;//カメラ

	//入力バインド
	void Cam_RotatePitch(float _axisValue);
	void Cam_RotateYaw(float _axisValue);

	void Pawn_MoveX(float _axisValue);
	void Pawn_MoveZ(float _axisValue);
	//メンバ変数
	//CCameraManager m_cameraMng;
	FVector2D m_cameraRotateInput;
	FVector2D m_pawnMoveInput;

	void InitializeCamera();

	float TestNum;

	float CameraRot_Pitch;
	float CameraRot_Yaw;

	float Rad;
};
