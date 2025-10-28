// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "InteractionActor.generated.h"

//�N���X�̑O���錾
class UBoxComponent;
class UWidgetComponent;

UCLASS()
class ORIGINALACTION_API AInteractionActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	AInteractionActor();
	//������
	virtual void BeginPlay()override;

	//�C���^���N�V�������Ă΂��֐�
	virtual void UseInteraction();

	//�C���^���N�V�����ΏۂƂ��ėL�����ۂ����擾����֐�
	bool GetIsValiable();

	//���g�̗��p�����^�[�W����Ԃ��֐�
	UAnimMontage* GetInteractMontage();

	UFUNCTION()
	virtual void BeVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION()
	virtual void BeUnVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);



	virtual void BeUnVisibleWidget();

public:
	//�C���^���N�V�������m�͈͗p�R���W����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pInteractionTriggerCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pRootTriggerCollision;

	//�L�[Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* m_KeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> m_KeyWidgetClass;

	//�A�j���[�W���������^�[�W��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* m_pIntaeractMontage;
	//�A�j���[�W���������^�[�W��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	bool m_UseMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
	bool m_AutoUsing=false;

protected:
	//�C���^���N�V�����ΏۂƂ��ėL�����ۂ����q�ŏ��������������ǂ��Ɣ��f
	bool m_isValiable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSpawner")
	bool m_IsUseSpawner = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSpawner")
	TArray<class AEnemySpawner*> m_pTargetSpawners;
};
