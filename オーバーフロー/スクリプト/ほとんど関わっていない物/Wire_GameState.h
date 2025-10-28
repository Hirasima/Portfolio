// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Wire_GameState.generated.h"

//�v���C���[�̍��W��ݒ肷��֐��f���Q�[�g
DECLARE_DELEGATE_RetVal(FVector, FGetPlayerPosDelegate);

UCLASS()
class ORIGINALACTION_API AWire_GameState : public AGameState
{
	GENERATED_BODY()

public:
	//�v���C���[�̍��W�֌W
	void SetPlayerPosGetter(FGetPlayerPosDelegate _delegate);
	FVector GetPlayerPos()const;
	//�Ɩ��󋵂̎擾�֐�
	bool GetIslLight();
	//�Ɩ��󋵂̐ݒ�֐�
	void SetIsLight(bool _isPoint);
private:
	//�v���C���[�̍��W�擾�f���Q�[�g
	FGetPlayerPosDelegate GetPlayerPosDelegate;
	//���݂̏Ɩ���
	bool IsLight=true;
};
