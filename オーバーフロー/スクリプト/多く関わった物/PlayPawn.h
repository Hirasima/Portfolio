// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "FighterCharacter.h"
#include "InputActionValue.h"
#include "Enemy_Base.h"
#include "PlayPawn.generated.h"

class AWeapon;
class UCameraComponent;
class USpringArmComponent;
class UItemComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class AWire_HUD;
class UNiagaraSystem;
class AInteractionActor;
class UNiagaraComponent;
class AAttackActor;
class AHpExpressionActor;
class ABuffBitManager;
class UBoxComponent;

//�A�j���[�V������ԗ񋓌^
UENUM(BlueprintType)
enum class EPlayerAnimState : uint8
{
	  Idle				UMETA(DisplayName = "Idle")
	, Run				UMETA(DisplayName = "Run")
	, Damage			UMETA(DisplayName = "Damage")
	, Dead				UMETA(DisplayName = "Dead")
	, Guard				UMETA(DisplayName = "Guard")
	, Parry_Up			UMETA(DisplayName = "Parry_Up")
	, Avoidance			UMETA(DisplayName = "Avoidance")
	, CriticalAttack    UMETA(DisplayName = "CriticalAttack")
	, Attack_Up			UMETA(DisplayName = "Attack_Up")
	, Attack_Under		UMETA(DisplayName = "Attack_Under")
	, Attack_Parry		UMETA(DisplayName = "Attack_Parry")
	, Stag				UMETA(DisplayName = "Stag")
	, Fall_Out			UMETA(DisplayName = "Fall_Out")
	, Fall_GlassBreak	UMETA(DisplayName = "FallGlassBreak")
	, MontaInteract		UMETA(DisplayName = "Interact")
	, Recovery			UMETA(DisplayName = "Recovery")
};

//�s����ԗ񋓌^
UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	  Idle				UMETA(DisplayName = "Idle")
	, Ready				UMETA(DisplayName = "Ready")
	, Damage			UMETA(DisplayName = "Damage")
	, Dead				UMETA(DisplayName = "Dead")
	, Attack_Up			UMETA(DisplayName = "Attack_Up")
	, Attack_Under		UMETA(DisplayName = "Attack_Under")
	, Attack_Parry		UMETA(DisplayName = "Attack_Parry")
	, Guard				UMETA(DisplayName = "Guard")
	, Parry				UMETA(DisplayName = "Parry")
	, Avoidance			UMETA(DisplayName = "Avoidance")
	, CriticalAttack    UMETA(DisplayName = "CriticalAttack")
	, Stag				UMETA(DisplayName = "Stag")
	, Recovery			UMETA(DisplayName = "Recovery")
};

//�o�t���ʗ񋓌^
UENUM(BlueprintType)
enum class EBuffPattern : uint8
{
	None					UMETA(DisplayName = "None"),
	SpeedUp				UMETA(DisplayName = "SpeedUp"),
	AdditionalAttack	UMETA(DisplayName = "AdditionalAttack"),
};

//�U���n�ߒʒm�p�f���Q�[�g
DECLARE_DELEGATE_FiveParams(
FReceivePlayerAttackScence,
EAttackPattarnState,
FVector,
FVector,
float,
float
);

//�^�[�Q�b�g�G�l�~�[���󂯎�邽�߂̃f���Q�[�g
DECLARE_DELEGATE_RetVal_FiveParams(AEnemy_Base*, FGetTargetEnemy, float, FVector, FVector, float, AEnemy_Base*);

