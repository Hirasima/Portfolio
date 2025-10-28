// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuideCharacter.generated.h"

//攻撃起動通知ステート
UENUM(BlueprintType)
enum class EGuideAnimState : uint8
{
	None		UMETA(DisplayName = "None"),
	Wait		UMETA(DisplayName = "Wait"),
	Dash		UMETA(DisplayName = "Dash"),
	Hug			UMETA(DisplayName = "Hug")
};

//攻撃起動通知ステート
UENUM(BlueprintType)
enum class EAutoMoveState : uint8
{
	None		UMETA(DisplayName = "None"),
	Auto		UMETA(DisplayName = "Auto"),
	Trace		UMETA(DisplayName = "Trace")
};

//ドアと移動ご座標を管理する構造体
USTRUCT(BlueprintType)
struct FGuideCharacterLogic
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAutoMoveState MoveLogic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector TrrigerPos;
};


UCLASS()
class ORIGINALACTION_API AGuideCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuideCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//現在のアニメーションステート取得用関数
	UFUNCTION(BlueprintCallable)
	EGuideAnimState GetGuideAnimState();

	void BeChild();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovePattern")
	TArray<FGuideCharacterLogic> m_Routes;
protected:
	//現在のアニメーションステート
	EGuideAnimState m_AnimState;

	//現在の移動原理
	EAutoMoveState m_ControlleState;

	//現在の移動番号
	int m_nowRouteNum;
};
