// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogController.generated.h"

UCLASS()
class ORIGINALACTION_API AFogController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ReveilFog(AActor* _a, AActor* _b);
	UFUNCTION()
	void VeilFog(AActor* _a, AActor* _b);

public:
	//インタラクション感知範囲用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	class ATriggerBox* m_pTrrigerCollision;

	//対象霧
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	class AStaticMeshActor* m_pTargetFog;

protected:
	bool m_IsUsed;
	float m_FogTimer;
};
