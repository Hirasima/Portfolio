// Fill out your copyright notice in the Description page of Project Settings.


#include "Wire_GameState.h"

//�v���C���[�̍��W�֌W
void AWire_GameState::SetPlayerPosGetter(FGetPlayerPosDelegate _delegate) {
	GetPlayerPosDelegate = _delegate;
}

//�v���C���[�̍��W��Ԃ��֐�
FVector AWire_GameState::GetPlayerPos() const{
	if (!GetPlayerPosDelegate.IsBound()) {
		UE_LOG(LogTemp, Warning, TEXT("Empty Delegate:GetPlayerPosDelegate In GameState_GetPlayerPos"));
		return FVector::Zero();
	}
	return GetPlayerPosDelegate.Execute();
}

//�Ɩ��󋵂̎擾�֐�
bool AWire_GameState::GetIslLight() {
	return IsLight;
}
//�Ɩ��󋵂̐ݒ�֐�
void AWire_GameState::SetIsLight(bool _isPoint) {
	IsLight = _isPoint;
}