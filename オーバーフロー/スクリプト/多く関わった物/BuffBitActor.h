// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//☆リスクはあるがバフenum利用のためプレイポーンをインクルード
#include "PlayPawn.h"
#include "BuffBitActor.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ORIGINALACTION_API ABuffBitActor : public AActor
{
	GENERATED_BODY()

	friend class ABuffBitManager;
	
public:	
	// Sets default values for this actor's properties
	ABuffBitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//バフを伝達される関数
	void SetBuffType(EBuffPattern _bufftype);

	//引数の敵に攻撃する関数
	void AttackTargetActor(AActor* _actor);

	//ソケット名設定
	void SetSocketName(FName _idolSocket, FName _activeSocket);

	//親スケルタルメッシュ設定
	void SetParentSkeltalMeshComp(USkeletalMeshComponent* _skeltalMeshComp);

	//点滅切り替え設定
	void SetBitIsPoint(bool _anable);

	//移動更新
	void UpdateMove(float DeltaTime);

	//攻撃処理
	void UpdateAttack(float DeltaTime);

	//プロパティ
	//メッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_pStaticMeshComp;
	//バフ色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffColor")
	FLinearColor AttackBuff_Color=FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffColor")
	FLinearColor SpeedBuff_Color = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuffColor")
	FLinearColor NoneBuff_Color=FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* m_pCircleModeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* m_pAttackModeMesh;

	UPROPERTY()
	EBuffPattern m_eBitState;

	//攻撃位置の半径
	UPROPERTY(EditDefaultsOnly)
	float AttackRadius;

	//攻撃位置の角度制限
	UPROPERTY(EditDefaultsOnly)
	FRotator AttackMinRotation;
	UPROPERTY(EditDefaultsOnly)
	FRotator AttackMaxRotation;

protected:
	UPROPERTY()
	UMaterialInterface* m_pMaterial;

	UPROPERTY()
	UNiagaraComponent* m_pBeamComponent;

	UPROPERTY(VisibleInstanceOnly)
	UNiagaraComponent* m_pOrbitComp;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* m_pBitBeam;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* m_pBitAwake;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* m_pBitOrbit;


	//アクティブ時と待機時のソケット名
	FName m_activeSocketName;
	FName m_idolSocketName;

	//親のメッシュコンポーネント
	UPROPERTY()
	USkeletalMeshComponent* m_pParentSkeltalMeshComp;

	//ビットの存在してる時間
	float timer;

	//攻撃中か
	bool m_bIsAttacking;

	//攻撃中のトランスフォーム
	FVector m_attackPos;
	FRotator m_attackRot;
	
	//攻撃対象
	UPROPERTY()
	AActor* m_pAttckActor;
	//攻撃時間
	float attackTimer;
	//攻撃済みかどうか
	bool m_bAttacked;

	//点滅状態
	bool IsPoint;
};
