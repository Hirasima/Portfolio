// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ItemGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UItemGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "My Variables")
	int32 MyVariable;
	TArray<bool> ItemFlag;

	UItemGameInstance();
};
