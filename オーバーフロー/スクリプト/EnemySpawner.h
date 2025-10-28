// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

//�N���X�̑O���錾
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
	//�C���^���N�V�������m�͈͗p�R���W�����𗘗p���邩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_IsUseCollision;
	//�C���^���N�V�������m�͈͗p�R���W����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn",meta=(EditCondition="m_IsUseCollision"))
	class ATriggerBox* m_pSpwnTrrigerCollision;
	//�N���Ԋu����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float m_SpawnLag;
	//�g�p�񐔐���
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
