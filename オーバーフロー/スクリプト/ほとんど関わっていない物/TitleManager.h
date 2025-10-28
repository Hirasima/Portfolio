// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleManager.generated.h"

UCLASS()
class ORIGINALACTION_API ATitleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATitleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//�A�^�b�N�A�Z�b�g���e�ڍs�\��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UTitleWidget> m_pTitleWidgetClass;

protected:
	//Wdiget�̃|�C���^�[
	UTitleWidget* m_pWidget;

};
