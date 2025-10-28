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

	//アニメーション中をtrue
	bIsAnimating = true;

	//アニメーション終了時（開き終わったら）呼ばれるデリゲートを設定
	FWidgetAnimationDynamicEvent EndDelegate;
	EndDelegate.BindDynamic(this, &UChuteRealMiniWidget::HandleAnimFinished);
	BindToAnimationFinished(Anim_OpenClose, EndDelegate);

	//アニメーションを再生
	PlayAnimation(Anim_OpenClose);

	//音
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

	//アニメーション中をtrue
	bIsAnimating = true;

	//アニメーション終了時（閉じ終わったら）呼ばれるデリゲートを設定
	FWidgetAnimationDynamicEvent EndDelegate;
	EndDelegate.BindDynamic(this, &UChuteRealMiniWidget::HandleAnimFinished);
	BindToAnimationFinished(Anim_OpenClose, EndDelegate);

	//アニメーションを再生
	PlayAnimationReverse(Anim_OpenClose);
}

void UChuteRealMiniWidget::HandleAnimFinished() {
	
	bIsAnimating = false;

	//逆再生で終わったかで開きと閉じを判別
	float t = GetAnimationCurrentTime(Anim_OpenClose);
	if (t <= 0.0f + KINDA_SMALL_NUMBER) {
		bIsOpen = false;
		RemoveFromParent();
	}
	else {
		bIsOpen = true;
	}
}