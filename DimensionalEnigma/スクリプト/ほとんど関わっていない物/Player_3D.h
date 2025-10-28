// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Player_3D.generated.h"

UCLASS()
class APlayer_3D : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayer_3D();

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
	class UInputAction* ActionInput;

	UPROPERTY(EditAnyWhere, Category = "Input")
	class UInputAction* BackInput;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Inputの設定
	void SetupInput();

	void PressedCamera(const FInputActionValue& Value);

	void PressedAction();

	void PressedStart();

	void PressedAxis(const FInputActionValue& Value);

	void PressedBack();

private:
	// 回転角度の設定
	void SetMovement(float angleDeg);

	void UpdateMove(float DeltaTime);

	void UpdatePlayerCollision();		// プレイヤーの移動可能判定処理

	void UpdateOtherActorCameraInfo(class AActor* CameraActor);		// 移動方向を示すカメラ情報を更新する処理

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
};
