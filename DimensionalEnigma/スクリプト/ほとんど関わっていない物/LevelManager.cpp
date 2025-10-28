// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)

{
	UE_LOG(LogTemp, Warning, TEXT("ULevelManager::Initialize()"));

	Setting_IA();

	Super::Initialize(Collection);

}

void ULevelManager::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("ULevelManager::Deinitialize()"));

	Super::Deinitialize();

}

void ULevelManager::Setting_IA()
{
	FName IAName = TEXT("/ Game/EnhancedInput/IMC_PlayerMove");
	
	//DefaultMappingContext = 0;
}