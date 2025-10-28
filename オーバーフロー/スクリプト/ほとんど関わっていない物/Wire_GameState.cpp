// Fill out your copyright notice in the Description page of Project Settings.


#include "Wire_GameState.h"

//プレイヤーの座標関係
void AWire_GameState::SetPlayerPosGetter(FGetPlayerPosDelegate _delegate) {
	GetPlayerPosDelegate = _delegate;
}

//プレイヤーの座標を返す関数
FVector AWire_GameState::GetPlayerPos() const{
	if (!GetPlayerPosDelegate.IsBound()) {
		UE_LOG(LogTemp, Warning, TEXT("Empty Delegate:GetPlayerPosDelegate In GameState_GetPlayerPos"));
		return FVector::Zero();
	}
	return GetPlayerPosDelegate.Execute();
}

//照明状況の取得関数
bool AWire_GameState::GetIslLight() {
	return IsLight;
}
//照明状況の設定関数
void AWire_GameState::SetIsLight(bool _isPoint) {
	IsLight = _isPoint;
}