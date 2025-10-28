// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "EnemyMarker.generated.h"

class AEnemy_Base;

//エネミーデータ構造体
USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemy_Base> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform EnemyTransform;
};

UCLASS()
class ORIGINALACTION_API AEnemyMarker : public ASkeletalMeshActor
{
	GENERATED_BODY()
public:
	FEnemyData GetEnemyMarkerData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowAbstract="false"), Category = "EnemyType")
	TSubclassOf<AEnemy_Base> m_EnemyType;
};
