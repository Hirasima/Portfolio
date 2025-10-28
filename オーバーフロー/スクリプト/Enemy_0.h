// Fill out your copyright notice in the Description page of Project Settings.
//�������{�X�̂炵��

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Base.h"
#include "Enemy_0.generated.h"

//�N���X�̑O���錾
class UAnimMontage;
class UNiagaraComponent;
class UBossHpBarWidget;

//�A�j���[�V�����X�e�[�g�񋓌^�������͂��ꂩ��U���p�^�[���݂̂ɂȂ�
UENUM(BlueprintType)
enum class EAttackAnimState : uint8
{
	Wait   UMETA(DisplayName = "Wait"),
	Dash   UMETA(DisplayName = "Dash"),
	BackStep   UMETA(DisplayName = "BackStep"),
	UpAttack   UMETA(DisplayName = "Up_Attack"),
	UnderAttack   UMETA(DisplayName = "Under_Attack"),
	CombAttack   UMETA(DisplayName = "CombAttack"),
	UpGuard  UMETA(DisplayName = "Up_Guard"),
	UnderGuard   UMETA(DisplayName = "Down_Guard"),
	Parry	UMETA(DisplayName = "Parry"),
	Parried UMETA(DisplayName = "Parried"),
	Damage	UMETA(DisplayName = "Damage"),
	Dead   UMETA(DisplayName = "Dead")
};

UCLASS()
class ORIGINALACTION_API AEnemy_0 : public AEnemy_Base
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	AEnemy_0();

	//�G���̏�ԂɑΉ�����AttackAsset��Ԃ����e�ڍs�\��
	FAttackAsset GetAttackAsset()override;

	//�U�����󂯂��Ƃ��ɌĂ΂��֐�
	virtual EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false)override;
	
protected:
	virtual void BeginPlay() override;

	//�t���[�̑����`�B�ׂ̈ɃI�[�o�[���C�h
	virtual void EnemyTick(float _DeltaTime, FVector _PlayerPos)override;

	//�G�l�~�[�̃A�j���[�V�����X�e�[�g�X�V
	virtual void EnemyBattleAnimTick()override;

	//�A�j���[�V�����X�e�[�g�擾�֐�
	UFUNCTION(BlueprintCallable)
	EAttackAnimState GetAttackAnimState();

	//�A�j���[�V���������^�[�W�����擾����֐�
	virtual UAnimMontage* GetAttackMontage()override;

	//�U���I����
	void OnFinishAttack()override;

	//�U���̎�ނ̎擾
	virtual EAttackPattarnState GetAttackPattarnState()override;

	//�U�����֐������Ԃ����Ă�����̏�����Tick���ɂ��Ă�����
	virtual void Attack()override;

	//�U���A�j���[�V�����X�e�[�g
	EAttackAnimState m_AttackAnimState = EAttackAnimState::Wait;

	//�A�^�b�N�A�Z�b�g���e�ڍs�\��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUnders;

	//�{�X�̃Q�[�W��Widget�̎���
	UPROPERTY(VisibleInstanceOnly)
	UBossHpBarWidget* m_BossGageWidget;

private:
	//�R���{�����e�ڍs�\��
	int m_combo;
	int m_DefaultMaxMp;
	bool IsViewGage = false;
public:
	//�A�j���[�V���������^�[�W���Q//////////////////////////////////
	//��i�U��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUpAttackMontage;
	//���i�U��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pUnderAttackMontage;
	//�K�E�Z�������p���u��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pCombAttack;
	
	
	////////////////////////////////////////////////////////////////

	//---------------------------------------------------
	// �G�t�F�N�g�֌W
	//---------------------------------------------------

	////////////////////////////////////////////////////////////////

	//�{�X�Q�[�W�֌W
	//�v���C���[�̃Q�[�W��Widget�̃N���X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> BossGageWidgetClass;


	//�`���[�g���A���n
	UPROPERTY(EditAnywhere, Category = "Chutereal")
	bool m_isChutereal = false;
	UPROPERTY(EditAnywhere, Category = "Chutereal")
	TSubclassOf<class AChuteRealManager> ChuterealManagerClass;
	bool IsSendedDead = false;
};
