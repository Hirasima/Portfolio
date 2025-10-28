// Fill out your copyright notice in the Description page of Project Settings.


#include "ChuteRealBigWidget.h"
#include"Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ChuteRealManager.h"
#include "InputCoreTypes.h"
#include "Framework/Application/SlateApplication.h"


//最初と最後のアニメーション終了時に呼ばれる
void UChuteRealBigWidget::HandleAnimFinished() {
	bIsAnimating = false;

	//逆再生で終わったかで開きと閉じを判別
	float t = GetAnimationCurrentTime(Anim_OpenClose);
	if (bIsOpen) {
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bIsOpen = false;
		//チュートリアルマネージャを探す
		AChuteRealManager* Mana = Cast<AChuteRealManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ChuterealManagerClass));
		//ステージを進める
		Mana->AddStageCount();
		RemoveFromParent();
	}
	else {
		bIsOpen = true;
		//ロードアニメーションを再生
		Info1->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(Anim_WidgetLoad);
		//音
		UGameplayStatics::PlaySound2D(this, OpenSownd, Volume);

		//時止め
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

	//ボタンに関数をバインド
	TabOfInfo1->OnClicked.AddDynamic(this, &UChuteRealBigWidget::OnClickTab1);
	TabOfInfo2->OnClicked.AddDynamic(this, &UChuteRealBigWidget::OnClickTab2);
	CloseButton->OnClicked.AddDynamic(this, &UChuteRealBigWidget::OnClickClose);

	Info1->SetVisibility(ESlateVisibility::Hidden);
	Info2->SetVisibility(ESlateVisibility::Hidden);
	YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
	NoCloseTab->SetVisibility(ESlateVisibility::Hidden);

	bIsBig = true;
}

//閉じるを押されたときの関数
void UChuteRealBigWidget::PlayClose() {
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
	EndDelegate.BindDynamic(this, &UChuteRealBigWidget::HandleAnimFinished);
	BindToAnimationFinished(Anim_WidgetLoad, EndDelegate);

	//アニメーションを再生
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

//スティック入力を設定する関数
FReply UChuteRealBigWidget::NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) {
	const FKey Key = InAnalogEvent.GetKey();
	if (Key == EKeys::Gamepad_LeftX||Key==EKeys::Gamepad_RightX) {
		const float X = InAnalogEvent.GetAnalogValue();
		
		//非ニュートラル時処理
		if (m_IsStickNewtral == false) {
			//ニュートラルならニュートラルにする
			if (FMath::Abs(X) < 0.1f) {
				m_IsStickNewtral = true;
				return FReply::Unhandled();
			}
		}
		//ニュートラルなら傾き次第で処理
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

//ゲームパッドのボタンが押された処理を設定する関数
FReply UChuteRealBigWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	const FKey Key = InKeyEvent.GetKey();
	
	if (Key == EKeys::Gamepad_FaceButton_Right) {
		//関数を登録
		PushedSelectButton(1);
		return FReply::Handled();
	}
	else if (Key == EKeys::Gamepad_FaceButton_Bottom) {
		//関数を登録
		PushedSelectButton(-1);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UChuteRealBigWidget::onStickHorizontal(float _value) {
	//右倒し
	if (_value > 0.f) {
		if (m_IsOpenCloser) {
			//Noが選択されている見た目にする
			YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
			NoCloseTab->SetVisibility(ESlateVisibility::Visible);
			m_IsSelectClose = false;
		}
		else {
			if (m_nowPage == 0) {
				//次のページへ
				OnClickTab2();
			}
			else {
				//閉じますか？の画面を開く
				OpenCloser();
			}
		}
	}
	//左倒し
	else {
		if (m_IsOpenCloser) {
			//Yesが選択されている見た目にする
			YesCloseTab->SetVisibility(ESlateVisibility::Visible);
			NoCloseTab->SetVisibility(ESlateVisibility::Hidden);
			m_IsSelectClose = true;
		}
		else {
			//元のページに戻る
			if (m_nowPage == 1) {
				OnClickTab1();
			}
		}
	}
}

void UChuteRealBigWidget::PushedSelectButton(int _value) {
	if (!m_IsOpenCloser) {
		//閉じる？を開いていないならスティックを倒した時と同じようにページをめくる
		onStickHorizontal(_value);
		return;
	}
	if (m_IsSelectClose) {
		//このWidgeを閉じる
		OnClickClose();
		YesCloseTab->SetVisibility(ESlateVisibility::Hidden);
		NoCloseTab->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		//閉じますか？を閉じる
		CloseCloser();
	}
}