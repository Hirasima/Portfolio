// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FighterCharacter.generated.h"

//�O���錾
class AWeapon;
class UCapsuleComponent;

//�U���N���ʒm�X�e�[�g
UENUM(BlueprintType)
enum class EAttackPattarnState : uint8
{
	None		UMETA(DisplayName = "None"),
	UpAttack	UMETA(DisplayName = "UpAttack"),
	DownAttack	UMETA(DisplayName = "DownAttack"),
	UpParry		UMETA(DisplayName = "UpParry"),
	DownParry	UMETA(DisplayName = "DownParry")
};

//��U�������A�N�V����
UENUM(BlueprintType)
enum class EAttackReaction : uint8
{
	None		UMETA(DisplayName = "None"),
	Hit			UMETA(DisplayName = "Hit"),
	Guard		UMETA(DisplayName = "Guard"),
	Parry		UMETA(DisplayName = "Parry")
};

//��낯�̑傫���񋓌^
UENUM(BlueprintType)
enum class EStagType : uint8
{
	None		UMETA(DisplayName = "None"),
	SmallStag	UMETA(DisplayName = "SmallStag"),
	BigStag		UMETA(DisplayName = "BigStag"),
	Down		UMETA(DisplayName = "Down")
};

//�U�����Ǘ�����\����
USTRUCT(BlueprintType)
struct FAttackAsset
{
	GENERATED_BODY()

public:

	//�A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimMantage;

	//�U����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int AttackPoint;

	//�m�b�N�o�b�N�̗�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KnockBackPower;

	//�K�[�h������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GuardBreakPower;

	//���݃^�C�v
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStagType StagType;

	//�ł��グ�U���\��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CanKnockUp;

	//�͈͍U���R���W�����̑��Έʒu
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector AreaCollisionRelativeLocation;

	//�͈͍U���R���W�����̑傫��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector AreaCollisionSize;

	//�R���X�g���N�^
	FAttackAsset(UAnimMontage* _animMontage = nullptr, float _attackPoint = 10, float _knockBackPower = 0.0f, EStagType _stagType = EStagType::None, bool _canKnockUp = false
		, FVector _collisionRelativeLocation = FVector::ZeroVector, FVector _collisionSize = FVector::ZeroVector)
	{
		AnimMantage = _animMontage;
		AttackPoint = _attackPoint;
		KnockBackPower = _knockBackPower;
		StagType = _stagType;
		CanKnockUp = _canKnockUp;
		AreaCollisionRelativeLocation = _collisionRelativeLocation;
		AreaCollisionSize = _collisionSize;
	}
};

UCLASS()
class ORIGINALACTION_API AFighterCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class UItemComponent;

public:
	// Sets default values for this character's properties
	AFighterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �I���W�i�����\�b�h //////////////////////////////////////////////////////////////////////

	//�U�����ꂽ�A�N�^�[�̃��X�g�̒��g������
	void ClearAttackedActors();

	//�G�t�F�N�g���o��
	virtual void SpawnEffect(EAttackReaction _attackReaction);

	//�m�b�N�o�b�N���X�V�֐�
	virtual void UpdateKnockBack(float DeltaTime);

	//�����^�[�W�����Đ�
	virtual void PlayAnimMantage(UAnimMontage* _animMontage);

protected:

	//��������������֐������V���M
	void FaceTargetLocation(const FVector _targetLocation);


	//---------------------------------------------------
	// �Q�b�^�[ �E�@�Z�b�^�[
	//---------------------------------------------------
public:

	//�U���̎�ނ̎擾
	virtual EAttackPattarnState GetAttackPattarnState();

	//���݂̏�ԂɑΉ�����AttackAsset��Ԃ�
	UFUNCTION(BlueprintCallable)
	virtual FAttackAsset GetAttackAsset();

	//�K�[�h���x�����͂�Ԃ�
	virtual int GetGuardBreakPower();

	//�ł��グ�U���\���Ԃ�
	virtual bool GetCanKnockUp();

	//���݂�HP������Ԃ�
	float GetHPRatio();

	//----------------------------------------------------
	// �C�x���g�֐�
	//----------------------------------------------------

public:

	//�U�����󂯂��Ƃ�
	virtual EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false);

	//��_���[�W��
	UFUNCTION(BlueprintCallable)
	virtual void Damaged(int _damagePoint);

	//��v���U����
	virtual void OnCriticalAttacked(int _damage);

	//��p���B��
	virtual void Parried();

	//��m�b�N�o�b�N��
	virtual void KnockBacked(FVector _direction, float _Time);

	//�U���q�b�g���̃C�x���g�֐�
	virtual void OnAttackHitEvent();

	//----------------------------------------------------
	//�Փ˔���֌W
	//----------------------------------------------------

public:

	//����̏d�Ȃ������̃C�x���g
	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);

public:
	//---------------------------------------------------
	// �v���p�e�B
	//---------------------------------------------------

	//����̃X�^�e�B�b�N���b�V��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> m_pWeaponClass;

	//��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> m_pSheathClass;

	//���s���x
	UPROPERTY(EditAnywhere)
	float m_walkSpeed;

	//�q�b�g�|�C���g�ő�l
	UPROPERTY(EditAnywhere)
	int m_Max_HitPoint;

	//�U����
	UPROPERTY(EditAnywhere)
	int m_attackPoint;

	//�v���\���H
	bool m_bEnableCritical;

	//�ł��グ���Ă��邩
	bool m_bIsKnockUpping;

	//-------------------------------------------------------
	// ���ʉ��n
	//-------------------------------------------------------

	//�U��SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* AttackSE;

	//��_���[�WSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* DamageSE;

	//���SSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* DeadSE;

	//�K�[�hSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* GuardSE;

	//�K�[�h����SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* JustGuardSE;

protected:

	//����������
	AWeapon* m_pWeapon;

	//��������
	AWeapon* m_pSheath;

	//�A�j���[�V�����C���X�^���X
	UAnimInstance* m_pAnimInstance;

	//��U���ς݂̃A�N�^�[
	TArray<AActor*> m_attackedActors;

	//HP�������
	int m_hitPoint;

	//�U���̓o�t�{��
	float m_AttackBuffRate;

	//Montage�Đ����x
	float m_montagePlayRate;

	//�m�b�N�o�b�N�p�����V���M
	FVector KnockBackStartPos;
	FVector KnockBackEndPos;
	float KnockBackDuration = 0.0f;
	float KnockBackElapsed = 0.0f;
	bool bIsKnockBack = false;

};
