// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemBase.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class UItemManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	//���������ɌĂяo�����֐�
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//�I�����ɌĂяo�����֐�
	virtual void Deinitialize() override;

	//---------------------------------------------------
	//�I���W�i�����\�b�h
	//---------------------------------------------------

	//�A�C�e���̓o�^
	void RegisterItem(AItemBase* _item);

	//�A�C�e�������ꂽ�Ƃ��ɌĂ΂��
	void OnGetItem(AItemBase* _item);

	//�A�C�e���E�B�W�F�b�g�̐���
	void GenerateItemWidget();

	//�A�C�e���E�B�W�F�b�g�̔j��
	void RemoveItemWidget();

	//�A�C�e���e�N�X�`���p�X���擾
	TArray<FString> GetItemTexturePath();

	//�A�C�e���擾�󋵂̎擾
	FString GetItemString();

	//---------------------------------------------------
	//�v���p�e�B
	//---------------------------------------------------

private:
	//�A�C�e��
	TArray<AItemBase*> m_itemArray;

	//�A�C�e���擾�ς݃e�N�X�`��
	UTexture2D* m_itemTexture_on;

	//�A�C�e�����擾�e�N�X�`��
	UTexture2D* m_itemTexture_off;

	//��A�C�e��
	UTexture2D* m_itemTexture_empty;

	//�A�C�e���E�B�W�F�b�g
	UUserWidget* m_itemWidget;

	//�A�C�e���̏��
	enum ItemStatus
	{
		none,
		idle,
		get,
	};
	TArray<ItemStatus> m_itemStatusArray;

	//�X�e�[�W�A�C�e����
	int m_maxItemCount;

	int m_getItemCount;
};
