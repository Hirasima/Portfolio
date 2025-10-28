// MappingCamera3D
// 23cu0228 hanawa

#pragma once

#include "CoreMinimal.h"
#include "EnigmaGameInstance.h" // 追加
#include "GameManager.h"        // 追加
#include "GameFramework/Pawn.h"
#include "MappingCamera3D.generated.h"

UCLASS()
class AMappingCamera3D : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMappingCamera3D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//void SetIsPlayerMoving(bool _IsPlayerMoving);

	int GetDirectionNumber();

	bool GetCameraMoving();

	void SetCameraInput(FVector2D _Input);

	void SetIsPlayerMoving(bool _IsPlayerMoving);

private:
	void UpdateMappingCamera(float DeltaTime);

	FRotator m_CameraRotation;

	UPROPERTY(EditAnywhere, category = "Direction")
	int m_DirectionCamNumber;

	//イージング用
	
	//カメラの更新
	void UpdateCamera();

	//入力用
	void SetCameraRotateInputX();

	//カメラの回転するX方向の入力
	float m_inputRotationDireX;

	//カメラが回転している方向
	int m_rotationDireX;

	//カメラ移動中
	bool m_cameraMoving;

	//// プレイヤー移動中
	//bool m_IsPlayerMoving;

	//カメラ回転の目的地
	FRotator m_targetAngle;

	//イージング速度
	float m_lerpRatio;

	FVector2D m_CameraInput;

	bool m_IsPlayerMoving;

private:

	UWorld* world;
	UEnigmaGameInstance* gameinstance;
	UStageSubSystem* gamemanager;
};
