// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "FighterCharacter.h"
#include "Enemy_Base.generated.h"

//�N���X�̑O���錾
class AEnemyManager;
class ABattleManager;
class UCapsuleComponent;
class AWeapon;
class UWidgetComponent;
class UNiagaraSystem;


//��ԗ񋓌^
UENUM(BlueprintType)
enum class EEnemySituationState : uint8
{
	Patrol   UMETA(DisplayName = "Patrol"),
	Battle   UMETA(DisplayName = "Battle"),
	Dead     UMETA(DisplayName = "Dead")
};

//��ԗ񋓌^
UENUM(BlueprintType)
enum class EEnemyMotionState : uint8
{
	Wait			UMETA(DisplayName = "Wait"),
	Dash			UMETA(DisplayName = "Dash"),
	UpGuard			UMETA(DisplayName = "UpGuard"),
	DownGuard		UMETA(DisplayName = "DownGuard"),
	Parried			UMETA(DisplayName = "Parried"),
	Damaged			UMETA(DisplayName = "Damaged"),
	Attack			UMETA(DisplayName = "Attack"),
	Stag			UMETA(DisplayName = "Stag"),
	Assacinated		UMETA(DisplayName = "Assacinated"),
	Dead			UMETA(DisplayName = "Dead")
};

//�G�l�~�[�}�l�[�W������̃A�����W�X�^�֐�
DECLARE_DELEGATE_OneParam(FUnregisterFromEnemyMng, AEnemy_Base*);
//�o�g���}�l�[�W������̃A�����W�X�^�֐�
DECLARE_DELEGATE_OneParam(FUnregisterFromBattleMng, AEnemy_Base*);
//�o�g���}�l�[�W���ւ̓o�^�֐�
DECLARE_DELEGATE_OneParam(FRegisterFromBattleMng, AEnemy_Base*);
//�o�g���}�l�[�W���Ƀq�b�g�X�g�b�v�ʒm������֐�
DECLARE_DELEGATE(FCallOfHitStop);


UCLASS()
class ORIGINALACTION_API AEnemy_Base : public AFighterCharacter
{
	GENERATED_BODY()

	//�e�}�l�[�W���N���X�̃t�����h������K�͉��̏ꍇ�C���^�[�t�F�[�X���p�ɐ؂�ւ�
	friend class AEnemyManager;
	friend class ABattleManager;

public:
	// Sets default values for this pawn's properties
	AEnemy_Base();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//�A�j���[�V�����X�e�[�g�擾�֐�
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetNowTargetMontage();

	//�U�����ɃA�j���[�V���������^�[�W�����擾����֐�
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetAttackMontage();

	//��낯���ɃA�j���[�V���������^�[�W�����擾����֐�
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetStagMontage();

	//���݂̃��[�V�����󋵂��擾����֐�
	UFUNCTION(BlueprintCallable)
	virtual EEnemyMotionState GetNowMotionState()const;

	//���݂̃��[�V�����󋵂�ݒ肷��֐�
	UFUNCTION(BlueprintCallable)
	void SetNowMotionState(EEnemyMotionState _state);

	//�m�b�N�o�b�N���I��������֐�
	UFUNCTION(BlueprintCallable)
	void KnockBackEnd();

	//�m�b�N�A�b�v���I��������֐�
	UFUNCTION(BlueprintCallable)
	void KnockUpEnd();

	//�m�b�N�o�b�N����֐�
	void KnockBackStart(FVector _powerPos, float _power);

	//�K�[�h���J�n����֐�:�I����AttackFinish���Ăׂ΂����̂Ŋȗ���
	UFUNCTION(BlueprintCallable)
	void GuardStart();

	//�K�����J�n
	UFUNCTION(BlueprintCallable)
	void LockOnStart();

	//�U�����ꂽ�A�N�^�[�̃��X�g�̒��g������
	UFUNCTION(BlueprintCallable)
	virtual void ClearAttackedActors();

