// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChuteRealMiniWidget.generated.h"



/**
 * 
 */
UCLASS()
class ORIGINALACTION_API UChuteRealMiniWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="Window")
	void PlayOpen();

	UFUNCTION(BlueprintCallable, Category = "Window")
	virtual void PlayClose();

protected:
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	UWidgetAnimation* Anim_OpenClose = nullptr;
	//‰¹
	UPROPERTY(EditAnywhere,Category="SFX")
	TObjectPtr<USoundBase> OpenSownd;
	UPROPERTY(EditAnywhere, Category = "SFX")
	float Volume = 1.0f;

	UFUNCTION()
	virtual void HandleAnimFinished();

	bool bIsOpen = false;
	bool bIsAnimating = false;
	bool bIsBig = false;
	
};
