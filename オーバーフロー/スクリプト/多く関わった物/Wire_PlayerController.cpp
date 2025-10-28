// Fill out your copyright notice in the Description page of Project Settings.

#include "InputMappingContext.h"
#include "Wire_PlayerController.h"
#include "Wire_PCManager.h"
#include "EnhancedInputSubsystems.h"

AWire_PlayerController::AWire_PlayerController()
{
	//PlayerCameraManager��ݒ�
	PlayerCameraManagerClass = m_PlayerCameraManagerClass;
}

void AWire_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���͊֌W������ ////////////////////////////////////////////////////////////

	//���[�J���v���C���[
	ULocalPlayer* localPlayer = nullptr;
	//���̓T�u�V�X�e��
	UEnhancedInputLocalPlayerSubsystem* enhancedInputLocalPlayerSubSystem = nullptr;

	//���[�J���v���C���[���擾
	localPlayer = GetLocalPlayer();

	//���[�J���v���C���[���L���łȂ���ΏI��
	if (!localPlayer) return;

	//���̓T�u�V�X�e�����擾
	enhancedInputLocalPlayerSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);

	//���̓T�u�V�X�e�����L���łȂ���ΏI��
	if (!localPlayer) return;
	//�}�b�s���O�R���e�L�X�g���L���łȂ���ΏI��
	if (!DefaultInputMappingContext) return;

	//���̓T�u�V�X�e���Ƀ}�b�s���O�R���e�L�X�g��ǉ�
	enhancedInputLocalPlayerSubSystem->AddMappingContext(DefaultInputMappingContext, 0);

	UE_LOG(LogTemp, Log, TEXT("PlayerController : Input Initializing Complete"));

	// �J�����֌W������ ////////////////////////////////////////////////////////////////

	//�f�t�H���g�|�[��
	APawn* defaultPawn = nullptr;
	//�|�[���擾
	defaultPawn = GetPawn();

	//�|�[�����L���łȂ���ΏI��
	if (!defaultPawn)return;

	//�^�[�Q�b�g�r���[��ݒ�
	SetViewTarget(defaultPawn);

	UE_LOG(LogTemp, Log, TEXT("PlayerControlller : ViewTarget Initializing Complete"));
}