UCLASS()
class ORIGINALACTION_API APlayPawn : public AFighterCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�I���W�i�����\�b�h

	//�G�t�F�N�g���o��
	virtual void SpawnEffect(EAttackReaction _attackReaction)override;

	//�U���A�N�^�[���o��
	void SpawnAttackActor(int _attackPoint, int _attackNum);

	//�o�t�̒ǉ�
	void AddBuff(EBuffPattern _newBuff);

	//�o�t�̃��Z�b�g
	void ResetBuff();

	//�����^�[�W�����Đ�
	void PlayAnimMantage(UAnimMontage* _animMonatage)override;

	//�J�����p��O��t�ɂ���֐�
	void ReverceCamYaw();

	//�q�b�g�X�g�b�v������֐�
	void DoHitStop();

	//�J�����h�ꎞ
	void DoCameraShaking(float _shakeAmount = 20.0f);

	//��̍U���L����
	UFUNCTION(BlueprintCallable)
	void IsActiveSheathAttack(bool _enable);

	//������Ԃ���ݒ�
	UFUNCTION(BlueprintCallable)
	void SetIsDrawWeapon(bool _isDraw);

	//�͈͍U��
	void DoAreaAttack(FVector _location, FRotator _rotation, FVector _collisiionSize);

	//�͈͍U���I������
	void FinishAreaAttack();

	//---------------------------------------------------
	// �Q�b�^�[ �E�@�Z�b�^�[
	//---------------------------------------------------

	//�J�����R���|�[�l���g�̎擾
	UCameraComponent* GetCameraComponent();

	//�X�v�����O�A�[���R���|�[�l���g�̎擾
	USpringArmComponent* GetSpringArmComponent();

	//������Ԃ��擾
	UFUNCTION(BlueprintCallable)
	EPlayerAnimState GetPlayerAnimState();
	//�ݒ�
	void SetPlayerAnimState(EPlayerAnimState _state);

	//�v���C���[��ԂɑΉ�����AttackAsset��Ԃ�
	FAttackAsset GetAttackAsset()override;

	//HUD��o�^����֐�
	void SetHUDtoPlayer(AWire_HUD* _hud);

	//�^�[�Q�b�g�G�l�~�[�擾
	AEnemy_Base* GetTargetEnemy(float _searchDistance, FVector _searchAngle, FVector _searcherPostion, float _nextTargetDirection = 0.0f, AEnemy_Base* _previousTarget = nullptr);

	//�U���̎�ނ̎擾
	EAttackPattarnState GetAttackPattarnState()override;

	//�X���E��Ԃɂ���֐������V���M
	void BeSlow(float _slowTime,float _slowPower);

	//�A�j���[�V���������^�[�W���̍Đ����x���擾
	UFUNCTION(BlueprintCallable)
	float GetAnimMantegePlayRate();

	//�C���^���N�g�����^�[�W���ݒ�֐������V���M
	void SetInteractMontage(UAnimMontage* _targetMontage);

	//�ł��グ�U���\���Ԃ�
	bool GetCanKnockUp()override;

	//�J�����̈ʒu���X�V���邩��ݒ�
	UFUNCTION(BlueprintCallable)
	void SetDoUpdateCameraLocation(bool _do);

	//------------------------------------------------------------
	//�@�f���Q�[�g�ݒ�p
	//------------------------------------------------------------

	//�\�����쑗�M�f���Q�[�g�̐ݒ�֐�
	void SetAttackScenceReceiver(FReceivePlayerAttackScence _delegate);

	//�^�[�Q�b�g�G�l�~�[�擾�p�f���Q�[�g�̐ݒ�֐�
	void SetTargetEnemyDelegate(FGetTargetEnemy _delegate);

	//---------------------------------------------------
	//�X�V�����Q
	//---------------------------------------------------

	//�J�����̊p�x�X�V
	void UpdateCameraAngle(float _DeltaTime);

	//�J�����̈ړ�����
	void UpdateCameraMove(float _DeltaTime);

	//�ړ��̍X�V
	void UpdateMoving();

	//�^�[�Q�b�g�}�[�J�[�̍X�V
	void UpdateTargetMarker();

	//�o�t�̍X�V
	void UpdateBuffs(float _deltaTime);

	//�K�[�h���x�̍X�V
	void UpdateGuardPower(float _deltaTime);

	//��������̍X�V
	void UpdateAvoid(float _deltaTime);

	//���S�����̍X�V
	void UpdateDead(float _deltaTime);

	//���͂̍X�V
	void UpdateInput(float _deltaTime);

	//----------------------------------------------------
	// �C�x���g�֐�
	//----------------------------------------------------

	//�U��
	UFUNCTION(BlueprintCallable)
	void OnAttack();

	//�U���I����
	UFUNCTION(BlueprintCallable)
	void OnFinishAttack();

	//�e��
	UFUNCTION(BlueprintCallable)
	void OnParry();

	//�e���I����
	UFUNCTION(BlueprintCallable)
	void OnFinishParry();

	//�e���U����
	UFUNCTION(BlueprintCallable)
	void OnParryAttack();

	//�e���U���I����
	UFUNCTION(BlueprintCallable)
	void OnFinishParryAttack();

	//�����
	UFUNCTION(BlueprintCallable)
	void OnAvoid();

	//����I����
	UFUNCTION(BlueprintCallable)
	void OnFinishAvoid();

	//�v���_���[�W��
	UFUNCTION(BlueprintCallable)
	void OnCriticalDamage();

	//�v���I����
	UFUNCTION(BlueprintCallable)
	void OnFinishCriticalAttack();

	UFUNCTION(BlueprintCallable)
	//�A�j���[�V���������^�[�W���I����
	void OnFinishAnimMontage();

	//�C���^���N�g�L����
	UFUNCTION(BlueprintCallable)
	void ActivationInteractor();

	//�񕜎�
	UFUNCTION(BlueprintCallable)
	void OnRecovery();

	//���S��
	UFUNCTION(BlueprintCallable)
	void OnDead();

	//���X�|�[����
	UFUNCTION(BlueprintCallable)
	void OnRespawn();

	//�U���q�b�g���̃C�x���g�֐�
	void OnAttackHitEvent()override;

	//��U����
	EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false)override;

	//��_���[�W��
	void Damaged(int _damagePoint)override;

	//��e��
	void Parried()override;

	//��i�U�����͎�
	void OnInputAttackUp();

	//���i�U�����͎�
	void OnInputAttackUnder();

	//�p���B�U�����͎�
	void OnInputParryAttack();

	//��i�h����͎�
	void OnInputGuard();

	//----------------------------------------------------
	//���͊֌W
	//----------------------------------------------------

	//���̓o�C���h�֐�
public:

	//�ړ�����
	void InputMoveDirection(const FInputActionValue& _Value);

	//�J��������
	void InputCameraAngle(const FInputActionValue& _Value);

	//�J�������͏I��
	void InputCameraAngleFin(const FInputActionValue& _Value);

	//��i�U������
	void InputAttackUp(const FInputActionValue& _Value);

	//���i�U������
	void InputAttackUnder(const FInputActionValue& _Value);

	//�h�����
	void InputGuard(const FInputActionValue& _Value);

	//�h����͏I��
	void InputGuardFin(const FInputActionValue& _Value);

	//��i�e������
	void InputParryUp(const FInputActionValue& _Value);

	//������
	void InputAvoidance(const FInputActionValue& _Value);

	//���b�N�I������
	void InputLockOn(const FInputActionValue& _Value);

	//�J�����^�[�Q�b�g�؂�ւ�����
	void InputCameraShift(const FInputActionValue& _Value);

	//�J�����^�[�Q�b�g�؂�ւ����͏I��
	void InputCameraShiftFin(const FInputActionValue& _Value);

	//�񕜓��͎�
	void InputRecovery(const FInputActionValue& _Value);

	//�V�t�g����
	void InputShift(const FInputActionValue& _Value);

	//�V�t�g���͏I��
	void InputShiftFin(const FInputActionValue& _Value);

	//�C���^���N�V�������͎������V�ǋL
	void InputInteraction(const FInputActionValue& _Value);
	void InputInteractionOrigin();


	//----------------------------------------------------
	//�Փ˔���֌W
	//----------------------------------------------------

	public:

	//����̏d�Ȃ������̃C�x���g
	void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult) override;
	
	//�U���\������̃C�x���g
	void ReceiveAttackScence(EAttackPattarnState _pattarn, FVector _PlayerPos, FVector _playerFrontVec, float _AttackRange, float _AttackDeg);

	UFUNCTION()
	//�C���^���N�V�����I�u�W�F�N�g�Ƃ̏d�Ȃ�̌��m�p
	void OnBeginOverlap(
		UPrimitiveComponent* _overlappedComponent,
		AActor* _otherActor,
		UPrimitiveComponent* _otherComp, 
		int32 _otherBodyIndex,
		bool _bFromSweep, 
		const FHitResult& _sweepResult);

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* _overlappedComponent,
		AActor* _otherActor,
		UPrimitiveComponent* _otherComp,
		int32 _otherBodyIndex);


	// �v���p�e�B ////////////////////////////////////////////////////////////////////////////
