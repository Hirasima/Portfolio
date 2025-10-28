// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "FighterCharacter.h"
#include "InputActionValue.h"
#include "Enemy_Base.h"
#include "PlayPawn.generated.h"

class AWeapon;
class UCameraComponent;
class USpringArmComponent;
class UItemComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class AWire_HUD;
class UNiagaraSystem;
class AInteractionActor;
class UNiagaraComponent;
class AAttackActor;
class AHpExpressionActor;
class ABuffBitManager;
class UBoxComponent;

//アニメーション状態列挙型
UENUM(BlueprintType)
enum class EPlayerAnimState : uint8
{
	  Idle				UMETA(DisplayName = "Idle")
	, Run				UMETA(DisplayName = "Run")
	, Damage			UMETA(DisplayName = "Damage")
	, Dead				UMETA(DisplayName = "Dead")
	, Guard				UMETA(DisplayName = "Guard")
	, Parry_Up			UMETA(DisplayName = "Parry_Up")
	, Avoidance			UMETA(DisplayName = "Avoidance")
	, CriticalAttack    UMETA(DisplayName = "CriticalAttack")
	, Attack_Up			UMETA(DisplayName = "Attack_Up")
	, Attack_Under		UMETA(DisplayName = "Attack_Under")
	, Attack_Parry		UMETA(DisplayName = "Attack_Parry")
	, Stag				UMETA(DisplayName = "Stag")
	, Fall_Out			UMETA(DisplayName = "Fall_Out")
	, Fall_GlassBreak	UMETA(DisplayName = "FallGlassBreak")
	, MontaInteract		UMETA(DisplayName = "Interact")
	, Recovery			UMETA(DisplayName = "Recovery")
};

//行動状態列挙型
UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	  Idle				UMETA(DisplayName = "Idle")
	, Ready				UMETA(DisplayName = "Ready")
	, Damage			UMETA(DisplayName = "Damage")
	, Dead				UMETA(DisplayName = "Dead")
	, Attack_Up			UMETA(DisplayName = "Attack_Up")
	, Attack_Under		UMETA(DisplayName = "Attack_Under")
	, Attack_Parry		UMETA(DisplayName = "Attack_Parry")
	, Guard				UMETA(DisplayName = "Guard")
	, Parry				UMETA(DisplayName = "Parry")
	, Avoidance			UMETA(DisplayName = "Avoidance")
	, CriticalAttack    UMETA(DisplayName = "CriticalAttack")
	, Stag				UMETA(DisplayName = "Stag")
	, Recovery			UMETA(DisplayName = "Recovery")
};

//バフ効果列挙型
UENUM(BlueprintType)
enum class EBuffPattern : uint8
{
	None					UMETA(DisplayName = "None"),
	SpeedUp				UMETA(DisplayName = "SpeedUp"),
	AdditionalAttack	UMETA(DisplayName = "AdditionalAttack"),
};

//攻撃始め通知用デリゲート
DECLARE_DELEGATE_FiveParams(
FReceivePlayerAttackScence,
EAttackPattarnState,
FVector,
FVector,
float,
float
);

//ターゲットエネミーを受け取るためのデリゲート
DECLARE_DELEGATE_RetVal_FiveParams(AEnemy_Base*, FGetTargetEnemy, float, FVector, FVector, float, AEnemy_Base*);

