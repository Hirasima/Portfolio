// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerForceMover.generated.h"

UCLASS()
class ORIGINALACTION_API APlayerForceMover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerForceMover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StartFallOut(AActor* _a, AActor* _b);
	UFUNCTION()
	void EndFallOut(AActor* _a, AActor* _b);

public:
	//インタラクション感知範囲用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallOut")
	class ATriggerBox* m_pStartTrrigerCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallOut")
	ATriggerBox* m_pEndTrrigerCollision;
	//破片エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallOut")
	class UNiagaraSystem* m_pBreakingGlassNia;

protected:
	bool m_IsValiable;
	bool m_IsTargetFalling;
	float m_fallingTimer;
	UPROPERTY()
	class APlayPawn* m_pTargetPlayer;
};
