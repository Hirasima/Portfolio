// Fill out your copyright notice in the Description page of Project Settings.


#include "Wire_HUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerGageWidget.h"
#include "Kismet/GameplayStatics.h"

//
void AWire_HUD::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Called HUD BeginPlay"));
	//�v���C���[�̃Q�[�W��Widget�������A�`��//////////////////////////////////////////////////////////
	if (!PlayerGageWidgetClass) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:PlayerGageWidgetClass In BeginPlay_WireHUD"));
		return;
	}
	//����
	m_PlayerGageWidget = CreateWidget<UPlayerGageWidget>(GetWorld(), PlayerGageWidgetClass);
	if (!m_PlayerGageWidget) {
		UE_LOG(LogTemp, Warning, TEXT("Create Faild:PlayerGageWidget In BeginPlay_WireHUD"));
		return;
	}
	//�`��
	m_PlayerGageWidget->AddToViewport();

	//m_PlayerGageWidget->SetVisibility(ESlateVisibility::Collapsed);
	////////////////////////////////////////////////////////////////////////////////////////////////
}

//�v���C���[��HP��ύX����֐�
void AWire_HUD::SetPlayerHpPercent(float _percent) {
	if (!m_PlayerGageWidget) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:PlayerGageWidget In HUD_SetPlayerHpPercent"));
		return;
	}
	m_PlayerGageWidget->SetHpPercent(_percent);
}

//�񕜃A�C�e���̏�Ԃ�ݒ�
void AWire_HUD::SetRecoveryItemState(int _itemNum, float _recastRatio)
{
	if (!m_PlayerGageWidget) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:PlayerGageWidget In HUD_SetPlayerHpPercent"));
		return;
	}

	m_PlayerGageWidget->SetRecoveryItemState(_itemNum, _recastRatio);
}

//�v���C���[�̃o�t�󋵂�ݒ�
void AWire_HUD::SetPlayerBuff(TArray<EBuffPattern>& _buffsArray)
{
	if (!m_PlayerGageWidget) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found:PlayerGageWidget In HUD_SetPlayerHpPercent"));
		return;
	}
	m_PlayerGageWidget->SetBuffs(_buffsArray);
}

//�o�t�̓_�ł��邩��ݒ�
void AWire_HUD::SetBuffIsPoint(bool _isPoint)
{
	m_PlayerGageWidget->SetBuffIsPoint(_isPoint);
}