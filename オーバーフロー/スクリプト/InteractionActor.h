// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "InteractionActor.generated.h"

//クラスの前方宣言
class UBoxComponent;
class UWidgetComponent;

UCLASS()
class ORIGINALACTION_API AInteractionActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AInteractionActor();
	//初期化
	virtual void BeginPlay()override;

	//インタラクション時呼ばれる関数
	virtual void UseInteraction();

	//インタラクション対象として有効化否かを取得する関数
	bool GetIsValiable();

	//自身の利用モンタージュを返す関数
	UAnimMontage* GetInteractMontage();

	UFUNCTION()
	virtual void BeVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION()
	virtual void BeUnVisibleWidget(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);



	virtual void BeUnVisibleWidget();

public:
	//インタラクション感知範囲用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pInteractionTriggerCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pRootTriggerCollision;

	//キーWidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* m_KeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> m_KeyWidgetClass;

	//アニメージョンモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* m_pIntaeractMontage;
	//アニメージョンモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	bool m_UseMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
	bool m_AutoUsing=false;

protected:
	//インタラクション対象として有効か否か★子で初期化した方が良きと判断
	bool m_isValiable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSpawner")
	bool m_IsUseSpawner = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSpawner")
	TArray<class AEnemySpawner*> m_pTargetSpawners;
};
