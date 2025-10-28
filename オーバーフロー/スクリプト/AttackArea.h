// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackArea.generated.h"

UCLASS()
class ORIGINALACTION_API AAttackArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//　オリジナルメソッド ////////////////////////////////////




	// プロパティ ///////////////////////////////////////////
public:

	//コリジョン
	UPROPERTY(EditDefaultsOnly)
	UShapeComponent* ShapeComp;

	//与えるダメージ量
	UPROPERTY(EditDefaultsOnly)
	int AttackPoint;

	//ダメージを与える周期
	UPROPERTY(EditDefaultsOnly)
	float AttackPeriod;

	//ダメージを与える回数
	UPROPERTY(EditDefaultsOnly)
	int AttackNum;

private:

	//ダメージを与えた回数
};
