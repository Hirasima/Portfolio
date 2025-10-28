// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "LineLight.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API ALineLight : public AStaticMeshActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay()override;
	virtual void OnConstruction(const FTransform& Transform)override;
	void ChangeColor(FLinearColor _color);
public:
	//Œõ‚ðƒIƒ“ƒIƒt‚·‚é‚â‚Â
	void BeLight(bool _switch);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neon")
	FLinearColor Color = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neon")
	bool m_IsfirstActivate;
};
