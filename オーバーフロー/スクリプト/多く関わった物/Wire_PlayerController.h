// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Wire_PlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS(Abstract)
class ORIGINALACTION_API AWire_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWire_PlayerController();

	void BeginPlay()override;
	
	//プロパティ//////////////////////////////////
public:

	//プレイヤーカメラ管理クラス
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerCameraManager> m_PlayerCameraManagerClass;

	//マッピングコンテキスト
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* DefaultInputMappingContext;
};