UCLASS()
class ORIGINALACTION_API APlayPawn : public AFighterCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//オリジナルメソッド

	//エフェクトを出す
	virtual void SpawnEffect(EAttackReaction _attackReaction)override;

	//攻撃アクターを出す
	void SpawnAttackActor(int _attackPoint, int _attackNum);

	//バフの追加
	void AddBuff(EBuffPattern _newBuff);

	//バフのリセット
	void ResetBuff();

	//モンタージュを再生
	void PlayAnimMantage(UAnimMontage* _animMonatage)override;

	//カメラ角を前後逆にする関数
	void ReverceCamYaw();

	//ヒットストップさせる関数
	void DoHitStop();

	//カメラ揺れ時
	void DoCameraShaking(float _shakeAmount = 20.0f);

	//鞘の攻撃有効化
	UFUNCTION(BlueprintCallable)
	void IsActiveSheathAttack(bool _enable);

	//抜刀状態かを設定
	UFUNCTION(BlueprintCallable)
	void SetIsDrawWeapon(bool _isDraw);

	//範囲攻撃
	void DoAreaAttack(FVector _location, FRotator _rotation, FVector _collisiionSize);

	//範囲攻撃終了処理
	void FinishAreaAttack();

	//---------------------------------------------------
	// ゲッター ・　セッター
	//---------------------------------------------------

	//カメラコンポーネントの取得
	UCameraComponent* GetCameraComponent();

	//スプリングアームコンポーネントの取得
	USpringArmComponent* GetSpringArmComponent();

	//挙動状態を取得
	UFUNCTION(BlueprintCallable)
	EPlayerAnimState GetPlayerAnimState();
	//設定
	void SetPlayerAnimState(EPlayerAnimState _state);

	//プレイヤー状態に対応するAttackAssetを返す
	FAttackAsset GetAttackAsset()override;

	//HUDを登録する関数
	void SetHUDtoPlayer(AWire_HUD* _hud);

	//ターゲットエネミー取得
	AEnemy_Base* GetTargetEnemy(float _searchDistance, FVector _searchAngle, FVector _searcherPostion, float _nextTargetDirection = 0.0f, AEnemy_Base* _previousTarget = nullptr);

	//攻撃の種類の取得
	EAttackPattarnState GetAttackPattarnState()override;

	//スロウ状態にする関数☆菅澤加筆
	void BeSlow(float _slowTime,float _slowPower);

	//アニメーションモンタージュの再生速度を取得
	UFUNCTION(BlueprintCallable)
	float GetAnimMantegePlayRate();

	//インタラクトモンタージュ設定関数☆菅澤加筆
	void SetInteractMontage(UAnimMontage* _targetMontage);

	//打ち上げ攻撃可能か返す
	bool GetCanKnockUp()override;

	//カメラの位置を更新するかを設定
	UFUNCTION(BlueprintCallable)
	void SetDoUpdateCameraLocation(bool _do);

	//------------------------------------------------------------
	//　デリゲート設定用
	//------------------------------------------------------------

	//予備動作送信デリゲートの設定関数
	void SetAttackScenceReceiver(FReceivePlayerAttackScence _delegate);

	//ターゲットエネミー取得用デリゲートの設定関数
	void SetTargetEnemyDelegate(FGetTargetEnemy _delegate);

	//---------------------------------------------------
	//更新処理群
	//---------------------------------------------------

	//カメラの角度更新
	void UpdateCameraAngle(float _DeltaTime);

	//カメラの移動処理
	void UpdateCameraMove(float _DeltaTime);

	//移動の更新
	void UpdateMoving();

	//ターゲットマーカーの更新
	void UpdateTargetMarker();

	//バフの更新
	void UpdateBuffs(float _deltaTime);

	//ガード強度の更新
	void UpdateGuardPower(float _deltaTime);

	//回避処理の更新
	void UpdateAvoid(float _deltaTime);

	//死亡処理の更新
	void UpdateDead(float _deltaTime);

	//入力の更新
	void UpdateInput(float _deltaTime);

	//----------------------------------------------------
	// イベント関数
	//----------------------------------------------------

	//攻撃
	UFUNCTION(BlueprintCallable)
	void OnAttack();

	//攻撃終了時
	UFUNCTION(BlueprintCallable)
	void OnFinishAttack();

	//弾き
	UFUNCTION(BlueprintCallable)
	void OnParry();

	//弾き終了時
	UFUNCTION(BlueprintCallable)
	void OnFinishParry();

	//弾き攻撃時
	UFUNCTION(BlueprintCallable)
	void OnParryAttack();

	//弾き攻撃終了時
	UFUNCTION(BlueprintCallable)
	void OnFinishParryAttack();

	//回避時
	UFUNCTION(BlueprintCallable)
	void OnAvoid();

	//回避終了時
	UFUNCTION(BlueprintCallable)
	void OnFinishAvoid();

	//致命ダメージ時
	UFUNCTION(BlueprintCallable)
	void OnCriticalDamage();

	//致命終了時
	UFUNCTION(BlueprintCallable)
	void OnFinishCriticalAttack();

	UFUNCTION(BlueprintCallable)
	//アニメーションモンタージュ終了時
	void OnFinishAnimMontage();

	//インタラクト有効時
	UFUNCTION(BlueprintCallable)
	void ActivationInteractor();

	//回復時
	UFUNCTION(BlueprintCallable)
	void OnRecovery();

	//死亡時
	UFUNCTION(BlueprintCallable)
	void OnDead();

	//リスポーン時
	UFUNCTION(BlueprintCallable)
	void OnRespawn();

	//攻撃ヒット時のイベント関数
	void OnAttackHitEvent()override;

	//被攻撃時
	EAttackReaction OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp = false)override;

	//被ダメージ時
	void Damaged(int _damagePoint)override;

	//被弾き
	void Parried()override;

	//上段攻撃入力時
	void OnInputAttackUp();

	//下段攻撃入力時
	void OnInputAttackUnder();

	//パリィ攻撃入力時
	void OnInputParryAttack();

	//上段防御入力時
	void OnInputGuard();

	//----------------------------------------------------
	//入力関係
	//----------------------------------------------------

	//入力バインド関数
