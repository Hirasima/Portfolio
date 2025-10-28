// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChuteRealMiniWidget.h"
#include "ChuteRealBigWidget.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UChuteRealBigWidget : public UChuteRealMiniWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct()override;

	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent)override;
	void onStickHorizontal(float _value);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)override;
	void PushedSelectButton(int _value);

	virtual void PlayClose()override;

	UFUNCTION()
	void OnClickTab1();
	UFUNCTION()
	void OnClickTab2();
	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OpenCloser();
	UFUNCTION()
	void CloseCloser();


protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Anim_WidgetLoad = nullptr;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Anim_CloserOpen = nullptr;

	//�^�u�؂�ւ��{�^��
	UPROPERTY(meta = (BindWidget))
	class UButton* TabOfInfo1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* TabOfInfo2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton = nullptr;
	//���^�u�摜
	UPROPERTY(meta = (BindWidget))
	class UImage* Info1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UImage* Info2 = nullptr;
	//����HWindow
	UPROPERTY(meta = (BindWidget))
	class UImage* NoCloseTab = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UImage* YesCloseTab = nullptr;

	UPROPERTY(EditAnywhere, Category = "Chutereal")
	TSubclassOf<class AChuteRealManager> ChuterealManagerClass;


	virtual void HandleAnimFinished()override;

	//����H���J���Ă��邩�ۂ�
	bool m_IsOpenCloser = false;
	int m_nowPage = 0;
	//���遨Yes���I������Ă���
	bool m_IsSelectClose = false;
	//���݃X�e�B�b�N���j���[�g�����ł���
	bool m_IsStickNewtral = true;
	
};
