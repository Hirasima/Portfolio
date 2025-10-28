// Player3D_Test.h
// 23cu0228 hanawa
// 23cu0221 tsuchiya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Player3D_Test.generated.h"

UCLASS()
class APlayer3D_Test : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayer3D_Test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void UpdateMove();					// プレイヤーの移動処理

	void UpdatePlayerCollision();		// プレイヤーの移動可能判定処理

	void UpdateOtherActorCameraInfo(class AActor* CameraActor);		// 移動方向を示すカメラ情報を更新する処理

	//void MoveForward(float Value);		// 入力バインド(移動：X軸)

	//void MoveRight(float Value);		// 入力バインド(移動：Y軸)

	// Inputの設定
	void SetupInput();					// Inputの初期設定

	void PressedCamera(const FInputActionValue& Value); // カメラの回転

	void PressedAction();				// 〇ボタンでのアクション

	void PressedStart();				// Startボタン

	void PressedAxis(const FInputActionValue& Value);	// スティックでの移動

	void PressedBack();					// ×ボタンで戻る

	void SetMovement(float angleDeg);	// 回転角度の設定

	void Move(float _DeltaTime);		// 仮の回転処理

	bool m_IsMove;						// 移動可能かどうか

	bool m_bWarp;						// 錯視移動可能か

	bool m_bCameraMove;					// カメラ移動中か

	bool m_IsMoving;					// 移動しているかどうか

	float ElapsedTime;					// 経過時間

	FVector m_ForwardDirection;			// 移動の前方向

	FVector m_RightDirection;			// 移動の横方向

	FVector m_RayVec;					// レイを飛ばす方向

	FVector2D m_MoveInput;				// スティックで入力された角度

	UPROPERTY(EditAnyWhere, Category = "Rotation")
	FVector m_MoveVector;// 移動量

	UPROPERTY(EditAnyWhere, Category = "Move")
	float m_MoveSpeed;		// 移動速度

	UPROPERTY(EditAnyWhere, Category = "Move")
	float m_RotationSpeed;	// 一秒で回転する速度

	UPROPERTY(EditAnyWhere, Category = "Rotation")
	FRotator m_rValue;		// 回転させる量（FRotator）

	UPROPERTY(EditAnyWhere, Category = "Rotation")
	float RotationAngle; // 回転する量(度指定)

	UPROPERTY(EditAnywhere, Category = "CameraActor")
	AActor* m_CameraActor;				// 移動方向を示すカメラ

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* MoveInput;		// 移動

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* StartInput;		// Startボタン

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* CameraInput;	// カメラ

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* ActionInput;	// 〇ボタン

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* BackInput;		// ×ボタン

};
