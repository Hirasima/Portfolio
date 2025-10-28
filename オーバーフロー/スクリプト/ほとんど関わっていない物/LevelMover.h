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
	//初期化
	virtual void BeginPlay()override;

public:
	//インタラクション感知範囲用コリジョンを利用するか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_IsUseCollision;
	//インタラクション感知範囲用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (EditCondition = "m_IsUseCollision"))
	class ATriggerBox* m_pSpwnTrrigerCollision;

	UPROPERTY(EditAnywhere,Category="Level")
	TSoftObjectPtr<UWorld> m_pTargetLevel;
};
