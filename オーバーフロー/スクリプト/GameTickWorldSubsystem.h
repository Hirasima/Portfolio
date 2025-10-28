// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameTickWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UGameTickWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	public:
	//状況による生成可否の定義
	virtual bool ShouldCreateSubsystem(UObject* Outer)const override;

	//ライフサイクル
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	//Tick
	virtual void Tick(float _deltaTime)override;

	//
	virtual TStatId GetStatId()const override;

};
