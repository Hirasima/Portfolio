// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "ElectroPawl.generated.h"

//クラスの前方宣言
class ANeonManager;
class UBoxComponent;

UCLASS()
class ORIGINALACTION_API AElectroPawl : public AInteractionActor
{
	GENERATED_BODY()
	
public:
	//コンストラクタ
	AElectroPawl();

	//インタラクション時呼ばれる関数
	virtual void UseInteraction()override;
	
protected:
	//初期化
	virtual void BeginPlay()override;

	//色変化見る用
	virtual void OnConstruction(const FTransform& Transform)override;

	//ネオンをつけたり消したりする関数
	void SwitchNeon();
	//ネオンを警告色で復旧する関数
	void BePointAlertNeon();

	//自身の色を変える関数
	void SetMyColor(FLinearColor _color);
protected:
	//自身の基本色
	UPROPERTY(EditAnywhere, Category = "Neon")
	int NeonMatNum;

	//自身の基本色
	UPROPERTY(EditAnywhere, Category = "Neon")
	FLinearColor Color = FLinearColor::Red;

	//ターゲットのネオンマネージャ★BeginPlayとかで取得した方がよさげ
	UPROPERTY(EditAnywhere, Category = "Neon")
	float m_BlindTime;

	//ターゲットのネオンマネージャ★BeginPlayとかで取得した方がよさげ
	UPROPERTY(EditAnywhere, Category = "Neon")
	ANeonManager* m_targetNeonMng;

	//★一旦こいつの共通変数として電源のオンオフを管理
	static bool m_neonSwich;

	//電源を警告状態で復旧する用のタイマー
	FTimerHandle TimeSlowTimerHandle;
};
