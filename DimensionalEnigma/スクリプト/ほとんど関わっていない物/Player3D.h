// Fill out your copyright notice in the Description page of Project Settings.

// 制作者：土谷祐太
// 三次元状態でのキューブを管理する


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WorldSelect.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnigmaGameInstance.h" // 追加
#include "GameManager.h"        // 追加
#include "StageSubSystem.h"
#include "Player3D.generated.h"

class UCameraComponent;//仮

UCLASS()
class APlayer3D : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayer3D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* MoveInput;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* StartInput;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* CameraInput; 

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* CameraInput_Y;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* ActionInput;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* BackInput;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* StickRInput_X;
	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* StickRInput_Y;

private:
	UPROPERTY(EditAnyWhere, Category = "SE_Path")
	FString MoveSE_Path = TEXT("/Game/Sound/HomeGrownSE_BlockMove.HomeGrownSE_BlockMove");//大竹追加　SEのパスを保存する変数

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsWarpable();

private:
	// Inputの設定
	void SetupInput();

	void PressedCamera(const FInputActionValue& Value);

	void PressedAction();

	void PressedStart();

	void PressedAxis(const FInputActionValue& Value);

	void PressedBack();

	void PressedStickR_X(const FInputActionValue& Value);
	void PressedStickR_Y(const FInputActionValue& Value);

	// 回転角度の設定
	void SetMovement(float angleDeg);

	//-------------------------------------------------大竹追加
private:
	float CalculateHeight(float Degrees);
	void UpdateGoalAction(float DeltaTime);

	void FreeCameraControl(float _DeltaTime);
	//void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void MoveRight(float Value);
	
	bool m_GoalFlag = false;//ゴールしているかのフラグ
	float m_SinkHeight = 10.0f;//ゴール時に地面に沈み込む高さ
	float m_SinkVel = 0.0f;//沈む速度
	float m_SinkAcc = 0.1f;//沈む加速度
	FVector m_BasePlayerPos;//基準座標
	float m_AddjustRotHeight;//回転に合わせて高さを上げる量

	AActor* m_DestinationActor;//移動先のアクター
	FVector m_DestinationPos;//移動先の座標

	FVector m_StickR_Value;//右Stickの入力量

	bool m_IsMuteSE = true;

	bool m_IsFriezeChecked = false;

	bool m_IsMoveDecision = true;

	UCameraComponent* CameraComponent;
public:
	void SetGoalFlag(bool _Flag);
	float GetFinHeight();
	bool GetIsMoving();
	FVector GetBasePlayerPos();
	//-------------------------------------------------

private:
	void UpdateMove(float DeltaTime);

	void UpdatePlayerCollision();		// プレイヤーの移動可能判定処理

	void UpdateOtherActorCameraInfo(class AActor* CameraActor);		// 移動方向を示すカメラ情報を更新する処理

	UStaticMeshComponent* MeshComponent;

	AWorldSelect* m_WorldSelect;

	bool m_IsMoving;		// 移動しているかどうか

	bool m_IsMove;

	bool m_bWarp;

	bool m_bDownWarp;

	bool m_bCameraMove;					// カメラ移動中か

	FVector m_ForwardDirection;			// 移動の前方向

	FVector m_RightDirection;			// 移動の横方向

	FVector m_TargetLocation;

	FVector m_DownWarpLocation;

	FVector m_StartLocation;

	float m_Alpha;			// 補間の進行度

	float ElapsedTime;		// 経過時間

	FVector m_RayVec;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	float m_MoveSpeed;		// 移動速度

	UPROPERTY(VisibleAnyWhere, Category = "Move")
	FVector m_MoveVector;// 移動量

	UPROPERTY(VisibleAnyWhere, Category = "Rotation")
	FRotator m_rValue;		// 回転させる量（FRotator）

	UPROPERTY(VisibleAnyWhere, Category = "Rotation")
	float RotationAngle; // 回転する量(度指定)

	UPROPERTY(VisibleAnyWhere, Category = "Rotation")
	float m_RotationSpeed;	// 一秒で回転する速度

	UPROPERTY(EditAnywhere, Category = "CameraActor")
	AActor* m_CameraActor;				// 移動方向を示すカメラ

	UPROPERTY(EditAnywhere, Category = "CameraActor")
	AActor* m_MainCamera;

	float m_CurrentTime;// 経過した時間

	FVector2D m_MoveInput; // 入力された角度

	private:

		UWorld* world;
		UEnigmaGameInstance* gameinstance;
		UStageSubSystem* gamemanager;
};