public:

	//�J�����̏�������p
	UPROPERTY(EditDefaultsOnly)
	float DefaultCameraFOV;

	//�X�v�����O�A�[�����[�g�R���|�[�l���g����̏����ʒu
	UPROPERTY(EditDefaultsOnly)
	FVector SpringArmOffset;

	//�X�v�����O�A�[���̒���
	UPROPERTY(EditDefaultsOnly)
	float DefaultSpringArmLength;

	//�J�����̃s�b�`���
	UPROPERTY(EditAnywhere)
	float CameraPitchMax;

	//�J�����̃s�b�`����
	UPROPERTY(EditAnywhere)
	float CameraPitchMin;

	//���b�N�I������Pitch
	UPROPERTY(EditAnywhere)
	float LockOnPitch;

	//�^�[�Q�b�g���̐��`��Ԃ̕�ԗ�
	UPROPERTY(EditAnywhere)
	float LockOnLerpAlpha;
	
	//�J�������Z�b�g�̑��x
	UPROPERTY(EditAnywhere)
	float CameraResetSpeed;

	//���b�N�I������
	UPROPERTY(EditAnywhere)
	float LockOnDistance;

	//���b�N�I�����̈ړ����x�{��
	UPROPERTY(EditAnywhere)
	float LockOnMoveSpeedCorrectionValue;

	//pc�ł̐��񑬓x
	UPROPERTY(EditAnywhere)
	float PCTurningSpeed;

	//�J������]�����x
	UPROPERTY(EditAnywhere)
	float CameraRotationAcceleration;

	//�J������]�����x
	UPROPERTY(EditAnywhere)
	float CameraRotationDeceleration;

	//�J������]���x���
	UPROPERTY(EditAnywhere)
	float CameraRotationSpeedMax;

	//����̈ړ����x
	UPROPERTY(EditAnywhere)
	float AvoidanceMoveSpeed;

	//�o�t�ő吔
	UPROPERTY(EditDefaultsOnly)
	int BuffRimitNum;

	//���i�U���ɂ��o�t�̌p������
	UPROPERTY(EditAnywhere)
	float BuffDurationByUnderAttack;

	//��i�U���ɂ��o�t�p�����Ԃ̑����{��
	UPROPERTY(EditAnywhere)
	float BuffDurationRateByUpAttack;

	//�o�t�̌p�����Ԍ�����~����
	UPROPERTY(EditAnywhere)
	float BuffDurationStopTime;

	//�R���{�o�t�ő吔
	UPROPERTY(EditAnywhere)
	int ComboBuffRimitNum;

	//�R���{�o�t�{��
	UPROPERTY(EditAnywhere)
	float ComboBuffRate;

	//�U�����x�o�t�̔{��
	UPROPERTY(EditAnywhere)
	float SpeedUpRate;

	//�K�[�h�̋����̍ő�l
	UPROPERTY(EditAnywhere)
	int GuardPowerMax;

	//�K�[�h�̋����̉񕜑��x
	UPROPERTY(EditAnywhere)
	float GuardPowerIncreasePerSecond;

	//�s�����[�h
	UPROPERTY(EditAnywhere)
	bool IsUndeadMode;

	//���S�Ɋ|���鎞��
	UPROPERTY(EditAnywhere)
	float TimeTakenToDead;

	//���X�|�[���n�_
	UPROPERTY(EditAnywhere)
	FVector RespawnPoint;

	//�U���R���W�������B����
	UPROPERTY(EditAnywhere)
	bool AttackCollisionHiddenInGame;

	//�^�[�Q�b�g�}�[�J�[
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TargetMarkerWidgetClass;

	//�U���q�b�g���A�b�v�p�̃A�N�^�[
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAttackActor> AttackActorClass;

	//HP�\���p�A�N�^�[�N���X
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHpExpressionActor> HpExpressionActorClass;

	//�o�t�r�b�g�Ǘ��N���X
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuffBitManager> BuffBitManagerClass;

	//�_���[�W���o�pPostProcessmaterial
	UPROPERTY(EditAnywhere)
	UMaterialInterface* m_pDamagePostMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic* DamageOverlayInstance;

	UPROPERTY(EditAnywhere)
	float DamageOverlaySowTime;

