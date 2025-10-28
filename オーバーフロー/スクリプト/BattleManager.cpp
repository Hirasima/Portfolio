// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include "EnemyManager.h"
#include "Wire_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ABattleManager::ABattleManager():Ref_EnemyManager(nullptr), m_pTargetEnemy(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////��풆�G�l�~�[�̓o�^�֐�
	//if (!Ref_EnemyManager) {
	//	UE_LOG(LogTemp, Warning, TEXT("Not Found: Ref_EnemyManager in BattleManager Tick()"));
	//	return;
	//}
	//SetBattleEnemies();

}

//�G�l�~�[�}�l�[�W�����̎Q�Ƃ�ݒ肷��֐�
void ABattleManager::SetEnemyManagerRef(AEnemyManager* _eMng) {
	if (!_eMng) {
		UE_LOG(LogTemp, Warning, TEXT("Irregular Ref:EnemyManager Ref To Set BattleManager"));
	}
	Ref_EnemyManager = _eMng;
}

//�v���C���[�̍U���̋N�������m����֐���D
void ABattleManager::SetPlayerAttackScence(EAttackPattarnState _pattarn,FVector _PlayerPos,FVector _playerFrontVec, float _AttackRange, float _AttackDeg) {
	for (AEnemy_Base* Enemy : BattleEnemies) {
		if (!Enemy) {
			continue;
		}
		//�˒���������
		FVector ToEnemy = (Enemy->GetActorLocation()) - _PlayerPos;
		if (ToEnemy.Size() > _AttackRange) {
			continue;
		}
		//�p�x��������
		float Dot = FVector::DotProduct(_playerFrontVec.GetSafeNormal(), ToEnemy.GetSafeNormal());
		float HalfCosAngle = FMath::Cos(FMath::DegreesToRadians(_AttackDeg * 0.5f));
		if (Dot < HalfCosAngle) {
			continue;
		}

		//�˒����Ȃ̂ōU���\�������ʒm
		Enemy->SetScenceOfPlayerAttack(_pattarn);
	}

	//�q�b�g�X�g�b�v��e�̂��߃X���E��������Z�b�g
	IsNowSlow = false;
}

//�^�[�Q�b�g�ԍ���ύX����֐���D
void ABattleManager::ChangeTarget(int _targetNum) {
	TargetEnemyNum = _targetNum;
	//�^�[�Q�b�g�G�l�~�[��ύX
	SetTargetEnemy();
}

//�Ǘ��ΏۂƂ��ēo�^����֐���D
void ABattleManager::AddEnemyForRegister(AEnemy_Base* _enemy) {
	if (!_enemy) {
		return;
	}
	if (BattleEnemies.Contains(_enemy)) {
		return;
	}

	//�o�^
	BattleEnemies.Add(_enemy);
	//�ΏۂɃA�����W�X�^�[�p�f���Q�[�g��ݒ�
	FUnregisterFromBattleMng Delegate;
	Delegate.BindUObject(this, &ABattleManager::UnregisterBattleEnemy);
	//���t�����h����؂藣���Ȃ�Z�b�g�֐��ɐ؂�ւ�
	_enemy->UnregisterFromBattleMngCallBack = Delegate;
	//�q�b�g�X�g�b�v�`�B�p�R�[���o�b�N��ݒ�
	FCallOfHitStop Calldelegate;
	Calldelegate.BindUObject(this, &ABattleManager::CallOfHitStop);
	_enemy->CallOfHitStopCallBack = Calldelegate;
	//UE_LOG(LogTemp, Warning, TEXT("HitStopCallBackSat!"));

	//�G�l�~�[�̏󋵂�퓬�ɕύX
	_enemy->SetEnemySituation(EEnemySituationState::Battle);

}

//�G�l�~�[�}�l�[�W���Ɍ�풆�̃G�l�~�[�����Ȃ����m�F����X�V�֐����c��������ł���
void ABattleManager::SetBattleEnemies() {
	if (!Ref_EnemyManager) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: Ref_EnemyManager in SetBattleEnemies"));
		return;
	}

	//�Ǘ��ΏۂƂ��ēo�^
	TArray<AEnemy_Base*> Enemies=Ref_EnemyManager->GetBattleEnemies();
	for (AEnemy_Base* Enemy : Enemies) {
		if (Enemy && !BattleEnemies.Contains(Enemy)) {
			BattleEnemies.Add(Enemy);
			//�ΏۂɃA�����W�X�^�[�p�f���Q�[�g��ݒ�
			FUnregisterFromBattleMng Delegate;
			Delegate.BindUObject(this, &ABattleManager::UnregisterBattleEnemy);
			//���t�����h����؂藣���Ȃ�Z�b�g�֐��ɐ؂�ւ�
			Enemy->UnregisterFromBattleMngCallBack = Delegate;
			//�G�l�~�[�̏󋵂�퓬�ɕύX
			Enemy->SetEnemySituation(EEnemySituationState::Battle);
		}
	}
}


//�^�[�Q�b�g�G�l�~�[��ݒ肷��֐�
void ABattleManager::SetTargetEnemy() {
	//�ł��߂��G�l�~�[�̋���
	float NearlestDist =FLT_MAX;
	//�ł��߂��G�l�~�[
	AEnemy_Base* NearlestEnemy = nullptr;

	//����
	if (BattleEnemies.Num() <= 0)
	{
		m_pTargetEnemy = nullptr;
		return;
	}

	//����������ʓ|������ŒZ��Ԃ�
	for (AEnemy_Base* Enemy : BattleEnemies) {
		if (!Enemy) {
			continue;
		}
		
		m_pTargetEnemy = Enemy;
	}
}

//�^�[�Q�b�g�G�l�~�[��Ԃ��֐�
AEnemy_Base* ABattleManager::GetTargetEnemy() {
	SetTargetEnemy();
	if (!m_pTargetEnemy) {
		return nullptr;
	}
	return m_pTargetEnemy;
}

//���X�g����ΏۃG�l�~�[���������֐�
void ABattleManager::UnregisterBattleEnemy(AEnemy_Base* _deadEnemy) {
	if (!BattleEnemies.Contains(_deadEnemy)) {
		UE_LOG(LogTemp, Warning, TEXT("Irregular Unregist:BattleEnemy Is Not Contain this Enemy"));
		return;
	}
	BattleEnemies.Remove(_deadEnemy);
}

//�G�l�~�[����̃q�b�g�X�g�b�v�ʒm����������֐�
void ABattleManager::CallOfHitStop() {
	if (IsNowSlow) {
		return;
	}
	IsNowSlow = true;

	//���c�����ŃX���E���[�V��������������//

	float dilationTime = 0.0001;
	float stopTime = 0.15;

	//���݂̃��x�����擾
	UWorld* world = GetWorld();

	//���Ԃ��~�߂�
	UGameplayStatics::SetGlobalTimeDilation(world, dilationTime);


	//�^�C�}�[�n���h��
	FTimerHandle TimerHandle;

	//�^�C�}�[�Z�b�g
	//�q�b�g�X�g�b�v�I�����Ɍ��̎��Ԃ̐i�ޑ��x�ɖ߂�
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Hit Stop End"));
		},
		stopTime * dilationTime,
		false
	);
}




