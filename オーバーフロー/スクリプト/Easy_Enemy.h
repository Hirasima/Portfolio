// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Base.h"
#include "Easy_Enemy.generated.h"

/**
 * 
 */
 //�U���p�^�[���񋓌^
UENUM(BlueprintType)
enum class EAttackState : uint8
{
	Wait   UMETA(DisplayName = "Wait"),
	UpAttack   UMETA(DisplayName = "Up_Attack"),
	UnderAttack   UMETA(DisplayName = "Under_Attack"),
};

UCLASS()
class ORIGINALACTION_API AEasy_Enemy : public AEnemy_Base
{
	GENERATED_BODY()
public:
	//�U���A�j���[�V�����X�e�[�g�̎擾�֐�
	UFUNCTION(BlueprintCallable)
	EAttackState GetAttackState();

	//�U�����̃A�j���[�V���������^�[�W�����擾����֐�
	virtual UAnimMontage* GetAttackMontage()override;

	//�U���p�^�[���擾�֐�
	virtual EAttackPattarnState GetAttackPattarnState()override;

	//�U���I����
	virtual void OnFinishAttack()override;

	//�G���̏�ԂɑΉ�����AttackAsset��Ԃ����e�ڍs�\��
	FAttackAsset GetAttackAsset()override;

	//�A�^�b�N�A�Z�b�g���e�ڍs�\��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUnders;

protected:
	//�U���֐�
	virtual void Attack()override;

	//�G�l�~�[�̏�ԍX�V�֐�*�G�l�~�[�}�l�[�W������̂݌ĂԂ���*
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos)override;


protected:
	//�U�����
	EAttackState m_NowAttackState;

	//�`���[�g���A���n
	UPROPERTY(EditAnywhere,Category="Chutereal")
	bool m_isChutereal = false;
	UPROPERTY(EditAnywhere, Category = "Chutereal")
	TSubclassOf<class AChuteRealManager> ChuterealManagerClass;

	//�A�j���[�V���������^�[�W���Q//////////////////////////////////
	//��i�U��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpAttackMontage;
	//���i�U��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUnderAttackMontage;

	////////////////////////////////////////////////////////////////

	bool IsSendedDead = false;
	
};
