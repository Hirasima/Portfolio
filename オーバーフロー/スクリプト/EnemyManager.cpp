// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Wire_GameState.h"
#include "PlayPawn.h"
#include "Wire_GameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyManager::AEnemyManager():
	PlayerPos(FVector::Zero())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
}

//�G�l�~�[�̏����ݒ�����s����֐�
void AEnemyManager::InitAllEnemies() {
	//�G�l�~�[���ꊇ�o�^//////////////////////////////////////////////////////////////////////////////
	//���[���h���擾
	UWorld* World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: World--But EnemyManager BeginPlay Called"));
		return;
	}
	//���[���h���̃G�l�~�[�����ׂĎ擾
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AEnemy_Base::StaticClass(), FoundActors);
	//�G�l�~�[��o�^
	for (AActor* Actor : FoundActors) {
		AEnemy_Base* Enemy = Cast<AEnemy_Base>(Actor);
		if (Enemy) {
			AllEnemies.Add(Enemy);

			//�Ǘ��Ώۂ���������f���Q�[�g�̓o�^
			FUnregisterFromEnemyMng Delegate;
			Delegate.BindUObject(this, &AEnemyManager::UnregisterMngEnemy);
			Enemy->UnregisterFromEnemyMngCallBack = Delegate;
			//�o�g���}�l�[�W���ɓo�^����f���Q�[�g��o�^
			Enemy->RegisterToBattleMngCallBack = m_BattleManagerRegister;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
}

//�G�l�~�[��V�K�o�^����֐�
void AEnemyManager::AddNewEnemy(AEnemy_Base* _pEnemy) {
	if (_pEnemy) {
		if (AllEnemies.Contains(_pEnemy)) {
			return;
		}
		AllEnemies.Add(_pEnemy);

		//�Ǘ��Ώۂ���������f���Q�[�g�̓o�^
		FUnregisterFromEnemyMng Delegate;
		Delegate.BindUObject(this, &AEnemyManager::UnregisterMngEnemy);
		_pEnemy->UnregisterFromEnemyMngCallBack = Delegate;
		//�o�g���}�l�[�W���ɓo�^����f���Q�[�g��o�^
		_pEnemy->RegisterToBattleMngCallBack = m_BattleManagerRegister;
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�Q�[���X�e�[�g�̎擾
	AWire_GameState* G_S = Cast<AWire_GameState>(GetWorld()->GetGameState());

	//�v���C���[�̍��W���X�V
	PlayerPos = GetWorld()->GetGameState<AWire_GameState>()->GetPlayerPos();

	//�G�l�~�[�̍X�V����
	for (int i = AllEnemies.Num() - 1;i >= 0;--i) {
		if (!AllEnemies[i]) {
			return;
		}
		//�Ɩ��󋵂̓`�B
		AllEnemies[i]->SetIsBlind(!(G_S->GetIslLight()));
		//�X�V�֐�
		AllEnemies[i]->EnemyTick(DeltaTime, PlayerPos);
	}

}

//�o�g���}�l�[�W�����W�X�^�֐������炤�֐�
void AEnemyManager::SetBattleManagerRegister(FRegisterFromBattleMng _regis) {
	m_BattleManagerRegister = _regis;
}

//�Ǘ��Ώۂ���������֐�
void AEnemyManager::UnregisterMngEnemy(AEnemy_Base* _deadEnemy) {
	if (!AllEnemies.Contains(_deadEnemy)) {
		UE_LOG(LogTemp, Warning, TEXT("Irregular Unregist:AllEnemy Is Not Contain this Enemy"));
		return;
	}
	AllEnemies.Remove(_deadEnemy);
}

//�x����ԂɂȂ�������`�B����֐�
void AEnemyManager::BeAlart(bool _Alart,FVector _alartpos) {
	TArray<AEnemy_Base*> BattleEnemies;

	for (AEnemy_Base* Enemy : AllEnemies) {
		if (!Enemy) {
			continue;
		}
		(_Alart) ? Enemy->BeAlarm(_alartpos) : Enemy->EndAlarm();
	}
}

//���͈͓��̃G�l�~�[��Ԃ��֐�
TArray<AEnemy_Base*> AEnemyManager::GetBattleEnemies() {
	TArray<AEnemy_Base*> BattleEnemies;

	for (AEnemy_Base* Enemy : AllEnemies) {
		if (!Enemy) {
			continue;
		}
		//������S���Ԃ���̃N�b�\���ʂɏd��
		if (FVector::Dist(PlayerPos, Enemy->GetActorLocation()) > Enemy->GetViewLange()) {
			continue;
		}
		BattleEnemies.Add(Enemy);
	}
	return BattleEnemies;
}

//n�Ԗڂɋ߂��G�l�~�[��Ԃ��֐����o�g���}�l�[�W���Ɉڍs
AEnemy_Base* AEnemyManager::GetEnemyByDistNum(int _DistNum) {
	//�ł��߂��G�l�~�[�̋���
	float NearlestDist = 100000;
	//�ł��߂��G�l�~�[
	AEnemy_Base* NearlestEnemy = nullptr;

	//����������ʓ|������ŒZ��Ԃ�
	for (AEnemy_Base* Enemy : AllEnemies) {
		if (!Enemy) {
			continue;
		}

		if (FVector::Dist(PlayerPos, Enemy->GetActorLocation()) >= NearlestDist) {
			continue;
		}
		NearlestEnemy = Enemy;
	}
	return NearlestEnemy;
}

//�^�[�Q�b�g�G�l�~�[��Ԃ��֐� //��
//
//�T���������ɑ��݂��Ȃ��ꍇ��nullptr��Ԃ�
//�T���������ɑO�̃^�[�Q�b�g (_previousTarget) �������݂��Ȃ����_previousTarget��Ԃ�
AEnemy_Base* AEnemyManager::GetTargetEnemy(float _searchDistance, FVector _searcherRightVector, FVector _searcherPostion, float _nextTargetDirection, AEnemy_Base* _previousTarget)
{
	//�T���������̓G
	TArray<AEnemy_Base*> enemysWithinRange;

	//�T���������̓G�̋���
	TArray<float> enemysDistance;

	//�T���������̓G�̊O�ό���
	TArray<float> enemysClossResult;

	//�ł��T�������ɋ߂��G
	AEnemy_Base* returnEnemy = _previousTarget;

	//�E���ōł��߂��G
	AEnemy_Base* rightEnemy = nullptr;

	//�����ōł��߂��G
	AEnemy_Base* leftEnemy = nullptr;

	//�b��I���ʂ̓G�̋���
	float rightDistance = FLT_MAX;
	float leftDistance = FLT_MAX;

	//���`�T���ŒT���������̓G��T��
	for (int i = 0; i < AllEnemies.Num(); ++i)
	{
		//����i�Ԗڂ̓G���L���łȂ���Ύ��̃��[�v��
		if (!AllEnemies[i]) continue;

		//�T���҂��猩���G�̋���
		float dis = FVector::Dist(AllEnemies[i]->GetActorLocation(), _searcherPostion);

		//���G�������ɑ��݂��Ă����
		if (dis <= _searchDistance)
		{
			//�T���������̓G�ɒǉ�
			enemysWithinRange.Add(AllEnemies[i]);
			enemysDistance.Add(dis);

			//�N���X��(2D)�ō��E�𔻒肷��

			//�T���҂��猩���G�̈ʒu
			FVector location = AllEnemies[i]->GetActorLocation();

			//�N���X��
			float cross = FVector2D::CrossProduct(FVector2D(location.X, location.Y), FVector2D(_searcherPostion.X, _searcherPostion.Y));
			enemysClossResult.Add(cross);
		}
	}

	//�T���������̓G�����Ȃ����
	if (enemysWithinRange.Num() <= 0)
	{
		//���O���c���A�k����Ԃ��ďI��
		UE_LOG(LogTemp, Log, TEXT("EnemyManager : There Are No Enemies Within Search Distance"));
		return nullptr;
	}

	//���`�T���ŒT���������̓G�̒��ōł��߂��G��T��
	for (int i = 0; i < enemysWithinRange.Num(); ++i)
	{
		//�ȑO�̃^�[�Q�b�g�ł���ΒT������e�����̃��[�v��
		if (_previousTarget && enemysWithinRange[i] == _previousTarget) continue;

		//�������E���Ȃ�
		if (enemysClossResult[i] <= 0)
		{
			//���������߂����
			if (enemysDistance[i] < rightDistance)
			{
				//�b�茋�ʂ��X�V
				rightEnemy = enemysWithinRange[i];
				rightDistance = enemysDistance[i];
			}
		}
		//���������Ȃ�
		else
		{
			//���������߂����
			if (enemysDistance[i] < leftDistance)
			{
				//�b�茋�ʂ��X�V
				leftEnemy = enemysWithinRange[i];
				leftDistance = enemysDistance[i];
			}
		}
	}

	//���̃^�[�Q�b�g���������i�E���j�Ȃ�
	if (_nextTargetDirection >= 0)
	{
		//�E���̓G��D��
		if (rightEnemy)
		{
			returnEnemy = rightEnemy;
		}
		else
		{
			returnEnemy = leftEnemy;
		}
	}
	//���̃^�[�Q�b�g���������i�����j�Ȃ�
	else
	{
		//�����̓G��D��
		if (leftEnemy)
		{
			returnEnemy = leftEnemy;
		}
		else
		{
			returnEnemy = rightEnemy;
		}
	}

	//�ŏI�I�ɍł������ɋ߂����ʂ�Ԃ�
	return returnEnemy;
}

