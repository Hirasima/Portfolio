// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterCharacter.h"
#include "GameTickable.h"
#include "EnemyBase_V2.generated.h"

//Enemy�}�l�[�W���̔j��\��z��ւ̃��W�X�^�֐�
DECLARE_DELEGATE_OneParam(FRegisterToDestroyer, AEnemyBase_V2*);

//Enemy�̑�g�X�e�[�g�񋓌^
UENUM(BlueprintType)
enum class EEnemySituation_V2 : uint8
{
	Patrol   UMETA(DisplayName = "Patrol"),
	Battle   UMETA(DisplayName = "Battle"),
	Dead     UMETA(DisplayName = "Dead")
};

//Enemy�̃A�j���[�V�����X�e�[�g�񋓌^
UENUM(BlueprintType)
enum class EEnemyAnimState_V2 : uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	//Montage
	Action  UMETA(DisplayName = "Action"),
	Stag	UMETA(DisplayName = "Stag"),
	Dead    UMETA(DisplayName = "Dead"),
	//Sequence
	Walk	UMETA(DisplayName = "Walk"),
	Dash	UMETA(DisplayName = "Dash"),
	Wait	UMETA(DisplayName = "Wait")
};

UCLASS()
class ORIGINALACTION_API AEnemyBase_V2 : public AFighterCharacter , public IGameTickable
{
	GENERATED_BODY()
public:
	//�X�V�֐��Q//////////////////////////////////////////////////////////////////////////////////////

	//���ۂ�Destroy���s����+���Œ��O�g���K�[�̌Ăяo��(��Destroy()���܂܂�Ă���̂ŃK�`�Ŋ��ɌĂԂ��ƁI�I)
	virtual void ExecuteDestroy() override;
	//Tick()�N
	virtual void GameTick(float _deltaTime)override;
	//���̃t���[���̓���Destroy�����Ԃ�true�ɂ��邩�`�F�b�N
	virtual void CheckDestroy() override;

public:
	//�Q�b�^�[�E�Z�b�^�[////////////////////////////////////////////////////////////////////////////////

	//Enemy�}�l�[�W���̔j��\��z��ւ̃��W�X�^�R�[���o�b�N���Z�b�g����֐�
	void SetRegisterToDestroyerCallBack(FRegisterToDestroyer _callback);

	//���݂�AnimState���擾����֐�
	UFUNCTION(BlueprintCallable)
	EEnemyAnimState_V2 GetEnemyAnimState_V2();
	//���̃t���[����AnimState��ݒ肷��֐�
	UFUNCTION(BlueprintCallable)
	void SetAnimState_V2(EEnemyAnimState_V2 _state);

	//�v���C���[�̍��W��ݒ肷��֐�
	void SetPlayerPos(FVector _pos);
	//�ҋ@���W��ݒ肷��֐�
	void SetWaitPos(FVector _pos);
	//�ꎞ�I�Ȉړ��Ώۍ��W�i�A���[�����j��ݒ肷��֐�,��2����:true�Ȃ�ҋ@���W�ɖ߂�悤�Ɏw��
	void SetTargetAlartPos(FVector _pos,bool _resetFlag=false);

public:
	//�C�x���g�֐��E�C�x���g�ŌĂ΂��֐��ŏ�L�ȊO//////////////////////////////////////////////////////
	void AddNextFlameDamage(int _damage);

protected:
	//Tick���ŌĂԊ֐�//////////////////////////////////////////////////////////////////////////////////
	
	//�O�t���[������_���[�W��HP�ւ̔��f
	void ApplyHp();

	//��g�X�e�[�g�̕ύX�`�F�b�N/////////////////////////////////////////////////
	void DefineSituation(float _deltaTime);
	//�v���C���[�����F���Ă��邩�̊m�F
	bool CheckPlayerFound();


	//��g�X�e�[�g���Ƃ̕��򏈗�:Dead�͂�������return�Ȃ̂Ŗ���,��������AnimState���߂��Ⴈ��
	
	//�p�g���[��
	virtual void DoPatrol();

	//�o�g��///////////
	virtual void DoBattle();

	//���肳�ꂽNextAnimState��NowAnimState�ɔ��f+Next�̏�����
	void ApplyAnimState();

protected:
	//Enemy�}�l�[�W���̔j��\��z��ւ̃��W�X�^�R�[���o�b�N
	FRegisterToDestroyer m_RegisterToDestroyerCallBack;

protected:
	//UPROPERTY()�Őݒ肷��n�ϐ�//////////////////////////////////////////////////////////////////
	
	//���F�͈�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float m_ViewLength;

	//���b�v���C���[������������A�邩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle_Param")
	float m_KeepBattleTime;

protected:
	//���݂̃X�e�[�g////////////////////////////////////////////////////////////////////////////////
	//���݂̑�g�X�e�[�g
	EEnemySituation_V2 m_NowSituation;
	//���݂�AnimState
	EEnemyAnimState_V2 m_NowAnimState;

	//���̃t���[���p�X�e�[�g/////////////////////////////////////////////////////////////////////////
	
	//���̃t���[����AnimState
	EEnemyAnimState_V2 m_NextAnimState;
	 
	//���̃t���[���̓���Destroy���邩
	bool m_IsNextDestroy = false;

	//���̃t���[���̗\�葍��_���[�W
	int m_NextFlameDamage;

	//�C�x���g�ɂ��\��///////////////////////////////////////////////////////////////////////
	//���̃t���[���̏I����Destroy�ҋ@�ɂ��邩
	bool m_IsEventNextDestroy = false;

	//�����Ă�ϐ�////////////////////////////////////////////////////////////////////////////

	//�v���C���[�̍��W
	FVector m_PlayerPos;
	//�ҋ@���W
	FVector m_WaitPos;
	//�ꎞ�x���Ώۍ��W
	FVector m_AlartPos;

	//�ꎞ�x����Ԃ��ۂ�
	bool m_IsAlart;
	//����A�j���[�V�����X�e�[�g��e����
	bool m_IsBlockingEqualAnimState;

	//�v���C���[����풆�Ɍ��������p������
	float m_LostPlayerTime;
};
