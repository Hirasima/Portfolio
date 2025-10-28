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
	//�󋵂ɂ�鐶���ۂ̒�`
	virtual bool ShouldCreateSubsystem(UObject* Outer)const override;

	//���C�t�T�C�N��
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	//Tick
	virtual void Tick(float _deltaTime)override;

	//
	virtual TStatId GetStatId()const override;

};