	//�G�l�~�[�̏�ԍX�V�֐�*�G�l�~�[�}�l�[�W������̂݌ĂԂ���*
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos);
	//�G�l�~�[�̃A�j���[�V�����X�e�[�g�X�V
	virtual void EnemyBattleAnimTick();
	//�G�l�~�[�̐퓬�X�V�֐�*�o�g���}�l�[�W������̂݌ĂԂ���*
	virtual void EnemyBattleTick(float _DeltaTime, FVector _PlayerPos);


	//�G�l�~�[�̏󋵂�ݒ肷��֐�
	void SetEnemySituation(EEnemySituationState _situ);
	//�G�l�~�[�̗\�m���Ă���U����ݒ肷��֐�*�o�g���}�l�[�W������̂݌Ăяo��
	UFUNCTION(BlueprintCallable)
	void SetScenceOfPlayerAttack(EAttackPattarnState _pattarn);

	//�G�l�~�[�}�l�[�W������̃A�����W�X�^�֐��R�[���o�b�N*�G�l�~�[�}�l�[�W������̂ݕύX���鎖*
	FUnregisterFromEnemyMng UnregisterFromEnemyMngCallBack;
	//�o�g���}�l�[�W������̃A�����W�X�^�֐��R�[���o�b�N*�o�g���}�l�[�W������̂ݕύX���鎖*
	FUnregisterFromBattleMng UnregisterFromBattleMngCallBack;
	//�o�g���}�l�[�W���ւ̓o�^�֐��R�[���o�b�N*�G�l�~�[�}�l�[�W������̂ݕύX���鎖*
	FRegisterFromBattleMng RegisterToBattleMngCallBack;
	//�o�g���}�l�[�W���Ƀq�b�g�X�g�b�v�ʒm������֐�*�o�g���}�l�[�W���݂̂���ύX���邱��
	FCallOfHitStop CallOfHitStopCallBack;

	//�U�����֐������Ԃ����Ă�����̏�����Tick���ɂ��Ă�����
	virtual void Attack();

	//���񒆁i��퓬���j�̋���
	virtual void Patrol();

	//�x����Ԃ̃I���I�t�֐�
	virtual void BeAlarm(FVector _alartPos);
	virtual void EndAlarm();

	//���E�Ƀv���C���[�����邩�`�F�b�N
	virtual bool PlayerFindCheck();

	//�o�g���}�l�[�W�����痣�E����֐�(�F�o�g���}�l�[�W������Ă΂�邱�Ƃ͂���)
	virtual void CheckOutBattleManager(FVector _PlayerPos);

	////�퓬���̈ړ�����
	virtual void InBattleMove();

	virtual void SpawnEffect(EAttackReaction _attackReaction)override;

	//�v���C���[�̕��������֐�
	void LookforPlayer();

	//��HP�o�[�Z�p���ؗp
	UFUNCTION(BlueprintImplementableEvent,Category="DebugFunc")
	void ScreenHpBar();

	//��v���U����
	virtual void OnCriticalAttacked(int _damage)override;

	//�t���[����̎��Ƀg���K�[����֐�
	virtual void BreakingFlowTrriger();

	//�t���[�̍X�V�֐�
	virtual void UpdateFlow(float _deltaTime);

protected:
	//�v���C���[�̍��W
	FVector m_PlayerPos;

	//���݂̃p�����[�^�[�ϐ�
	int Mp;
	float At;
	//���݂̏��
	EEnemySituationState m_Situation;
	//���݂̂�낯���
	EStagType m_StagState;
	//���ݗ\�m���Ă���v���C���[�̍U��
	EAttackPattarnState m_NowScenceOfPlayerAttack;

	//�K�[�h�p������
	float m_guardTime;

	//�e����p������
	float m_parriedTime;

	//�e����Ă��邩
	bool m_bParried;

	//���������Ă��邩
	bool m_IsBlind = false;

	//�x����Ԃł��邩
	bool m_IsAlart = false;

	//������i�����ʒu�j
	FTransform m_firstPos;
	//�x���ʒu
	FVector m_alartPos;
	//�x���ʒu�ɂ������ۂ�
	bool m_isAlartPoin;

	//���݂̃��[�V������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	EEnemyMotionState m_MotionState;

	//��
	UPROPERTY(EditAnywhere)
	UStaticMesh* m_pFaceMesh;

	//�x�������^�C�}�[
	FTimerHandle AlartEndTimerHandle;

	//�m�b�N�o�b�N�̎��Ɉꎞ�ۑ�����p���[�u�����g�f�[�^
	int m_Def_GroundFriction;
	int m_Def_BrakingFrictionFactor;
	int m_Def_BrakingDecelWalking;
	bool m_Def_bOrientRotationToMovement;

	bool bKnockBack = false;

	//�t���[�֌W�^�C�}�[
	float FlowTimer = 0;

	//���U�����W�b�N�ύX�̂��߂̕ϐ�
	TArray<int> m_nowHandIndexies;
	int m_nowHand;

	//�ł������邩�ۂ�
	bool m_CanKnockUp = true;

	//�R���{����AttackAsset�V�X�e���ڍs�ϐ�
	int m_combo;

	//�v���C���[����������U�����
	bool m_LookPlayer = false;

	

