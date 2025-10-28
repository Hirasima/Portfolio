// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackArea.generated.h"

UCLASS()
class ORIGINALACTION_API AAttackArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�@�I���W�i�����\�b�h ////////////////////////////////////




	// �v���p�e�B ///////////////////////////////////////////
public:

	//�R���W����
	UPROPERTY(EditDefaultsOnly)
	UShapeComponent* ShapeComp;

	//�^����_���[�W��
	UPROPERTY(EditDefaultsOnly)
	int AttackPoint;

	//�_���[�W��^�������
	UPROPERTY(EditDefaultsOnly)
	float AttackPeriod;

	//�_���[�W��^�����
	UPROPERTY(EditDefaultsOnly)
	int AttackNum;

private:

	//�_���[�W��^������
};
