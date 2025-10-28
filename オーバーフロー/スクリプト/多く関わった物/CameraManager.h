// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

//�O���錾
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ORIGINALACTION_API ACameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �I���W�i�����\�b�h //////////////////////////////////////////////////////////////////////

public:



	// �v���p�e�B //////////////////////////////////////////////////////////////////////

public:
	//�X�v�����O�A�[�����[�g�R���|�[�l���g����̏����ʒu
	UPROPERTY(EditAnywhere)
	FVector m_springArmOffset;

	//���񑬓x
	UPROPERTY(EditAnywhere)
	float m_turningSpeed;

private:
	//�J����
	UCameraComponent* m_pCameraComp;

	//�X�v�����O�A�[��
	USpringArmComponent* m_pSpringArmComp;
};
