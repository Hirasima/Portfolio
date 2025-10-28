// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//�����X�N�͂��邪�o�tenum���p�̂��߃v���C�|�[�����C���N���[�h
#include "PlayPawn.h"
#include "BuffBitActor.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ORIGINALACTION_API ABuffBitActor : public AActor
{
	GENERATED_BODY()

	friend class ABuffBitManager;
	
public:	
	// Sets default values for this actor's properties
	ABuffBitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�o�t��`�B�����֐�
	void SetBuffType(EBuffPattern _bufftype);

	//�����̓G�ɍU������֐�
	void AttackTargetActor(AActor* _actor);

	//�\�P�b�g���ݒ�
	void SetSocketName(FName _idolSocket, FName _activeSocket);

	//�e�X�P���^�����b�V���ݒ�
	void SetParentSkeltalMeshComp(USkeletalMeshComponent* _skeltalMeshComp);

	//�_�Ő؂�ւ��ݒ�
	void SetBitIsPoint(bool _anable);

	//�ړ��X�V
	void UpdateMove(float DeltaTime);

	//�U������
	void UpdateAttack(float DeltaTime);

	//�v���p�e�B
	//���b�V��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_pStaticMeshComp;
	//�o�t�F
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffColor")
	FLinearColor AttackBuff_Color=FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffColor")
	FLinearColor SpeedBuff_Color = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffColor")
	FLinearColor NoneBuff_Color=FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* m_pCircleModeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* m_pAttackModeMesh;

	UPROPERTY()
	EBuffPattern m_eBitState;

	//�U���ʒu�̔��a
	UPROPERTY(EditDefaultsOnly)
	float AttackRadius;

	//�U���ʒu�̊p�x����
	UPROPERTY(EditDefaultsOnly)
	FRotator AttackMinRotation;
	UPROPERTY(EditDefaultsOnly)
	FRotator AttackMaxRotation;

protected:
	UPROPERTY()
	UMaterialInterface* m_pMaterial;

	UPROPERTY()
	UNiagaraComponent* m_pBeamComponent;

	UPROPERTY(VisibleInstanceOnly)
	UNiagaraComponent* m_pOrbitComp;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* m_pBitBeam;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* m_pBitAwake;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* m_pBitOrbit;


	//�A�N�e�B�u���Ƒҋ@���̃\�P�b�g��
	FName m_activeSocketName;
	FName m_idolSocketName;

	//�e�̃��b�V���R���|�[�l���g
	UPROPERTY()
	USkeletalMeshComponent* m_pParentSkeltalMeshComp;

	//�r�b�g�̑��݂��Ă鎞��
	float timer;

	//�U������
	bool m_bIsAttacking;

	//�U�����̃g�����X�t�H�[��
	FVector m_attackPos;
	FRotator m_attackRot;
	
	//�U���Ώ�
	UPROPERTY()
	AActor* m_pAttckActor;
	//�U������
	float attackTimer;
	//�U���ς݂��ǂ���
	bool m_bAttacked;

	//�_�ŏ��
	bool IsPoint;
};
