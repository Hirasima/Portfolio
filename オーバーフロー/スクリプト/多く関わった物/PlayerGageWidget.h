// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayPawn.h"
#include "PlayerGageWidget.generated.h"

//�N���X�̑O���錾
class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class ORIGINALACTION_API UPlayerGageWidget : public UUserWidget
{
	GENERATED_BODY()

	//�R���X�g���N�^
	virtual void NativeConstruct()override;

public:
	//HP�̊������󂯎��֐�
	void SetHpPercent(float _percent);

	//�����ǉ�

	//�񕜃A�C�e���̏�Ԃ�ݒ�
	void SetRecoveryItemState(int _itemNum, float _recastRatio);

	//�o�t�̏�Ԃ�ݒ�
	void SetBuffs(TArray<EBuffPattern>& _buffsArray);

	//�o�t�̏�Ԏ擾
	UFUNCTION(BlueprintCallable)
	TArray<EBuffPattern> GetBuffs();

	//�o�t�̓_�ł��邩��ݒ�
	void SetBuffIsPoint(bool _isPoint);

protected:
	//---------------------------------------
	//�o�C���h�ϐ� (�ϐ����ƃG�f�B�^��̖��O����v������)
	//---------------------------------------

	//HP�o�[
	UPROPERTY(meta = (BindWidget))
	UImage* HPBarImage;

	//�m�C�Y�t���[���t���[��
	UPROPERTY(meta = (BindWidget))
	UImage* HP_Frame;

	//�A�C�e���̐�
	UPROPERTY(meta = (BindWidget))
	UImage* ItemNumber;

	//�A�C�e���̃��L���X�g�^�C��
	UPROPERTY(meta = (BindWidget))
	UImage* ItemRatio;

	//---------------------------------------
	//���I�}�e���A���C���X�^���X
	//---------------------------------------

	//�t���[���̃}�e���A���C���X�^���X
	UPROPERTY()
	UMaterialInstanceDynamic* HP_FrameMat;

	//���I�}�e���A���C���X�^���X
	UPROPERTY()
	UMaterialInstanceDynamic* BarMat;

	//�A�C�e�����̓��I�}�e���A��
	UPROPERTY()
	UMaterialInstanceDynamic* ItemNumMat;

	//�A�C�e���̃��L���X�g�^�C���̓��I�}�e���A��
	UPROPERTY()
	UMaterialInstanceDynamic* ItemRatioMat;

	//�o�t��UI�̉摜
	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_0;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_1;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_2;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_3;

	UPROPERTY(meta = (BindWidget))
	UImage* Buff_In_4;
	//�܂Ƃ߂ĊǗ�����ׂ̔z��
	TArray<UImage*> Buff_In;

	//���I�}�e���A���C���X�^���X
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> BuffIcons;
	
	//�o�t�ۑ��p
	UPROPERTY(EditAnywhere)
	TArray<EBuffPattern> Buff;
};
