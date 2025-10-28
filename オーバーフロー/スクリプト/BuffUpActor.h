// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "PlayPawn.h"
#include "BuffUpActor.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API ABuffUpActor : public AInteractionActor
{
	GENERATED_BODY()
public:
	virtual void BeVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult)override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	int BuffSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	EBuffPattern BuffKind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MachineMesh")
	UStaticMesh* m_pLockedStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MachineMesh")
	UStaticMesh* m_pUseableStaticMesh;

protected:
	float CoolDownTimer;
};
