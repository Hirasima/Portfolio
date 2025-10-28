// Fill out your copyright notice in the Description page of Project Settings.


#include "ChuteRealBigWidget.h"
#include"Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ChuteRealManager.h"
#include "InputCoreTypes.h"
#include "Framework/Application/SlateApplication.h"


//�ŏ��ƍŌ�̃A�j���[�V�����I�����ɌĂ΂��
void UChuteRealBigWidget::HandleAnimFinished() {
	bIsAnimating = false;

	//�t�Đ��ŏI��������ŊJ���ƕ��𔻕�
	float t = GetAnimationCurrentTime(Anim_OpenClose);
	if (bIsOpen) {
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bIsOpen = false;
		//�`���[�g���A���}�l�[�W����T��
		AChuteRealManager* Mana = Cast<AChuteRealManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ChuterealManagerClass));
		//�X�e�[�W��i�߂�
		Mana->AddStageCount();
		RemoveFromParent();
	}
	else {
		bIsOpen = true;
		//���[�h�A�j���[�V�������Đ�
		Info1->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(Anim_WidgetLoad);
		//��
		UGameplayStatics::PlaySound2D(this, OpenSownd, Volume);

		//���~��
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
		FInputModeUIOnly Mode;

		Mode.SetWidgetToFocus(this->TakeWidget());
		PC->SetInputMode(Mode);
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
	}
}

void UChuteRealBigWidget::NativeConstruct() {
	Super::NativeConstruct();

	//�{�^���Ɋ֐����o�C���h
	TabOfInfo1->OnClicked.AddDynamic(this, &UChuteRealBigWidget::OnClickTab1);
	TabOfInfo2->OnClicked.AddDynamic(this, &UChuteRealBigWidget::OnClickTab2);
	CloseButton->OnClicked.AddDynamic(this, &UChuteRealBigWidget::OnClickClose);

	Info1->SetVisibility(ESlateVisibility::Hidden);
	Info2->SetVisibility(ESlateVisibility::Hidden);
	YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
	NoCloseTab->SetVisibility(ESlateVisibility::Hidden);

	bIsBig = true;
}

//����������ꂽ�Ƃ��̊֐�
void UChuteRealBigWidget::PlayClose() {
	if (!bIsOpen || bIsAnimating) {
		return;
	}

	if (!Anim_OpenClose) {
		bIsOpen = false;
		RemoveFromParent();
		return;
	}

	//�A�j���[�V��������true
	bIsAnimating = true;

	//�A�j���[�V�����I�����i���I�������j�Ă΂��f���Q�[�g��ݒ�
	FWidgetAnimationDynamicEvent EndDelegate;
	EndDelegate.BindDynamic(this, &UChuteRealBigWidget::HandleAnimFinished);
	BindToAnimationFinished(Anim_WidgetLoad, EndDelegate);

	//�A�j���[�V�������Đ�
	PlayAnimationReverse(Anim_WidgetLoad);
}

void UChuteRealBigWidget::OnClickTab1() {
	Info1->SetVisibility(ESlateVisibility::Visible);
	Info2->SetVisibility(ESlateVisibility::Hidden);
	m_nowPage = 0;
}

void UChuteRealBigWidget::OnClickTab2() {
	Info1->SetVisibility(ESlateVisibility::Hidden);
	Info2->SetVisibility(ESlateVisibility::Visible);
	m_nowPage = 1;
}

void UChuteRealBigWidget::OnClickClose() {
	PlayClose();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->bShowMouseCursor = false;
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;

	PC->SetInputMode(FInputModeGameOnly());
	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);
}

void UChuteRealBigWidget::OpenCloser() {
	PlayAnimation(Anim_CloserOpen);
	YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
	NoCloseTab->SetVisibility(ESlateVisibility::Visible);
	m_IsOpenCloser = true;
}

void UChuteRealBigWidget::CloseCloser() {
	PlayAnimationReverse(Anim_CloserOpen);
	YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
	NoCloseTab->SetVisibility(ESlateVisibility::Hidden);
	m_IsOpenCloser = false;
}

//�X�e�B�b�N���͂�ݒ肷��֐�
FReply UChuteRealBigWidget::NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) {
	const FKey Key = InAnalogEvent.GetKey();
	if (Key == EKeys::Gamepad_LeftX||Key==EKeys::Gamepad_RightX) {
		const float X = InAnalogEvent.GetAnalogValue();
		
		//��j���[�g����������
		if (m_IsStickNewtral == false) {
			//�j���[�g�����Ȃ�j���[�g�����ɂ���
			if (FMath::Abs(X) < 0.1f) {
				m_IsStickNewtral = true;
				return FReply::Unhandled();
			}
		}
		//�j���[�g�����Ȃ�X������ŏ���
		else {
			if (FMath::Abs(X) >= 0.5f) {
				onStickHorizontal(X);
				m_IsStickNewtral = false;
			}
			return FReply::Unhandled();
		}
	}
	return Super::NativeOnAnalogValueChanged(InGeometry, InAnalogEvent);
}

//�Q�[���p�b�h�̃{�^���������ꂽ������ݒ肷��֐�
FReply UChuteRealBigWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	const FKey Key = InKeyEvent.GetKey();
	
	if (Key == EKeys::Gamepad_FaceButton_Right) {
		//�֐���o�^
		PushedSelectButton(1);
		return FReply::Handled();
	}
	else if (Key == EKeys::Gamepad_FaceButton_Bottom) {
		//�֐���o�^
		PushedSelectButton(-1);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UChuteRealBigWidget::onStickHorizontal(float _value) {
	//�E�|��
	if (_value > 0.f) {
		if (m_IsOpenCloser) {
			//No���I������Ă��錩���ڂɂ���
			YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
			NoCloseTab->SetVisibility(ESlateVisibility::Visible);
			m_IsSelectClose = false;
		}
		else {
			if (m_nowPage == 0) {
				//���̃y�[�W��
				OnClickTab2();
			}
			else {
				//���܂����H�̉�ʂ��J��
				OpenCloser();
			}
		}
	}
	//���|��
	else {
		if (m_IsOpenCloser) {
			//Yes���I������Ă��錩���ڂɂ���
			YesCloseTab->SetVisibility(ESlateVisibility::Visible);
			NoCloseTab->SetVisibility(ESlateVisibility::Hidden);
			m_IsSelectClose = true;
		}
		else {
			//���̃y�[�W�ɖ߂�
			if (m_nowPage == 1) {
				OnClickTab1();
			}
		}
	}
}

void UChuteRealBigWidget::PushedSelectButton(int _value) {
	if (!m_IsOpenCloser) {
		//����H���J���Ă��Ȃ��Ȃ�X�e�B�b�N��|�������Ɠ����悤�Ƀy�[�W���߂���
		onStickHorizontal(_value);
		return;
	}
	if (m_IsSelectClose) {
		//����Widge�����
		OnClickClose();
		YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
		NoCloseTab->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		//���܂����H�����
		CloseCloser();
	}
}