// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ORIGINALACTION_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//�I���W�i�����\�b�h-------------------------------------------------

public:

	//�R���W�����̗L�����ݒ�
	UFUNCTION(BlueprintCallable)
	void SetCollisionActive(bool _enable);

	//����̋O���̗L���ݒ�
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitActive(bool _enable);

	//����̋O���̐F�̐ݒ�
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitColor(FLinearColor _linearColor);

	//����̋O���̕��̐ݒ�
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitWidth(float _width);

	//����̃G�t�F�N�g�̃p�����[�^�ݒ�
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitEffectParam(float _spawnRate, float _size);

	//����̍U����Ԑݒ�p
	UFUNCTION(BlueprintCallable)
	void SetAttacking(bool _enabl);

	//�v���~�e�B�u�R���|�[�l���g�̎擾
	UFUNCTION(BlueprintCallable)
	UPrimitiveComponent* GetPrimitiveComponent();

	void ChangeColor(FLinearColor _color, bool _point = false, int _materialIndex = 2);

	bool GetIsPoint();
	void SetIsPoint(bool _point,int _index=2);

	UFUNCTION(BlueprintCallable)
	void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);

//�I���W�i���v���p�e�B*----------------------------------------------

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_pStaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* m_pNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neon")
	FLinearColor Color = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neon")
	int m_NeonMatNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* m_pBladeOrbit;

protected:

	bool m_bAttacking;

	bool m_IsPoint;

};