public:	

	//�U��
	UFUNCTION(BlueprintCallable)
	virtual void OnAttack();

	//�U���I����
	UFUNCTION(BlueprintCallable)
	virtual void OnFinishAttack();

	//�\���֐�
	UFUNCTION(BlueprintCallable)
	virtual void SpawnAttackScenceEffect();

	//���S�A�j���[�V�����I�����ɌĂԂ�i���������j
	UFUNCTION(BlueprintCallable)
	virtual void DeleteOut();

	//�U�����󂯂��Ƃ��ɌĂ΂��֐�
	virtual EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false)override;

	//��_���[�W��
	virtual void Damaged(int _damagePoint)override;

	//�͂����ꂽ���̌Ă΂��֐�
	virtual void Parried()override;

	//�ҋ@���W��ݒ肷��֐�
	void SetWaitPosition(FTransform _targetPos);

	//HP�擾�֐�
	int GetEnemyHp();

	//�Ɩ��󋵂̓`�B�֐�
	void SetIsBlind(bool _isBlind);

	//���E�͈͂��擾����֐�
	float GetViewLange();
	

	//�p�����[�^�ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	bool m_IsUseGuard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	bool m_IsUseFlow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	int Max_Mp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float Max_At;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float GuardGein_Mp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float ParriedGein_Mp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	bool m_IsAttackLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AngerParam")
	float UpperAnger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AngerParam")
	float UnderAnger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_turningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_NearestRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_BattleSearchRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	float m_BlindBattleSearchRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move_Param")
	bool m_IsAnotherFirstPos=false;

public:
	//��{�A�j���[�V���������^�[�W���Q///////////////////////////////////////
	//��i�K�[�h
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpGuardMontage;
	//���i�K�[�h
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUnderGuardMontage;
	//��e��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpParriedMontage;
	//��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pDeadMontage;

	//��낯�Q
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pMiniStagMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pBigStagMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pKnockUpMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pKnockUpKeepMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pAssacinatedMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset GuardCounterAttack;
	//////////////////////////////////////////////////////////////////////
	

	//�_���[�W�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* DamageEffect;

	//�K�[�h�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* GuardEffect;
	//��p���B����e�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParriedDamageEffect;

	//�p���B�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryEffect;

	//�t���[�I�[���i���j�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* FlowAura;

	//�U���\���G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* AttackScenceEffect;

	//�K�[�h�̍ő厞��
	UPROPERTY(EditAnywhere)
	float m_guardMaxTime;

	//�͂�����ő厞��
	UPROPERTY(EditAnywhere)
	float m_parriedMaxTime;

	//�t���[��Ԃ��ۂ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
	bool m_IsFlow;
	//�t���[�{��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
	float m_FlowSpeed;
	//�t���[�u���C�N�p������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow")
	float m_FlowBreakedTime=5.0f;

	//���U�����W�b�N�ύX�̂��߂̕ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeckState")
	int m_MaxCosts;

	//�m�b�N�A�b�v���x���L�[�v���邩
	bool m_bIsKeepZAxis = false;
	FVector KeepPos;

	//-------------------------------------------------------
	// ���ʉ��n
	//-------------------------------------------------------

public:

	//SE����
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	float SE_Volume;

	//�t���[�I��SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* FlowBreakSE;
};