private:
	//�J�����R���|�[�l���g
	UPROPERTY(VisibleInstanceOnly)
	UCameraComponent* CameraComp;

	//�J�����̎���p
	float m_cameraFOV;

	//�J������]���x
	float m_cameraRotationSpeed;

	//�J�����̊p�x���Z�b�g����
	bool m_bResetingCamera;

	//�J�����̑��Έʒu
	FVector m_cameraRelativeLocation;

	//�J�����̈ʒu�X�V�����邩
	bool m_bDoUpdateCameraLocation;

	//�X�v�����O�A�[��
	UPROPERTY(VisibleInstanceOnly)
	USpringArmComponent* SpringArmComp;

	//�X�v�����O�A�[���̒���
	float m_springArmLength;

	//����i�C�A�K���R���|�[�l���g
	UPROPERTY(VisibleInstanceOnly)
	UNiagaraComponent* AvoidEffectComp;

	//����s���p�R���W����
	UPROPERTY(VisibleInstanceOnly)
	UBoxComponent* AvoidCollision;

	//����J�n���ʒu
	FVector AvoidBeginPosition;

	//�A�C�e���R���|�[�l���g
	UPROPERTY(VisibleInstanceOnly)
	UItemComponent* ItemComp;

	//�E�B�W�F�b�g�R���|�[�l���g
	UPROPERTY(VisibleInstanceOnly)
	UWidgetComponent* WidgetComp;

	//HUD�̃|�C���^�[
	UPROPERTY()
	AWire_HUD* HUD;

	//�o�t
	UPROPERTY(EditInstanceOnly)
	TArray<EBuffPattern> Buffs;

	//�U���̑��x
	UPROPERTY(VisibleInstanceOnly)
	float AttackSpeed;

	//HP�\���p�A�N�^�[
	UPROPERTY(VisibleAnywhere)
	AHpExpressionActor* HpExpressionActor;

	//�o�t�r�b�g�}�l�[�W���[
	UPROPERTY(VisibleInstanceOnly)
	ABuffBitManager* BuffBitManager;

	//���݂̃A�j���[�V�������
	EPlayerAnimState m_ePlayerAnimState;

	//���݂̍s�����
	EPlayerActionState m_ePlayerActionState;

	//�f�t�H���g�̕��s���x
	float m_defaultMaxWalkSpeed;

	//�o�t�̌p������
	float m_buffAditionalTimer;

	//�o�t�̒�~����
	float m_buffLifeTimer;

	//�o�t�U���ς݂�
	bool m_bDoBuffAttack;

	//�R���{�o�t��
	int m_comboBuffNum;

	//���G��ԃt���O
	bool m_bInvincible;

	//�U�����J�E���^�[
	int m_attackCounter;

	//�K�[�h�̋��x���Z�p
	float GuardPowerFloat;

	//���̖h��s���Ńo�t���l���ł�����
	bool IsSuccesiedGetBuff;

	//���S���t�F�[�h�A�E�g�p��
	float m_alphaOfDeadFade;

	//���S�o�ߎ���
	float m_deathDuration;

	//�󒆍U������
	bool m_bDoFly;

	//�e���U���o���邩
	bool m_bCanParryAttack;

	//�e���U�����͂����邩
	bool m_bIsInputParryAttack;

	//�v���U���Ώ�
	UPROPERTY()
	AFighterCharacter* m_pCriticalTarget;

	//�ł��グ��ꂽ�G
	UPROPERTY()
	TArray<AFighterCharacter*> m_pKnockCaughtUpEnemies;

	//�͈͍U���p
	UPROPERTY()
	UBoxComponent* m_pAreaAttackCollisition;

	//�e�����A�N�^�[
	UPROPERTY()
	AFighterCharacter* m_pParriedFighter;

	//�K�[�h�̋��x
	UPROPERTY(VisibleInstanceOnly)
	int GuardPower;

	//�q�b�g�X�g�b�v�O�̎��Ԃ̐i�ޑ��x
	float m_dilationTimeBeforeHitStop;

	//------------------------------------------------
	//�A�j���[�V�����֌W
	//------------------------------------------------

