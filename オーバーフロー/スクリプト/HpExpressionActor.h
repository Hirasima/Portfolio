// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HpExpressionActor.generated.h"

UCLASS()
class ORIGINALACTION_API AHpExpressionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHpExpressionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//HPパーセントを受け取って色を変化させる関数
	void ChangeColor_ByHp(float _hpPercent);

	//プロパティ
	//メッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_pStaticMeshComp;
	//色情報
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HPColor")
	FLinearColor HeightHP_Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HPColor")
	FLinearColor MidHP_Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HPColor")
	FLinearColor LowHP_Color;

protected:
	UPROPERTY()
	UMaterialInterface* m_pMaterial;
};
