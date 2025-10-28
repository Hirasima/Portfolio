// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	//�����N�̃e�N�X�`���擾
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetRank(UTexture2D* _texture);

	//�N���A�^�C���̎擾
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetClearTime(float _clearTime);

	//�v���C���[�����̎擾
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPlayerStep(int _playerStep);

	//�J������]�񐔎擾
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetCameraMove(int _cameraMove);

	//�A�C�e���e�N�X�`���̎擾
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetItemTexture(UTexture2D* _newTexture);

public:
	//�����N�̃e�N�X�`��
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* m_rankTexture;

	//�A�C�e���e�N�X�`���̔z��
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> m_itemTexturePathArray;

	//�A�C�e���u���b�V��
	UPROPERTY(BlueprintReadOnly)
	TArray<FSlateBrush> m_itemBrush;

	//�����N�u���b�V��
	UPROPERTY(BlueprintReadOnly)
	FSlateBrush m_rankBrush;

	//�N���A�^�C��
	UPROPERTY(BlueprintReadOnly)
	float m_clearTime;

	//�v���C���[����
	UPROPERTY(BlueprintReadOnly)
	int m_playerStep;

	//�J������]��
	UPROPERTY(BlueprintReadOnly)
	int m_cameraMove;

	//�A�C�e���̎擾�󋵕�����
	UPROPERTY(BlueprintReadOnly)
	FString m_itemString;
};
