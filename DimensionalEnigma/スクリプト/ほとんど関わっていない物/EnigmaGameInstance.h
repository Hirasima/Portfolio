// Fill out your copyright notice in the Description page of Project Settings.


// 23cu0221

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.h"
#include "StageSubSystem.h"
#include "ItemManager.h"
#include "EnigmaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEnigmaGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	// ‚±‚±‚ÉŽg‚¢‚½‚¢UGameInstance‚ð‹L“ü
private:
	UGameManager* m_GameManager;

	UStageSubSystem* m_stageSubSystem;

	UItemManager* m_itemManager;
};
