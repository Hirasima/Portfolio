// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChuteRealManager.generated.h"

UCLASS()
class ORIGINALACTION_API AChuteRealManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChuteRealManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddDeadCount();

	void AddStageCount();

protected:
	//ステージ０～６用シングルスポナー
	UPROPERTY(EditAnywhere)
	class AEnemySpawner* m_pFirstSpawner = nullptr;
	//ステージ7～10用マルチスポナー
	UPROPERTY(EditAnywhere)
	class AEnemySpawner* m_pMultipleSpawner = nullptr;
	//中ボススポナー
	UPROPERTY(EditAnywhere)
	class AEnemySpawner* m_pBossSpawner = nullptr;

	//ステージ1～2：攻撃チュートリアル
	UPROPERTY(EditAnywhere,Category="Widget")
	TSubclassOf<class UChuteRealMiniWidget> ChuterealWindows;
	//上記Widget表示時間
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_AttackChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_AttackChuterealWidget;

	UPROPERTY()
	UChuteRealMiniWidget* WindowRef = nullptr;

	//ステージ3～4:バフ説明
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> BuffChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_BuffChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_BuffChuterealWidget;

	//ステージ5～６:受けチュートリアル
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> GuardChuterealWindow;
	//上記Widget表示時間
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_GuardChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_GuardChuterealWidget;

	//ステージ3～4:空中コンボ説明
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> AirAttackChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_AirAttackChuterealWidget;

	//ステージ5～６:回復チュートリアル
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget>HealChuterealWindow;
	//上記Widget表示時間
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_HealChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_HealChuterealWidget;

	//ステージ3～4:フロー説明
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> FlowChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_FlowChuterealWidget;

	//ステージ3～4:パリィ説明
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> ParryChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_ParryChuterealWidget;

	//最後に出す導線用Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* m_KeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> m_KeyWidgetClass;



	//タイマー
	float ChuterealTimer = 0.0f;
	bool m_IsTimerStop = false;

	float SpawnTimer = 0.0f;
	int DeadCount = 0;

	//チュートリアル工数
	int stage = 0;

	bool IsSpawn = false;

};