public:

	//移動入力
	void InputMoveDirection(const FInputActionValue& _Value);

	//カメラ入力
	void InputCameraAngle(const FInputActionValue& _Value);

	//カメラ入力終了
	void InputCameraAngleFin(const FInputActionValue& _Value);

	//上段攻撃入力
	void InputAttackUp(const FInputActionValue& _Value);

	//下段攻撃入力
	void InputAttackUnder(const FInputActionValue& _Value);

	//防御入力
	void InputGuard(const FInputActionValue& _Value);

	//防御入力終了
	void InputGuardFin(const FInputActionValue& _Value);

	//上段弾き入力
	void InputParryUp(const FInputActionValue& _Value);

	//回避入力
	void InputAvoidance(const FInputActionValue& _Value);

	//ロックオン入力
	void InputLockOn(const FInputActionValue& _Value);

	//カメラターゲット切り替え入力
	void InputCameraShift(const FInputActionValue& _Value);

	//カメラターゲット切り替え入力終了
	void InputCameraShiftFin(const FInputActionValue& _Value);

	//回復入力時
	void InputRecovery(const FInputActionValue& _Value);

	//シフト入力
	void InputShift(const FInputActionValue& _Value);

	//シフト入力終了
	void InputShiftFin(const FInputActionValue& _Value);

	//インタラクション入力時★菅澤追記
	void InputInteraction(const FInputActionValue& _Value);
	void InputInteractionOrigin();


	//----------------------------------------------------
	//衝突判定関係
	//----------------------------------------------------

	public:

	//武器の重なった時のイベント
	void OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult) override;
	
	//攻撃予備動作のイベント
	void ReceiveAttackScence(EAttackPattarnState _pattarn, FVector _PlayerPos, FVector _playerFrontVec, float _AttackRange, float _AttackDeg);

	UFUNCTION()
	//インタラクションオブジェクトとの重なりの検知用
	void OnBeginOverlap(
		UPrimitiveComponent* _overlappedComponent,
		AActor* _otherActor,
		UPrimitiveComponent* _otherComp, 
		int32 _otherBodyIndex,
		bool _bFromSweep, 
		const FHitResult& _sweepResult);

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* _overlappedComponent,
		AActor* _otherActor,
		UPrimitiveComponent* _otherComp,
		int32 _otherBodyIndex);


	// プロパティ ////////////////////////////////////////////////////////////////////////////
