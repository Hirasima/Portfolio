// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

//前方宣言
class AFighterCharacter;
class UNiagaraSystem;
class AWire_HUD;

//アイテムの種類列挙型
UENUM(BlueprintType)
enum class EItemKinds : uint8
{
	HpRecovery	UMETA(DisplayName = "HpRecovery")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORIGINALACTION_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// オリジナルメソッド //////////////////////////////////////////

	//アイテムを使用する際に呼ばれる関数
	void UseItem(EItemKinds _itemKinds);

	//HP回復処理
	void RecoveryHp();

	//HUDに伝達する
	void CommunicateToHUD();

	// プロパティ //////////////////////////////////////////////////

public:

	//HP回復量
	UPROPERTY(EditAnywhere)
	int HPRecoveryAmount;

	//HP回復アイテム最大数
	UPROPERTY(EditAnywhere)
	int HPRecoveryItemNumMax;

	//HP回復アイテム再使用時間
	UPROPERTY(EditAnywhere)
	float HpRecoveryItemRecastTime;

	//HUD
	UPROPERTY()
	AWire_HUD* HUD;

	//親ファイターキャラクター
	UPROPERTY()
	AFighterCharacter* m_pParentFighter;
private:

	//HP回復アイテム数
	UPROPERTY(VisibleAnywhere)
	int m_hpRecoveryItemNum;

	//HP回復アイテム再使用タイマー
	UPROPERTY(VisibleAnywhere)
	float m_hpRecoveryItemRecastTimer;

	//---------------------------
	// エフェクト関係
	//---------------------------
public:

	//HP回復時のエフェクト
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HPRecoveryEffect;

};
