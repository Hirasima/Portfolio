// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void SetItemTexturePath(int _indexNum, FString _path);

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<UTexture2D*> m_itemTextureArray;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> m_itemTexturePathArray;

	UPROPERTY(BlueprintReadOnly)
	TArray<FSlateBrush> m_itemBrush;
};