public:

	//カメラの初期視野角
	UPROPERTY(EditDefaultsOnly)
	float DefaultCameraFOV;

	//スプリングアームルートコンポーネントからの初期位置
	UPROPERTY(EditDefaultsOnly)
	FVector SpringArmOffset;

	//スプリングアームの長さ
	UPROPERTY(EditDefaultsOnly)
	float DefaultSpringArmLength;

	//カメラのピッチ上限
	UPROPERTY(EditAnywhere)
	float CameraPitchMax;

	//カメラのピッチ下限
	UPROPERTY(EditAnywhere)
	float CameraPitchMin;

	//ロックオン時のPitch
	UPROPERTY(EditAnywhere)
	float LockOnPitch;

	//ターゲット時の線形補間の補間率
	UPROPERTY(EditAnywhere)
	float LockOnLerpAlpha;
	
	//カメラリセットの速度
	UPROPERTY(EditAnywhere)
	float CameraResetSpeed;

	//ロックオン距離
	UPROPERTY(EditAnywhere)
	float LockOnDistance;

	//ロックオン中の移動速度倍率
	UPROPERTY(EditAnywhere)
	float LockOnMoveSpeedCorrectionValue;

	//pcでの旋回速度
	UPROPERTY(EditAnywhere)
	float PCTurningSpeed;

	//カメラ回転加速度
	UPROPERTY(EditAnywhere)
	float CameraRotationAcceleration;

	//カメラ回転減速度
	UPROPERTY(EditAnywhere)
	float CameraRotationDeceleration;

	//カメラ回転速度上限
	UPROPERTY(EditAnywhere)
	float CameraRotationSpeedMax;

	//回避の移動速度
	UPROPERTY(EditAnywhere)
	float AvoidanceMoveSpeed;

	//バフ最大数
	UPROPERTY(EditDefaultsOnly)
	int BuffRimitNum;

	//下段攻撃によるバフの継続時間
	UPROPERTY(EditAnywhere)
	float BuffDurationByUnderAttack;

	//上段攻撃によるバフ継続時間の増加倍率
	UPROPERTY(EditAnywhere)
	float BuffDurationRateByUpAttack;

	//バフの継続時間減少停止時間
	UPROPERTY(EditAnywhere)
	float BuffDurationStopTime;

	//コンボバフ最大数
	UPROPERTY(EditAnywhere)
	int ComboBuffRimitNum;

	//コンボバフ倍率
	UPROPERTY(EditAnywhere)
	float ComboBuffRate;

	//攻撃速度バフの倍率
	UPROPERTY(EditAnywhere)
	float SpeedUpRate;

	//ガードの強さの最大値
	UPROPERTY(EditAnywhere)
	int GuardPowerMax;

	//ガードの強さの回復速度
	UPROPERTY(EditAnywhere)
	float GuardPowerIncreasePerSecond;

	//不死モード
	UPROPERTY(EditAnywhere)
	bool IsUndeadMode;

	//死亡に掛かる時間
	UPROPERTY(EditAnywhere)
	float TimeTakenToDead;

	//リスポーン地点
	UPROPERTY(EditAnywhere)
	FVector RespawnPoint;

	//攻撃コリジョンを隠すか
	UPROPERTY(EditAnywhere)
	bool AttackCollisionHiddenInGame;

	//ターゲットマーカー
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TargetMarkerWidgetClass;

	//攻撃ヒット数アップ用のアクター
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAttackActor> AttackActorClass;

	//HP表現用アクタークラス
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHpExpressionActor> HpExpressionActorClass;

	//バフビット管理クラス
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuffBitManager> BuffBitManagerClass;

	//ダメージ演出用PostProcessmaterial
	UPROPERTY(EditAnywhere)
	UMaterialInterface* m_pDamagePostMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic* DamageOverlayInstance;

	UPROPERTY(EditAnywhere)
	float DamageOverlaySowTime;

