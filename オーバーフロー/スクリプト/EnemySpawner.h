// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

//クラスの前方宣言
class AEnemy_Base;
class AEnemyMarker;
class UBoxComponent;

struct FEnemyData;


UCLASS()
class ORIGINALACTION_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void DoSpawnEnemy(AActor* _a, AActor* _b);
	void DoSpawnEnemy(int _SpawnCount=0);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<AEnemyMarker*> SpawnTargets;
	//インタラクション感知範囲用コリジョンを利用するか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_IsUseCollision;
	//インタラクション感知範囲用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn",meta=(EditCondition="m_IsUseCollision"))
	class ATriggerBox* m_pSpwnTrrigerCollision;
	//湧き間隔時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float m_SpawnLag;
	//使用回数制限
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_IsSpawnOnlyOnce = false;
protected:
	UPROPERTY()
	TArray<FEnemyData> SpawnEnemyDatas;

	bool m_IsUsed;
	float m_nowSpawnCountTime;
	int m_nowSpawnCount;
	bool m_IsLocked = false;
};
