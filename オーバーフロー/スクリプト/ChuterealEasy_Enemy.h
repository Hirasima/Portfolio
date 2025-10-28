// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Easy_Enemy.h"
#include "ChuterealEasy_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API AChuterealEasy_Enemy : public AEasy_Enemy
{
	GENERATED_BODY()
	
protected:
//	//エネミーの状態更新関数*エネミーマネージャからのみ呼ぶこと*
//	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos)override;
//
//protected:
//	UPROPERTY(EditAnywhere, Category = "ChuterealManagerClassBP")
//	TSubclassOf<class AChuteRealManager> ChuterealManagerClass;
//	bool IsSendDead = false;
};