private:
	//カメラコンポーネント
	UPROPERTY(VisibleInstanceOnly)
	UCameraComponent* CameraComp;

	//カメラの視野角
	float m_cameraFOV;

	//カメラ回転速度
	float m_cameraRotationSpeed;

	//カメラの角度リセット中か
	bool m_bResetingCamera;

	//カメラの相対位置
	FVector m_cameraRelativeLocation;

	//カメラの位置更新をするか
	bool m_bDoUpdateCameraLocation;

	//スプリングアーム
	UPROPERTY(VisibleInstanceOnly)
	USpringArmComponent* SpringArmComp;

	//スプリングアームの長さ
	float m_springArmLength;

	//回避ナイアガラコンポーネント
	UPROPERTY(VisibleInstanceOnly)
	UNiagaraComponent* AvoidEffectComp;

	//回避行動用コリジョン
	UPROPERTY(VisibleInstanceOnly)
	UBoxComponent* AvoidCollision;

	//回避開始時位置
	FVector AvoidBeginPosition;

	//アイテムコンポーネント
	UPROPERTY(VisibleInstanceOnly)
	UItemComponent* ItemComp;

	//ウィジェットコンポーネント
	UPROPERTY(VisibleInstanceOnly)
	UWidgetComponent* WidgetComp;

	//HUDのポインター
	UPROPERTY()
	AWire_HUD* HUD;

	//バフ
	UPROPERTY(EditInstanceOnly)
	TArray<EBuffPattern> Buffs;

	//攻撃の速度
	UPROPERTY(VisibleInstanceOnly)
	float AttackSpeed;

	//HP表現用アクター
	UPROPERTY(VisibleAnywhere)
	AHpExpressionActor* HpExpressionActor;

	//バフビットマネージャー
	UPROPERTY(VisibleInstanceOnly)
	ABuffBitManager* BuffBitManager;

	//現在のアニメーション状態
	EPlayerAnimState m_ePlayerAnimState;

	//現在の行動状態
	EPlayerActionState m_ePlayerActionState;

	//デフォルトの歩行速度
	float m_defaultMaxWalkSpeed;

	//バフの継続時間
	float m_buffAditionalTimer;

	//バフの停止時間
	float m_buffLifeTimer;

	//バフ攻撃済みか
	bool m_bDoBuffAttack;

	//コンボバフ数
	int m_comboBuffNum;

	//無敵状態フラグ
	bool m_bInvincible;

	//攻撃数カウンター
	int m_attackCounter;

	//ガードの強度加算用
	float GuardPowerFloat;

	//その防御行動でバフを獲得できたか
	bool IsSuccesiedGetBuff;

	//死亡時フェードアウト用α
	float m_alphaOfDeadFade;

	//死亡経過時間
	float m_deathDuration;

	//空中攻撃中か
	bool m_bDoFly;

	//弾き攻撃出来るか
	bool m_bCanParryAttack;

	//弾き攻撃入力があるか
	bool m_bIsInputParryAttack;

	//致命攻撃対象
	UPROPERTY()
	AFighterCharacter* m_pCriticalTarget;

	//打ち上げられた敵
	UPROPERTY()
	TArray<AFighterCharacter*> m_pKnockCaughtUpEnemies;

	//範囲攻撃用
	UPROPERTY()
	UBoxComponent* m_pAreaAttackCollisition;

	//弾いたアクター
	UPROPERTY()
	AFighterCharacter* m_pParriedFighter;

	//ガードの強度
	UPROPERTY(VisibleInstanceOnly)
	int GuardPower;

	//ヒットストップ前の時間の進む速度
	float m_dilationTimeBeforeHitStop;

	//------------------------------------------------
	//アニメーション関係
	//------------------------------------------------

