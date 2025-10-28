// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayPawn.h"
#include "BuffBitManager.generated.h"

class ABuffBitActor;
class USpringArmComponent;

UCLASS()
class ORIGINALACTION_API ABuffBitManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuffBitManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//オリジナルメソッド///////////////////////////////////

public:

	//初期化
	void Init(USkeletalMeshComponent* _parentMesh = nullptr);

	//バフビットの移動処理
	void UpdateBitMove();

	//バフの状態設定
	void SetBuffs(TArray<EBuffPattern>& _buffs);

	//ビットの点滅状態切り替え
	void SetIsPoint(bool _isPoint);

	//点滅状態か取得
	bool GetIsPoint();

	//ビットの追撃
	void Attack(AActor* _actor);

	//プロパティ//////////////////////////////////////////

public:
	//ビットクラス
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuffBitActor> BuffBitActorClass;

protected:

	//ビットの入れ物
	UPROPERTY(VisibleInstanceOnly)
	TArray<ABuffBitActor*> m_pBuffBitActors;

	USkeletalMeshComponent* m_pParentMeshComp;

	//点滅状態か
	bool IsPoint;

};
