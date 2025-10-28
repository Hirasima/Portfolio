// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FighterCharacter.generated.h"

//前方宣言
class AWeapon;
class UCapsuleComponent;

//攻撃起動通知ステート
UENUM(BlueprintType)
enum class EAttackPattarnState : uint8
{
	None		UMETA(DisplayName = "None"),
	UpAttack	UMETA(DisplayName = "UpAttack"),
	DownAttack	UMETA(DisplayName = "DownAttack"),
	UpParry		UMETA(DisplayName = "UpParry"),
	DownParry	UMETA(DisplayName = "DownParry")
};

//被攻撃時リアクション
UENUM(BlueprintType)
enum class EAttackReaction : uint8
{
	None		UMETA(DisplayName = "None"),
	Hit			UMETA(DisplayName = "Hit"),
	Guard		UMETA(DisplayName = "Guard"),
	Parry		UMETA(DisplayName = "Parry")
};

//よろけの大きさ列挙型
UENUM(BlueprintType)
enum class EStagType : uint8
{
	None		UMETA(DisplayName = "None"),
	SmallStag	UMETA(DisplayName = "SmallStag"),
	BigStag		UMETA(DisplayName = "BigStag"),
	Down		UMETA(DisplayName = "Down")
};

//攻撃を管理する構造体
USTRUCT(BlueprintType)
struct FAttackAsset
{
	GENERATED_BODY()

public:

	//アニメーション
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimMantage;

	//攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int AttackPoint;

	//ノックバックの力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KnockBackPower;

	//ガードを削る力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GuardBreakPower;

	//怯みタイプ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStagType StagType;

	//打ち上げ攻撃可能か
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CanKnockUp;

	//範囲攻撃コリジョンの相対位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector AreaCollisionRelativeLocation;

	//範囲攻撃コリジョンの大きさ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector AreaCollisionSize;

	//コンストラクタ
	FAttackAsset(UAnimMontage* _animMontage = nullptr, float _attackPoint = 10, float _knockBackPower = 0.0f, EStagType _stagType = EStagType::None, bool _canKnockUp = false
		, FVector _collisionRelativeLocation = FVector::ZeroVector, FVector _collisionSize = FVector::ZeroVector)
	{
		AnimMantage = _animMontage;
		AttackPoint = _attackPoint;
		KnockBackPower = _knockBackPower;
		StagType = _stagType;
		CanKnockUp = _canKnockUp;
		AreaCollisionRelativeLocation = _collisionRelativeLocation;
		AreaCollisionSize = _collisionSize;
	}
};

UCLASS()
class ORIGINALACTION_API AFighterCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class UItemComponent;

public:
	// Sets default values for this character's properties
	AFighterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// オリジナルメソッド //////////////////////////////////////////////////////////////////////

	//攻撃されたアクターのリストの中身を消す
	void ClearAttackedActors();

	//エフェクトを出す
	virtual void SpawnEffect(EAttackReaction _attackReaction);

	//ノックバック中更新関数
	virtual void UpdateKnockBack(float DeltaTime);

	//モンタージュを再生
	virtual void PlayAnimMantage(UAnimMontage* _animMontage);

protected:

	//特定方向を向く関数★菅澤加筆
	void FaceTargetLocation(const FVector _targetLocation);


	//---------------------------------------------------
	// ゲッター ・　セッター
	//---------------------------------------------------
public:

	//攻撃の種類の取得
	virtual EAttackPattarnState GetAttackPattarnState();

	//現在の状態に対応するAttackAssetを返す
	UFUNCTION(BlueprintCallable)
	virtual FAttackAsset GetAttackAsset();

	//ガード強度を削る力を返す
	virtual int GetGuardBreakPower();

	//打ち上げ攻撃可能か返す
	virtual bool GetCanKnockUp();

	//現在のHP割合を返す
	float GetHPRatio();

	//----------------------------------------------------
	// イベント関数
	//----------------------------------------------------

public:

	//攻撃を受けたとき
	virtual EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false);

	//被ダメージ時
	UFUNCTION(BlueprintCallable)
	virtual void Damaged(int _damagePoint);

	//被致命攻撃時
	virtual void OnCriticalAttacked(int _damage);

	//被パリィ時
	virtual void Parried();

	//被ノックバック時
	virtual void KnockBacked(FVector _direction, float _Time);

	//攻撃ヒット時のイベント関数
	virtual void OnAttackHitEvent();

	//----------------------------------------------------
	//衝突判定関係
	//----------------------------------------------------

public:

	//武器の重なった時のイベント
	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);

public:
	//---------------------------------------------------
	// プロパティ
	//---------------------------------------------------

	//武器のスタティックメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> m_pWeaponClass;

	//鞘
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> m_pSheathClass;

	//歩行速度
	UPROPERTY(EditAnywhere)
	float m_walkSpeed;

	//ヒットポイント最大値
	UPROPERTY(EditAnywhere)
	int m_Max_HitPoint;

	//攻撃力
	UPROPERTY(EditAnywhere)
	int m_attackPoint;

	//致命可能か？
	bool m_bEnableCritical;

	//打ち上げられているか
	bool m_bIsKnockUpping;

	//-------------------------------------------------------
	// 効果音系
	//-------------------------------------------------------

	//攻撃SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* AttackSE;

	//被ダメージSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* DamageSE;

	//死亡SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* DeadSE;

	//ガードSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* GuardSE;

	//ガード成功SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* JustGuardSE;

protected:

	//武器を入れる器
	AWeapon* m_pWeapon;

	//鞘を入れる器
	AWeapon* m_pSheath;

	//アニメーションインスタンス
	UAnimInstance* m_pAnimInstance;

	//被攻撃済みのアクター
	TArray<AActor*> m_attackedActors;

	//HPを入れる器
	int m_hitPoint;

	//攻撃力バフ倍率
	float m_AttackBuffRate;

	//Montage再生速度
	float m_montagePlayRate;

	//ノックバック用☆菅澤加筆
	FVector KnockBackStartPos;
	FVector KnockBackEndPos;
	float KnockBackDuration = 0.0f;
	float KnockBackElapsed = 0.0f;
	bool bIsKnockBack = false;

};