public:

	//モンタージュ

	//弾き
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* ParryMontage;

	//弾き攻撃
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset ParryAttack;

	//回避
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AvoidanceMontage;

	//回復
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* RecoveryMontage;

	//ガードブレイク
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* GuardBreakMontage;

	//死亡
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DeadMontage;

	//アタックアセット

	//上段
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUps;

	//下段
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackUnders;

	//空中
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAttackAsset> AttackAirs;

	//空中強
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset AttackStrongAirs;

	//致命
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAttackAsset CriticalAttack;

private:

	//インタラクトアニメーション
	UPROPERTY()
	UAnimMontage* m_InteractTargetMontage;

	// デリゲート ///////////////////////////////////////////

	//バトルマネージャに予備動作を伝達するデリゲート
	FReceivePlayerAttackScence m_AttackScenceDeleGate;

	//ターゲットエネミーを取得するデリゲート
	FGetTargetEnemy m_GetTargetEnemyDeleGate;

	//----------------------------------------------------
	//入力関係
	//----------------------------------------------------

public:

	//インプットアクション

	//移動アクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_moveAction;

	//カメラアクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_cameraMoveAction;

	//攻撃アクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_attackUpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_attackUnderAction;

	//防御アクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_guardAction;

	//上段弾きアクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_parryUpAction;

	//回避アクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_avoidanceAction;

	// ロックオンアクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_LockOnAction;

	//カメラターゲット切り替えアクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_CameraShiftAction;

	//シフトアクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ShiftAction;

	//インタラクトアクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Interaction;

	//回復アクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Recovery;


	//---------------------------------------------------
	// エフェクト関係
	//---------------------------------------------------

public:
	//ダメージエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* DamageEffect;

	//ガードエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* GuardEffect;

	//パリィエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryEffect;

	//パリィ攻撃発動エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryAttackActivationEffect;

	//パリィ攻撃エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ParryAttackEffect;

	//回避エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* AvoidEffect;

	//-------------------------------------------------------
	// 効果音系
	//-------------------------------------------------------

public:

	//SE音量
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	float SE_Volume;

	//回避SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* AvoidSE;

	//回避成功SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* JustAvoidSE;

	//ガードブレイクSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* GuardBreakSE;

	//弾きSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* ParrySE;

	//弾き成功SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* JustParrySE;

	//弾き攻撃入力SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* ParryAttackInputSE;

	//弾き攻撃SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* ParryAttackSE;

	//回復SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* RecoverySE;

	//バフ解除SE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* BuffResetSE;

	//リスポーンSE
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* RespawnSE;

private:

	//入力バインド用変数////////////////////////////////////////

	//現在のカメラ角度(FVector2D)
	FVector2D m_cameraAngle;

	//現在の移動方向(FVector2D)
	FVector2D m_moveDirection;

	//ロックオン入力
	bool m_bLockOn;

	//ターゲット切り替え可能か
	bool m_bCameraShift;

	float m_cameraShiftDire;

	//シフト状態
	bool m_bShiftOn;

	//etc

	//ターゲット中のファイター
	AEnemy_Base* m_pTargetEnemy;

	//スロウ状態を終える関数☆菅澤加筆
	void EndSlow();
	//状態変数
	//スロウか否か☆菅澤加筆
	bool m_IsSlow;
	//スロウタイマー管理用タイムハンドラー☆菅澤加筆
	FTimerHandle TimeSlowTimerHandle;

	//現在重なっているインタラクト対象
	UPROPERTY()
	AInteractionActor* m_nowTargetInteractor;

	//（ダメージ時に呼ぶ）グリッジポストプロセスを出現させる関数
	void ShowDamageOverlay();

	UPROPERTY()
	TArray<AEnemy_Base*> m_AirTargetEnemys;
};
