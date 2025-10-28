// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Kismet/GameplayStatics.h"



void UTitleWidget::NativeConstruct() {
	Super::NativeConstruct();
	bIsFocusable = true;
}

void UTitleWidget::PushedSelectButton(int _value) {
	UE_LOG(LogTemp, Warning, TEXT("TitleCall!"));
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->bShowMouseCursor = false;
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;

	PC->SetInputMode(FInputModeGameOnly());
	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);

	//���~�߉������Ď��g������
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();

	if (m_pTargetLevel.IsNull()) {
		return;
	}

	FName LevelName = FPackageName::GetShortFName(m_pTargetLevel.GetAssetName());

	UGameplayStatics::OpenLevel(this, LevelName);
}

//�Q�[���p�b�h�̃{�^���������ꂽ������ݒ肷��֐�
FReply UTitleWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Gamepad_FaceButton_Right) {
		//�֐���o�^
		PushedSelectButton(1);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}