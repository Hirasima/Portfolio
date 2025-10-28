// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Wire_GameState.generated.h"

//プレイヤーの座標を設定する関数デリゲート
DECLARE_DELEGATE_RetVal(FVector, FGetPlayerPosDelegate);

UCLASS()
class ORIGINALACTION_API AWire_GameState : public AGameState
{
	GENERATED_BODY()

public:
	//プレイヤーの座標関係
	void SetPlayerPosGetter(FGetPlayerPosDelegate _delegate);
	FVector GetPlayerPos()const;
	//照明状況の取得関数
	bool GetIslLight();
	//照明状況の設定関数
	void SetIsLight(bool _isPoint);
private:
	//プレイヤーの座標取得デリゲート
	FGetPlayerPosDelegate GetPlayerPosDelegate;
	//現在の照明状況
	bool IsLight=true;
};
