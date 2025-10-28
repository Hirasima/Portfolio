// Fill out your copyright notice in the Description page of Project Settings.


#include "ChuteRealMiniWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include"Sound/SoundBase.h"


void UChuteRealMiniWidget::PlayOpen() {
	if (bIsOpen || bIsAnimating) {
		return;
	}

	if (!IsInViewport()) {
		AddToViewport(99);
	}
	
	if (!Anim_OpenClose) {
		bIsOpen = true;
		return;
	}

	//�A�j���[�V��������true
	bIsAnimating = true;

	//�A�j���[�V�����I�����i�J���I�������j�Ă΂��f���Q�[�g��ݒ�
	FWidgetAnimationDynamicEvent EndDelegate;
	EndDelegate.BindDynamic(this, &UChuteRealMiniWidget::HandleAnimFinished);
	BindToAnimationFinished(Anim_OpenClose, EndDelegate);

	//�A�j���[�V�������Đ�
	PlayAnimation(Anim_OpenClose);

	//��
	if (!bIsBig) {
		UGameplayStatics::PlaySound2D(this, OpenSownd, Volume);
	}

}

void UChuteRealMiniWidget::PlayClose() {
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
	EndDelegate.BindDynamic(this, &UChuteRealMiniWidget::HandleAnimFinished);
	BindToAnimationFinished(Anim_OpenClose, EndDelegate);

	//�A�j���[�V�������Đ�
	PlayAnimationReverse(Anim_OpenClose);
}

void UChuteRealMiniWidget::HandleAnimFinished() {
	
	bIsAnimating = false;

	//�t�Đ��ŏI��������ŊJ���ƕ��𔻕�
	float t = GetAnimationCurrentTime(Anim_OpenClose);
	if (t <= 0.0f + KINDA_SMALL_NUMBER) {
		bIsOpen = false;
		RemoveFromParent();
	}
	else {
		bIsOpen = true;
	}
}