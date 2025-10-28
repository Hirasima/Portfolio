// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct()override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)override;

	void PushedSelectButton(int _value);
	
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> m_pTargetLevel;
};
