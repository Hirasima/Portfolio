// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayPawn.h"
#include "BuffBitManager.generated.h"

class ABuffBitActor;
class USpringArmComponent;

UCLASS()
class ORIGINALACTION_API ABuffBitManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuffBitManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�I���W�i�����\�b�h///////////////////////////////////

public:

	//������
	void Init(USkeletalMeshComponent* _parentMesh = nullptr);

	//�o�t�r�b�g�̈ړ�����
	void UpdateBitMove();

	//�o�t�̏�Ԑݒ�
	void SetBuffs(TArray<EBuffPattern>& _buffs);

	//�r�b�g�̓_�ŏ�Ԑ؂�ւ�
	void SetIsPoint(bool _isPoint);

	//�_�ŏ�Ԃ��擾
	bool GetIsPoint();

	//�r�b�g�̒ǌ�
	void Attack(AActor* _actor);

	//�v���p�e�B//////////////////////////////////////////

public:
	//�r�b�g�N���X
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuffBitActor> BuffBitActorClass;

protected:

	//�r�b�g�̓��ꕨ
	UPROPERTY(VisibleInstanceOnly)
	TArray<ABuffBitActor*> m_pBuffBitActors;

	USkeletalMeshComponent* m_pParentMeshComp;

	//�_�ŏ�Ԃ�
	bool IsPoint;

};
