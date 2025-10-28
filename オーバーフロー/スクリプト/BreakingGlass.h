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
	//コンストラクタ
	ABreakingGlass();
	//インタラクション時呼ばれる関数
	virtual void UseInteraction()override;
protected:
	//初期化
	virtual void BeginPlay()override;
public:
	//破片エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* m_pBreakingGlassNia;
	//インタラクション感知範囲用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UBoxComponent* m_pNiagaraTarget;
	//落下用ガラスか否か
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool m_IsFall;
};
