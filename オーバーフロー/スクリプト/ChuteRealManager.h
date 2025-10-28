// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChuteRealManager.generated.h"

UCLASS()
class ORIGINALACTION_API AChuteRealManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChuteRealManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddDeadCount();

	void AddStageCount();

protected:
	//�X�e�[�W�O�`�U�p�V���O���X�|�i�[
	UPROPERTY(EditAnywhere)
	class AEnemySpawner* m_pFirstSpawner = nullptr;
	//�X�e�[�W7�`10�p�}���`�X�|�i�[
	UPROPERTY(EditAnywhere)
	class AEnemySpawner* m_pMultipleSpawner = nullptr;
	//���{�X�X�|�i�[
	UPROPERTY(EditAnywhere)
	class AEnemySpawner* m_pBossSpawner = nullptr;

	//�X�e�[�W1�`2�F�U���`���[�g���A��
	UPROPERTY(EditAnywhere,Category="Widget")
	TSubclassOf<class UChuteRealMiniWidget> ChuterealWindows;
	//��LWidget�\������
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_AttackChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_AttackChuterealWidget;

	UPROPERTY()
	UChuteRealMiniWidget* WindowRef = nullptr;

	//�X�e�[�W3�`4:�o�t����
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> BuffChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_BuffChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_BuffChuterealWidget;

	//�X�e�[�W5�`�U:�󂯃`���[�g���A��
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> GuardChuterealWindow;
	//��LWidget�\������
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_GuardChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_GuardChuterealWidget;

	//�X�e�[�W3�`4:�󒆃R���{����
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> AirAttackChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_AirAttackChuterealWidget;

	//�X�e�[�W5�`�U:�񕜃`���[�g���A��
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget>HealChuterealWindow;
	//��LWidget�\������
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_HealChuterealWidget;
	UPROPERTY(EditAnywhere)
	int SurviveTimeOf_HealChuterealWidget;

	//�X�e�[�W3�`4:�t���[����
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> FlowChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_FlowChuterealWidget;

	//�X�e�[�W3�`4:�p���B����
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UChuteRealMiniWidget> ParryChuterealWindows;
	UPROPERTY(EditAnywhere)
	int SpawnTimeOf_ParryChuterealWidget;

	//�Ō�ɏo�������pWidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* m_KeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> m_KeyWidgetClass;



	//�^�C�}�[
	float ChuterealTimer = 0.0f;
	bool m_IsTimerStop = false;

	float SpawnTimer = 0.0f;
	int DeadCount = 0;

	//�`���[�g���A���H��
	int stage = 0;

	bool IsSpawn = false;

};
