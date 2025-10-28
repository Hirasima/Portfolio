// Fill out your copyright notice in the Description page of Project Settings.


#include "Wire_GameMode.h"
#include"EnemyManager.h"
#include"BattleManager.h"
#include "PlayPawn.h"
#include "Wire_GameState.h"
#include "Wire_HUD.h"
#include "Kismet/GameplayStatics.h"

//�R���X�g���N�^
AWire_GameMode::AWire_GameMode():m_EnemyManager(nullptr),m_BattleManager(nullptr) {


}
//�Q�[���J�n������
void AWire_GameMode::BeginPlay() {
	Super::BeginPlay();

	//���[���h���擾
	UWorld* World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp,Warning, TEXT("Not Found: World--But GameMode BeginPlay Called"));
		return;
	}
	//�G�l�~�[�}�l�[�W���̍���
	m_EnemyManager = World->SpawnActor<AEnemyManager>(AEnemyManager::StaticClass());
	//�o�g���}�l�[�W���̍���
	m_BattleManager = World->SpawnActor<ABattleManager>(ABattleManager::StaticClass());

	//�o�g���}�l�[�W���ɃG�l�~�[�}�l�[�W���̎Q�Ƃ�n��
	m_BattleManager->SetEnemyManagerRef(m_EnemyManager);

	//�v���C���[Pawn���擾�iSuper::BeginPlay�Ő����ς݁j////////////////////////////////////////////////////////////////
	APawn* P_Pawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!P_Pawn) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: PlayerPawn--But GameMode BegnPlay Called"));
		return;
	}
	APlayPawn* Player = Cast<APlayPawn>(P_Pawn);
	if (!Player) {
		UE_LOG(LogTemp, Warning, TEXT("Faild Cast: PlayerPawn to APlayPawn"));
		return;
	}

	//HUD���擾
	AHUD* hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	if (!hud) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HUD--But GameMode BegnPlay Called"));
		return;
	}
	AWire_HUD* WireHud = Cast<AWire_HUD>(hud);
	if (!WireHud) {
		UE_LOG(LogTemp, Warning, TEXT("Faild Cast: HUD to AWire_HUD"));
		return;
	}
	//�v���C���[��HUD��`�B
	Player->SetHUDtoPlayer(WireHud);

	//�o�g���}�l�[�W���̃v���C���[�\������T�m�֐����v���C���[�ɓ`�B
	FReceivePlayerAttackScence ScenceDelegate;
	ScenceDelegate.BindUObject(m_BattleManager, &ABattleManager::SetPlayerAttackScence);
	Player->SetAttackScenceReceiver(ScenceDelegate);

	//�o�g���}�l�[�W���̃^�[�Q�b�g�G�l�~�[�擾�֐����v���C���[�ɓ`�B
	FGetTargetEnemy TargetDelegate;
	TargetDelegate.BindUObject(m_EnemyManager, &AEnemyManager::GetTargetEnemy);
	Player->SetTargetEnemyDelegate(TargetDelegate);

	//�G�l�~�[�}�l�[�W���Ƀo�g���}�l�[�W���ɓo�^����֐���`�B��
	FRegisterFromBattleMng RegistDelegate;
	RegistDelegate.BindUObject(m_BattleManager, &ABattleManager::AddEnemyForRegister);
	m_EnemyManager->SetBattleManagerRegister(RegistDelegate);

	//�G�l�~�[��������
	m_EnemyManager->InitAllEnemies();

	//�Q�[���X�e�[�g�Ƀv���C���[�̍��W�擾�֐����f���Q�[�g�Ƃ��ēo�^/////////////////////////////////////////////////
	//�Q�[���X�e�[�g�̎擾
	AWire_GameState* G_S = GetWorld()->GetGameState<AWire_GameState>();
	if (!G_S) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: GameMode--But GameMode BegnPlay Called"));
		return;
	}
	//�f���Q�[�g�Ƀv���C���[���W�擾�֐����o�C���h
	FGetPlayerPosDelegate PLDelegate;
	PLDelegate.BindUObject(Player, &APawn::GetActorLocation);

	//�Q�[���X�e�[�g�ɓo�^
	G_S->SetPlayerPosGetter(PLDelegate);
}


//�G�l�~�[�}�l�[�W����Ԃ��֐�
AEnemyManager* AWire_GameMode::GetEnemyManager() {
	if (!m_EnemyManager) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: EnemyManager--But GetEnemyManager Called"));
		return nullptr;
	}

	return m_EnemyManager;
}
//�o�g���}�l�[�W����Ԃ��֐�
ABattleManager* AWire_GameMode::GetBattleManager() {
	if (!m_BattleManager) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: BattleManager--But GetBattleManager Called"));
		return nullptr;
	}

	return m_BattleManager;
}

