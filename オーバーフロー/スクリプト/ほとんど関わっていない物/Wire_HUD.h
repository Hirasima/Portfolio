// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayPawn.h"
#include "Wire_HUD.generated.h"

//�N���X�̑O���錾
class UPlayerGageWidget;

UCLASS()
class ORIGINALACTION_API AWire_HUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//�v���C���[�̃Q�[�W��Widget�̃N���X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerGageWidgetClass;

	//�v���C���[�̃Q�[�W��Widget�̎���
	UPROPERTY(VisibleInstanceOnly)
	UPlayerGageWidget* m_PlayerGageWidget;

	//�v���C���[��HP��ύX����֐�
	void SetPlayerHpPercent(float _percent);

	//�񕜃A�C�e���̏�Ԃ�ݒ�
	void SetRecoveryItemState(int _itemNum, float _recastRatio);

	//�v���C���[�̃o�t�󋵂�ݒ�
	void SetPlayerBuff(TArray<EBuffPattern>& _buffsArray);

	//�o�t�̓_�ł��邩��ݒ�
	void SetBuffIsPoint(bool _isPoint);
};
