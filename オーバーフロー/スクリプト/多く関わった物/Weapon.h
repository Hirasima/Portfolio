// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ORIGINALACTION_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//オリジナルメソッド-------------------------------------------------

public:

	//コリジョンの有効化設定
	UFUNCTION(BlueprintCallable)
	void SetCollisionActive(bool _enable);

	//武器の軌道の有効設定
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitActive(bool _enable);

	//武器の軌道の色の設定
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitColor(FLinearColor _linearColor);

	//武器の軌道の幅の設定
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitWidth(float _width);

	//武器のエフェクトのパラメータ設定
	UFUNCTION(BlueprintCallable)
	void SetBladeOrbitEffectParam(float _spawnRate, float _size);

	//武器の攻撃状態設定用
	UFUNCTION(BlueprintCallable)
	void SetAttacking(bool _enabl);

	//プリミティブコンポーネントの取得
	UFUNCTION(BlueprintCallable)
	UPrimitiveComponent* GetPrimitiveComponent();

	void ChangeColor(FLinearColor _color, bool _point = false, int _materialIndex = 2);

	bool GetIsPoint();
	void SetIsPoint(bool _point,int _index=2);

	UFUNCTION(BlueprintCallable)
	void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);

//オリジナルプロパティ*----------------------------------------------

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_pStaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* m_pNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neon")
	FLinearColor Color = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neon")
	int m_NeonMatNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* m_pBladeOrbit;

protected:

	bool m_bAttacking;

	bool m_IsPoint;

};
