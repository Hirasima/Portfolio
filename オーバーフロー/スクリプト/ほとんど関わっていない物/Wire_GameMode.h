// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Wire_GameMode.generated.h"

//�N���X�̑O���錾
class AEnemyManager;
class ABattleManager;


UCLASS()
class ORIGINALACTION_API AWire_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	//�R���X�g���N�^
	AWire_GameMode();
private:
	//�Q�[���J�n������
	virtual void BeginPlay() override;
public:
	//�G�l�~�[�}�l�[�W����Ԃ��֐�
	AEnemyManager* GetEnemyManager();
	//�o�g���}�l�[�W����Ԃ��֐�
	ABattleManager* GetBattleManager();

private:
	UPROPERTY()
	AEnemyManager* m_EnemyManager;
	ABattleManager* m_BattleManager;
};
