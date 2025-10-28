// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRouter.generated.h"

UCLASS()
class ORIGINALACTION_API AEnemyRouter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyRouter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SendNextPos(AActor* _a, AActor* _b);

protected:
	//インタラクション感知範囲用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Routes")
	TArray<class ATriggerBox*> m_pRouteCoollisions;

};
