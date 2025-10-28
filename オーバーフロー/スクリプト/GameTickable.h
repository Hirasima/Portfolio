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

//�N���X
class IGameTickable {

	GENERATED_BODY()

public:
	//���ۂ�Destroy���s����
	virtual void ExecuteDestroy(){}
	//Tick()�N
	virtual void GameTick(float _deltaTime){}
	//���̃t���[���̓���Destroy�����Ԃ�true�ɂ��邩�`�F�b�N
	virtual void CheckDestroy(){}
};
