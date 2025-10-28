// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "BreakingGlass.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API ABreakingGlass : public AInteractionActor
{
	GENERATED_BODY()
public:
	//�R���X�g���N�^
	ABreakingGlass();
	//�C���^���N�V�������Ă΂��֐�
	virtual void UseInteraction()override;
protected:
	//������
	virtual void BeginPlay()override;
public:
	//�j�ЃG�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* m_pBreakingGlassNia;
	//�C���^���N�V�������m�͈͗p�R���W����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UBoxComponent* m_pNiagaraTarget;
	//�����p�K���X���ۂ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool m_IsFall;
};
