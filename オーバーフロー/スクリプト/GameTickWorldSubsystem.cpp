// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTickWorldSubsystem.h"
#include "Engine/World.h"

//状況による生成可否の定義
bool UGameTickWorldSubsystem::ShouldCreateSubsystem(UObject* Outer)const {
    const UWorld* World=Cast<UWorld>(Outer);
    if(!World)return false;

    //ゲーム中とPIEのみ生成
    const auto WT =World->WorldType;

    if (WT == EWorldType::Game || WT == EWorldType::PIE) {
        return true;
    }
    return false;
}

//ライフサイクル
void UGameTickWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
    Super::Initialize(Collection);
}
//Tick
void UGameTickWorldSubsystem::Tick(float _deltaTime) {

}

//
TStatId UGameTickWorldSubsystem::GetStatId()const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UWorldSubsystem,STATGROUP_Tickables);
}
