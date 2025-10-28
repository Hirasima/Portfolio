// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTickWorldSubsystem.h"
#include "Engine/World.h"

//�󋵂ɂ�鐶���ۂ̒�`
bool UGameTickWorldSubsystem::ShouldCreateSubsystem(UObject* Outer)const {
    const UWorld* World=Cast<UWorld>(Outer);
    if(!World)return false;

    //�Q�[������PIE�̂ݐ���
    const auto WT =World->WorldType;

    if (WT == EWorldType::Game || WT == EWorldType::PIE) {
        return true;
    }
    return false;
}

//���C�t�T�C�N��
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