public:

	//�����^�[�W��

	//�e��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* ParryMontage;

	//�e���U��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset ParryAttack;

	//���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AvoidanceMontage;

	//��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* RecoveryMontage;

	//�K�[�h�u���C�N
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* GuardBreakMontage;

	//���S
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DeadMontage;

	//�A�^�b�N�A�Z�b�g

	//��i
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUps;

	//���i
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUnders;

	//��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackAirs;

	//�󒆋�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset AttackStrongAirs;

	//�v��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset CriticalAttack;

private:

	//�C���^���N�g�A�j���[�V����
	UPROPERTY()
	UAnimMontage* m_InteractTargetMontage;

	// �f���Q�[�g ///////////////////////////////////////////

	//�o�g���}�l�[�W���ɗ\�������`�B����f���Q�[�g
	FReceivePlayerAttackScence m_AttackScenceDeleGate;

	//�^�[�Q�b�g�G�l�~�[���擾����f���Q�[�g
	FGetTargetEnemy m_GetTargetEnemyDeleGate;

	//----------------------------------------------------
	//���͊֌W
	//----------------------------------------------------

public:

	//�C���v�b�g�A�N�V����

	//�ړ��A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_moveAction;

	//�J�����A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_cameraMoveAction;

	//�U���A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_attackUpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_attackUnderAction;

	//�h��A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_guardAction;

	//��i�e���A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_parryUpAction;

	//����A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_avoidanceAction;

	// ���b�N�I���A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_LockOnAction;

	//�J�����^�[�Q�b�g�؂�ւ��A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_CameraShiftAction;

	//�V�t�g�A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ShiftAction;

	//�C���^���N�g�A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Interaction;

	//�񕜃A�N�V����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Recovery;


	//---------------------------------------------------
	// �G�t�F�N�g�֌W
	//---------------------------------------------------

public:
	//�_���[�W�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* DamageEffect;

	//�K�[�h�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* GuardEffect;

	//�p���B�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryEffect;

	//�p���B�U�������G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryAttackActivationEffect;

	//�p���B�U���G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryAttackEffect;

	//����G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* AvoidEffect;

	//-------------------------------------------------------
	// ���ʉ��n
	//-------------------------------------------------------

public:

	//SE����
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	float SE_Volume;

	//���SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* AvoidSE;

	//��𐬌�SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* JustAvoidSE;

	//�K�[�h�u���C�NSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* GuardBreakSE;

	//�e��SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* ParrySE;

	//�e������SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* JustParrySE;

	//�e���U������SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* ParryAttackInputSE;

	//�e���U��SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* ParryAttackSE;

	//��SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* RecoverySE;

	//�o�t����SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* BuffResetSE;

	//���X�|�[��SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* RespawnSE;

private:

	//���̓o�C���h�p�ϐ�////////////////////////////////////////

	//���݂̃J�����p�x(FVector2D)
	FVector2D m_cameraAngle;

	//���݂̈ړ�����(FVector2D)
	FVector2D m_moveDirection;

	//���b�N�I������
	bool m_bLockOn;

	//�^�[�Q�b�g�؂�ւ��\��
	bool m_bCameraShift;

	float m_cameraShiftDire;

	//�V�t�g���
	bool m_bShiftOn;

	//etc

	//�^�[�Q�b�g���̃t�@�C�^�[
	AEnemy_Base* m_pTargetEnemy;

	//�X���E��Ԃ��I����֐������V���M
	void EndSlow();
	//��ԕϐ�
	//�X���E���ۂ������V���M
	bool m_IsSlow;
	//�X���E�^�C�}�[�Ǘ��p�^�C���n���h���[�����V���M
	FTimerHandle TimeSlowTimerHandle;

	//���ݏd�Ȃ��Ă���C���^���N�g�Ώ�
	UPROPERTY()
	AInteractionActor* m_nowTargetInteractor;

	//�i�_���[�W���ɌĂԁj�O���b�W�|�X�g�v���Z�X���o��������֐�
	void ShowDamageOverlay();

	UPROPERTY()
	TArray<AEnemy_Base*> m_AirTargetEnemys;
};
