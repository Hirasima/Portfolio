// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Base.h"
#include "GirlsBitWeapon.h"
#include "Girl_Boss.generated.h"

/**
 * 
 */

 //�U���񋓌^
UENUM(BlueprintType)
enum class EGirlBossAttackState : uint8
{
	Wait   UMETA(DisplayName = "Wait"),
	Montage   UMETA(DisplayName = "Montage")
};


//�\�P�b�g�Ƒҋ@�g�����X�t�H�[���̉�
USTRUCT(BlueprintType)
struct FHandleWaitInfo
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HandHandleName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform WaitTransform;
};

//�r�b�g�ƃ\�P�b�g�̉�
USTRUCT(BlueprintType)
struct FBitWeaponWithHandles
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGirlsBitWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHandleWaitInfo WaitHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHandleWaitInfo HandHandle;
};


UCLASS()
class ORIGINALACTION_API AGirl_Boss : public AEnemy_Base
{
	GENERATED_BODY()
	

public:

	virtual void BeginPlay()override;

	UFUNCTION(BlueprintCallable)
	EGirlBossAttackState GetGirlBossAnimState();
	UFUNCTION(BlueprintCallable)
	int GetStageLv();

	//����d�Ȃ�����
	virtual void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult)override;
	
	UFUNCTION(BlueprintCallable)
	void EndAnim();

	//�����^�]�p
	UFUNCTION(BlueprintCallable)
	void ReturnLHandWeaponToWait();
	//�����^�]�p
	UFUNCTION(BlueprintCallable)
	void ReturnRHandWeaponToWait();

	UFUNCTION(BlueprintCallable)
	void L_WeaponActive();
	UFUNCTION(BlueprintCallable)
	void R_WeaponActive();

	UFUNCTION(BlueprintCallable)
	void Call_R_Weapon();
	UFUNCTION(BlueprintCallable)
	void Call_L_Weapon();

	UFUNCTION(BlueprintCallable)
	void EndAttackMotion();
	UFUNCTION(BlueprintCallable)
	void EndAttackMotion2();

	//�r�b�g�z��
	UPROPERTY(EditAnywhere)
	TArray<FBitWeaponWithHandles> m_WeaponsInfo;

	UPROPERTY(EditAnywhere)
	UAnimMontage* m_ProtSlashMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* m_ProtDuoSlashMontage;


	void ForgetDamagedActor();

	virtual EAttackPattarnState GetAttackPattarnState()override;

protected:
	//Tick
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos)override;

	void SpawnCausionEffect(FVector _pos);

	//��Enemy_Base�ɈڐA��������D�V�X�e��
	// 
	// At<=0�Ŏ�D�ő喇���ɂȂ邩�A�ő�R�X�g�ɂȂ�܂Ńh���[
	// ���ݎ�D��0�Ԗڂ���؂��Ă���(���̃^�[���̍s���񐔂͕ۑ�)
	// ��D��0�ɂȂ�����At�����ɖ߂�
	// 
	//��D�I�o
	void DrewHand();
	//���ݎ�D(�Ή��ԍ�)
	TArray<int> Now_Hand;
	//���݃J�[�h�C���f�b�N�X(�R�D�T�O)(0,0,0,1,1,2�������ȃJ�[�h�ꗗ�̃C���f�b�N�X�Q)
	TArray<int> CardsDeck;

	//�f�b�L�ɏ��L���Ă���J�[�h�ꗗ(�C���f�b�N�X=�Ή��ԍ�,�v�f(����,�R�X�g))
	TArray<FIntPoint> CardsData;

	//���݂̍ő��D����
	int HandsMax=3;
	//���݂̍ő�R�X�g��
	int MaxHandsCosts=6;
	//���ݐ؂�����D����
	int NowUsedCardsNum=0;

	bool NowMotion = false;

	/////////////////////////////////////////

	//���[�h�`�F���W�̂��
	void StepUp0_1();
	void StepUp1_2();

	//�U��
	void Attack()override;

	//�U�����
	EGirlBossAttackState m_AttackState;

	//�����X�e�[�W(0��2)
	int StageLv;
	FVector m_firstPlayerPos;
	float Timer;
};
