// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

//前方宣言
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ORIGINALACTION_API ACameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// オリジナルメソッド //////////////////////////////////////////////////////////////////////

public:



	// プロパティ //////////////////////////////////////////////////////////////////////

public:
	//スプリングアームルートコンポーネントからの初期位置
	UPROPERTY(EditAnywhere)
	FVector m_springArmOffset;

	//旋回速度
	UPROPERTY(EditAnywhere)
	float m_turningSpeed;

private:
	//カメラ
	UCameraComponent* m_pCameraComp;

	//スプリングアーム
	USpringArmComponent* m_pSpringArmComp;
};
