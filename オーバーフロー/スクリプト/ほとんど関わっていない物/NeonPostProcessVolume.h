// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PostProcessVolume.h"
#include "NeonPostProcessVolume.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API ANeonPostProcessVolume : public APostProcessVolume
{
	GENERATED_BODY()
	
	//コンストラクタ
	ANeonPostProcessVolume();

	//初期
	virtual void BeginPlay()override;
};
