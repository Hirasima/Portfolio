// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameTickable.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class UGameTickable : public UInterface
{
	GENERATED_BODY()
	
};

//クラス
class IGameTickable {

	GENERATED_BODY()

public:
	//実際のDestroy実行処理
	virtual void ExecuteDestroy(){}
	//Tick()君
	virtual void GameTick(float _deltaTime){}
	//次のフレームの頭にDestroyする状態をtrueにするかチェック
	virtual void CheckDestroy(){}
};
