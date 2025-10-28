// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "NeonMaker.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API ANeonMaker : public AStaticMeshActor
{
	GENERATED_BODY()
protected:
	virtual void OnConstruction(const FTransform& Transform)override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Neon")
	FLinearColor Color = FLinearColor::Red;
};
