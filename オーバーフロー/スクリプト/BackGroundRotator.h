// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BackGroundRotator.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API ABackGroundRotator : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ABackGroundRotator();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

public:
	UPROPERTY(EditAnywhere)
	AStaticMeshActor* m_pTarget;

protected:
	UPROPERTY()
	class APlayPawn* m_pPlayer;
};
