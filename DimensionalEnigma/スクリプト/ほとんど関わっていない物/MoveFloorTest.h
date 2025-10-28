// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnigmaGameInstance.h"
#include "GameFramework/Actor.h"
#include "MoveFloorTest.generated.h"

UENUM()
enum class ESwitchType : uint8
{
	ManyTimes = 0,
	Once = 1,
};

UENUM()
enum class EMovingType : uint8
{
	Movement = 0,
	Rotation = 1,
	Scale = 2,
	ThreeD = 3,
	TwoD = 4,
};

UENUM()
enum class EEasingType : uint8
{
	Linear = 0,
	EaseOut = 1,
	EaseIn = 2,
	EaseInOut = 3,
};
UCLASS()

class AMoveFloorTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveFloorTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	UStaticMeshComponent* SwitchComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	UStaticMeshComponent* BoxComponent;

private:
	// プレイヤーが乗っているかチェックする関数
	void UpdateCheckPlayer();
	
	// スイッチが作動した際呼ばれる関数
	void UpdateMoveBlock(float DeltaTime);

	void SetMaterial();

	// 移動速度を設定する関数
	FVector SetMoveSpeed(EEasingType _EasingType, FVector& _Start, FVector& _End, float _Alpha, float _MoveEndTime);

	// 回転速度を設定する関数
	FRotator SetRotationSpeed(EEasingType _EasingType, FRotator& _Start, FRotator& _End, float _Alpha, float _MoveEndTime);

	// 拡縮速度を設定する関数
	FVector SetScaleSpeed(EEasingType _EasingType, FVector& _Start, FVector& _End, float _Alpha, float _MoveEndTime);

	// イージング処理を設定する関数
	float EasingType(EEasingType _EasingNum, float _Alpha, float _MoveEndTime);

	UPROPERTY(EditAnywhere, Category = "Movement")
	AActor* m_MoveBlock;

	UPROPERTY(EditAnywhere, Category = "Movement")
	EMovingType m_MovingType;

	UPROPERTY(EditAnywhere, Category = "Movement")
	ESwitchType m_SwitchType;

	UPROPERTY(EditAnywhere, Category = "MoveFloor")
	TArray<FVector> m_MoveDistances;

	UPROPERTY(EditAnywhere, Category = "RotateFloor")
	FRotator m_RotationDistance;

	UPROPERTY(EditAnywhere, Category = "ScaleFloor")
	FVector m_ScaleSize;

	UPROPERTY(EditAnywhere, Category = "MoveSpeedType")
	EEasingType m_EasingType;

	UPROPERTY(EditAnywhere, Category = "MoveSpeedType")
	float m_MoveEndTime;

	UPROPERTY(EditAnywhere, Category = "Dimension")
	AActor* m_MainCamera;

	UPROPERTY(EditAnywhere, Category = "Dimension")
	AActor* m_PlayerControlCamera;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	FString SE_Path = TEXT("/Game/Sound/SwitchSE.SwitchSE");

	UMaterialInstanceDynamic* m_DefaultSwitchColor;

	FVector m_MoveDistance;
	
	FVector m_StartOtherObjLocation;

	FRotator m_StartOtherObjRotation;

	FVector m_StartOtherObjScale;

	FVector m_ObjLocation;

	FRotator m_ObjRotation;

	FVector m_ObjScale;

	FRotator m_Default3DRotation;

	FRotator m_Default2DRotation;

	FRotator m_CamRotationSpeed;

	float m_MoveCurrentTime;

	float m_SwitchCoolTime;

	float m_Alpha;

	int m_MoveDistanceIdx;

	// スイッチが押されているかどうか
	bool m_isSwitch;

	// ブロックが移動するかどうか
	bool m_isMoving;

	bool m_isMoveReturn;

	bool m_isOnce;

	private:
		UWorld* world;
		UEnigmaGameInstance* gameinstance;
		UStageSubSystem* gamemanager;
};
