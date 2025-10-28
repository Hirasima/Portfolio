// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "InteractionSpawnTrriger.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API AInteractionSpawnTrriger : public AInteractionActor
{
	GENERATED_BODY()
	
public:
	//インタラクション時呼ばれる関数
	virtual void UseInteraction()override;
protected:
	virtual void BeginPlay()override;
};
