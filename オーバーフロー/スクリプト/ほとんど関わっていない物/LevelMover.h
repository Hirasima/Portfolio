// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "LevelMover.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API ALevelMover : public AInteractionActor
{
	GENERATED_BODY()
public:
	virtual void UseInteraction()override;
	UFUNCTION()
	virtual void HitMoveLevel(AActor* _a,AActor* _b);
protected:
	//������
	virtual void BeginPlay()override;

public:
	//�C���^���N�V�������m�͈͗p�R���W�����𗘗p���邩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_IsUseCollision;
	//�C���^���N�V�������m�͈͗p�R���W����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (EditCondition = "m_IsUseCollision"))
	class ATriggerBox* m_pSpwnTrrigerCollision;

	UPROPERTY(EditAnywhere,Category="Level")
	TSoftObjectPtr<UWorld> m_pTargetLevel;
};
