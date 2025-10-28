// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Weapon.h"
#include "InteractionActor.h"
#include "Wire_HUD.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "WidgetTargetMarker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttackActor.h"
#include "HpExpressionActor.h"
#include "BuffBitManager.h"
#include "ItemComponent.h"
#include "NiagaraComponent.h"


// Sets default values
APlayPawn::APlayPawn()
	: m_ePlayerAnimState(EPlayerAnimState::Idle)
	, m_cameraAngle(FVector2D::ZeroVector)
	, m_moveDirection(FVector2D::ZeroVector)
	, m_bLockOn(false)
	, m_bCameraShift(true)
	, m_bShiftOn(false)
	, m_pTargetEnemy(nullptr)
	, m_IsSlow(false)
	, m_nowTargetInteractor(nullptr)
	, m_buffAditionalTimer(0.0f)
	, m_bDoBuffAttack(false)
	, m_bInvincible(false)
	, m_attackCounter(0)
	, m_cameraRelativeLocation(FVector::ZeroVector)
	, m_pCriticalTarget(nullptr)
	, m_bDoUpdateCameraLocation(true)
	, m_alphaOfDeadFade(0.0f)
	, m_deathDuration(0.0f)
	, m_bDoFly(false)
	, m_bCanParryAttack(false)
	, m_bIsInputParryAttack(false)
	, m_pAreaAttackCollisition(nullptr)
	, m_bResetingCamera(false)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//スプリングアームの初期化
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(SpringArmOffset);
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->ProbeChannel = ECC_Camera;

	//カメラの初期化
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	//アイテムコンポーネントの初期化
	ItemComp = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComp"));

	//回避用エフェクトコンポーネントの初期化
	AvoidEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AvoidEffectComp"));
	AvoidEffectComp->SetupAttachment(RootComponent);

	//回避用コリジョン初期化
	AvoidCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AvoidCollision"));
	AvoidCollision->SetupAttachment(RootComponent);

	//範囲攻撃用コリジョン
	m_pAreaAttackCollisition = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaAttackCollision"));

	//ターゲットマーカーの初期化
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));

	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	

	//Playerタグを追加
	Tags.Add("Player");
}

// Called when the game starts or when spawned
void APlayPawn::BeginPlay()
{
	Super::BeginPlay();

	ItemComp->m_pParentFighter = this;

	//カメラの初期化
	CameraComp->FieldOfView = DefaultCameraFOV;
	m_cameraFOV = DefaultCameraFOV;

	//スプリングアームの位置の初期化
	SpringArmComp->SetRelativeLocation(SpringArmOffset);
	//スプリングアームの回転の初期化
	SpringArmComp->SetRelativeRotation(FRotator::ZeroRotator);
	//スプリングアームの長さの初期化
	SpringArmComp->TargetArmLength = DefaultSpringArmLength;
	m_springArmLength = DefaultSpringArmLength;

	SpringArmComp->bUsePawnControlRotation = false;

	//回避用コリジョン初期化
	float radius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float halfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	AvoidCollision->SetBoxExtent(FVector(radius, radius, halfHeight));
	AvoidCollision->SetActive(false);

	//範囲攻撃用コリジョン
	m_pAreaAttackCollisition->OnComponentBeginOverlap.AddDynamic(this, &APlayPawn::OnWeaponOverlap);
	m_pAreaAttackCollisition->SetBoxExtent(FVector::ZeroVector);
	m_pAreaAttackCollisition->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//自動ポストプロセスを黙らせる
	CameraComp->PostProcessSettings.bOverride_AutoExposureMethod = true;
	CameraComp->PostProcessSettings.bOverride_AutoExposureMinBrightness = true;
	CameraComp->PostProcessSettings.bOverride_AutoExposureMaxBrightness = true;
	CameraComp->PostProcessSettings.bOverride_AutoExposureBias = true;

	CameraComp->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
	CameraComp->PostProcessSettings.AutoExposureMinBrightness = 1.0f;
	CameraComp->PostProcessSettings.AutoExposureMaxBrightness = 1.0f;
	CameraComp->PostProcessSettings.AutoExposureBias = 10.0f;

	//ダメージ演出用ポストマテリアルの初期化
	if (m_pDamagePostMaterial) {
		DamageOverlayInstance = UMaterialInstanceDynamic::Create(m_pDamagePostMaterial, this);
		DamageOverlayInstance->SetScalarParameterValue(TEXT("DamageAmount"), 0.0f);

		CameraComp->AddOrUpdateBlendable(DamageOverlayInstance);
	}

	//オーバーラップの登録☆菅澤追記
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayPawn::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayPawn::OnEndOverlap);

	HpExpressionActor = GetWorld()->SpawnActor<AHpExpressionActor>(HpExpressionActorClass);
	HpExpressionActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("HeadSetSocket"));

	//バフビットマネージャー初期化
	BuffBitManager = GetWorld()->SpawnActor<ABuffBitManager>(BuffBitManagerClass);
	BuffBitManager->Init(GetMesh());

	//Yawの自動更新を非アクティブ化
	bUseControllerRotationYaw = false;

	//リスポーン地点の初期化
	RespawnPoint = GetActorLocation();

	//HPを最大に
	m_hitPoint = m_Max_HitPoint;

	GuardPower = GuardPowerMax;

	//攻撃速度の初期化
	m_montagePlayRate = 1.0f;

	//バフの停止時間を初期化（最大にする）
	m_buffLifeTimer = BuffDurationStopTime;

	//バフ初期化
	Buffs.Init(EBuffPattern::None, 5);

	//バフをHUDに伝達
	HUD->SetPlayerBuff(Buffs);

	//武器の初期化
	m_pWeapon->SetBladeOrbitColor(FLinearColor::Blue * 0.5f);
	m_pWeapon->SetBladeOrbitWidth(20.0f);
	
	//キャラクタームーブメントの歩行速度の初期値を保存
	m_defaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//ItemComp->CommunicateToHUD();

	if (!TargetMarkerWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Marker Widget Class Is None"));
	}

	//ウィジェットの初期化
	WidgetComp->SetWidgetClass(TargetMarkerWidgetClass);
	WidgetComp->InitWidget();
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComp->SetDrawAtDesiredSize(true);
	WidgetComp->SetTranslucentSortPriority(20); // 描画優先度を上げる
	WidgetComp->SetVisibility(false);
}

//---------------------------------------------------------------------------
// Tick
//---------------------------------------------------------------------------
void APlayPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//カメラの角度更新
	UpdateCameraAngle(DeltaTime);

	//カメラの移動処理
	UpdateCameraMove(DeltaTime);

	//移動の更新
	UpdateMoving();

	//ターゲットマーカーの更新
	UpdateTargetMarker();

	//バフの更新
	UpdateBuffs(DeltaTime);

	//ガード強度の更新
	UpdateGuardPower(DeltaTime);

	//回避中なら
	if (m_ePlayerActionState == EPlayerActionState::Avoidance)
	{
		UpdateAvoid(DeltaTime);
	}

	//死亡中なら
	if (m_ePlayerActionState == EPlayerActionState::Dead)
	{
		UpdateDead(DeltaTime);
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
	{
		UE_LOG(LogTemp, Log, TEXT("PLC EEEEEEEEEEEEEEEEEEEEEEE"));
		/*FVector vec(0.0f, 0.0f, 10000.0f);
		LaunchCharacter(vec, true, true);*/
	}
	if(GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
		
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("PlayerAnimState : %s"), *UEnum::GetValueAsString(m_ePlayerAnimState))
	//UE_LOG(LogTemp, Warning, TEXT("PlayerActionState : %s"), *UEnum::GetValueAsString(m_ePlayerActionState));
}

void APlayPawn::PossessedBy(AController* NewController) {
	UE_LOG(LogTemp, Warning, TEXT("Player : Called Process:PossessedBy"));
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void APlayPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//ヌルチェック群
	if (!PlayerInputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Component Not Found:PlayerInputComponent"));
		return;
	}
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Faild Process:Cast To UEnhancedInputComponent from PlayerInputComponent"));
		return;
	}

	//カメラ回転の入力をバインド
	EnhancedInputComponent->BindAction(IA_cameraMoveAction, ETriggerEvent::Triggered, this, &APlayPawn::InputCameraAngle);
	//カメラ回転の入力をバインド
	EnhancedInputComponent->BindAction(IA_cameraMoveAction, ETriggerEvent::Completed, this, &APlayPawn::InputCameraAngleFin);

	//移動入力をバインド
	EnhancedInputComponent->BindAction(IA_moveAction, ETriggerEvent::Triggered, this, &APlayPawn::InputMoveDirection);

	//上段攻撃入力をバインド
	EnhancedInputComponent->BindAction(IA_attackUpAction, ETriggerEvent::Started, this, &APlayPawn::InputAttackUp);
	//下段攻撃入力をバインド
	EnhancedInputComponent->BindAction(IA_attackUnderAction, ETriggerEvent::Started, this, &APlayPawn::InputAttackUnder);

	// 10/1仕様変更により機能を停止

	//上防御入力をバインド
	EnhancedInputComponent->BindAction(IA_guardAction, ETriggerEvent::Started, this, &APlayPawn::InputGuard);
	//上防御入力終了をバインド
	EnhancedInputComponent->BindAction(IA_guardAction, ETriggerEvent::Completed, this, &APlayPawn::InputGuardFin);

	//弾き入力をバインド
	EnhancedInputComponent->BindAction(IA_parryUpAction, ETriggerEvent::Started, this, &APlayPawn::InputParryUp);

	//回避入力をバインド
	EnhancedInputComponent->BindAction(IA_avoidanceAction, ETriggerEvent::Started, this, &APlayPawn::InputAvoidance);

	//ロックオン入力をバインド
	EnhancedInputComponent->BindAction(IA_LockOnAction, ETriggerEvent::Started, this, &APlayPawn::InputLockOn);

	//カメラターゲット切り替えをバインド
	EnhancedInputComponent->BindAction(IA_CameraShiftAction, ETriggerEvent::Started, this, &APlayPawn::InputCameraShift);
	//カメラターゲット切り替えをバインド
	EnhancedInputComponent->BindAction(IA_CameraShiftAction, ETriggerEvent::Completed, this, &APlayPawn::InputCameraShiftFin);

	////シフト入力をバインド
	//EnhancedInputComponent->BindAction(IA_ShiftAction, ETriggerEvent::Started, this, &APlayPawn::InputShift);
	////シフト入力終了をバインド
	//EnhancedInputComponent->BindAction(IA_ShiftAction, ETriggerEvent::Completed, this, &APlayPawn::InputShiftFin);

	//インタラクトをバインド☆菅澤記載
	EnhancedInputComponent->BindAction(IA_Interaction, ETriggerEvent::Completed, this, &APlayPawn::InputInteraction);

	// 10/1仕様変更により機能を停止

	////回復入力をバインド
	//EnhancedInputComponent->BindAction(IA_Recovery, ETriggerEvent::Started, this, &APlayPawn::InputRecovery);
}


//---------------------------------------------------------------------------------------------
//オリジナルメソッド
//---------------------------------------------------------------------------------------------

//エフェクトを出す
void APlayPawn::SpawnEffect(EAttackReaction _attackReaction)
{
	//リアクションが無効だったら無視
	if (_attackReaction == EAttackReaction::None)
	{
		return;
	}
	//被ダメ時
	else if (_attackReaction == EAttackReaction::Hit)
	{
		UNiagaraComponent* niagaraComp;

		niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageEffect, GetCapsuleComponent()->GetComponentLocation() + FVector(10.0f, 0.0f, 50.0f));

		niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor(10.0f, 0.0f, 0.0f, 0.2));
	}
	//ガード時
	else if (_attackReaction == EAttackReaction::Guard)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GuardEffect, m_pWeapon->GetActorLocation());
	}
	//弾き時
	else if (_attackReaction == EAttackReaction::Parry)
	{
		UNiagaraComponent* niagaraComp;

		niagaraComp=UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryEffect, m_pWeapon->GetActorLocation());

		niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor::Blue * 50.0f);
	}
}

//攻撃アクターを出す
void APlayPawn::SpawnAttackActor(int _attackPoint, int _attackNum)
{
	UWorld* world = GetWorld();

	AAttackActor* attackActor = nullptr;

	//有効でなければ終了
	if (!world) return;

	attackActor = Cast<AAttackActor>(world->SpawnActor(AttackActorClass));

	//スポーンしたアクターを初期化
	attackActor->SetAttackPoint(_attackPoint);
	attackActor->SetAttackNum(_attackNum);
	attackActor->SetDamageTarget(m_attackedActors);

	//ビットに攻撃を伝える
	if (m_attackedActors.IsValidIndex(0))
	{
		//UE_LOG(LogTemp, Log, TEXT("Player : Bit Attack"));
		BuffBitManager->Attack(m_attackedActors[0]);
	}
}

//バフの追加
void APlayPawn::AddBuff(EBuffPattern _newBuff)
{
	//バフの停止時間を初期化（最大にする）
	m_buffLifeTimer = BuffDurationStopTime;

	//もしバフの数が上限になっていたら
	if (Buffs.Num() >= BuffRimitNum)
	{
		//一番先頭の要素を削除
		Buffs.RemoveAt(0);
	}

	//バフの追加
	Buffs.Add(_newBuff);

	//Widgetに通知
	HUD->SetPlayerBuff(Buffs);

	//ビットにバフを通知
	BuffBitManager->SetBuffs(Buffs);

	// バフによる倍率の計算
	m_montagePlayRate = 1.0f;

	//バフの分速度を上げる
	for (int i = 0; i < Buffs.Num(); ++i)
	{
		if (Buffs[i] == EBuffPattern::SpeedUp)
		{
			m_montagePlayRate *= SpeedUpRate;
		}
	}

	//歩行速度を更新
	GetCharacterMovement()->MaxWalkSpeed = m_defaultMaxWalkSpeed * m_montagePlayRate;
}

//バフのリセット
void APlayPawn::ResetBuff()
{
	Buffs.Init(EBuffPattern::None, 5);

	//ビットにバフを通知
	BuffBitManager->SetBuffs(Buffs);

	//コンボバフをリセット
	m_comboBuffNum = 0;

	//バフをHUDに伝達
	HUD->SetPlayerBuff(Buffs);

	m_montagePlayRate = 1.0f;

	//歩行速度を更新
	GetCharacterMovement()->MaxWalkSpeed = m_defaultMaxWalkSpeed;
}

//カメラを前後反転する関数☆菅澤加筆
void APlayPawn::ReverceCamYaw() {
	//☆演出仮置き☆菅澤追記
	FRotator Reverce = SpringArmComp->GetRelativeRotation();
	Reverce.Yaw += 180.0f;
	SpringArmComp->SetRelativeRotation(Reverce);
}

//モンタージュを再生
void APlayPawn::PlayAnimMantage(UAnimMontage* _animMontage)
{
	if (m_ePlayerActionState != EPlayerActionState::Idle)
	{
		return;
	}

	Super::PlayAnimMantage(_animMontage);
}

//ヒットストップさせる関数
void APlayPawn::DoHitStop()
{
	UE_LOG(LogTemp, Log, TEXT("Hit Stop"));

	float dilationTime = 0.0001;
	float stopTime = 0.15;

	//現在のレベルを取得
	UWorld* world = GetWorld();

	//現在の時間の進むスピードを取得取得
	m_dilationTimeBeforeHitStop = UGameplayStatics::GetGlobalTimeDilation(world);

	//時間を止める
	UGameplayStatics::SetGlobalTimeDilation(world, dilationTime);

	//タイマーハンドル
	FTimerHandle TimerHandle;

	//タイマーセット
	//ヒットストップ終了時に元の時間の進む速度に戻す
	GetWorldTimerManager().SetTimer(
		TimerHandle, 
		[this]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Hit Stop End"));
		},
		stopTime * dilationTime,
		false
	);
}

//カメラ揺れ時
void APlayPawn::DoCameraShaking(float _shakeAmount) 
{
	UE_LOG(LogTemp, Log, TEXT("Camera Shaking"));

	//現在のレベルを取得
	UWorld* world = GetWorld();

	FVector cameraPos = CameraComp->GetComponentLocation();

	FVector shakeAmount = {
		FMath::RandRange(-_shakeAmount, _shakeAmount),
		0.0f,
		FMath::RandRange(-_shakeAmount, _shakeAmount)
	};

	CameraComp->SetWorldLocation(shakeAmount + cameraPos);

	//タイマーハンドル
	FTimerHandle TimerHandle;

	//タイマーセット
	//ヒットストップ終了時に元の時間の進む速度に戻す
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			CameraComp->SetWorldLocation(SpringArmComp->GetSocketLocation(SpringArmComp->SocketName));
			UE_LOG(LogTemp, Log, TEXT("Camera Shaking End"));
		},
		0.05,
		false
	);
}

//鞘の攻撃有効化
void APlayPawn::IsActiveSheathAttack(bool _enable)
{
	if (!m_pSheath)
	{
		return;
	}

	m_pSheath->SetAttacking(_enable);
}

//抜刀状態かを設定
void APlayPawn::SetIsDrawWeapon(bool _isDraw)
{
	if (!m_pWeapon || !m_pSheath)
	{
		return;
	}

	//武器の有効化設定
	m_pWeapon->SetActive(_isDraw);
	//鞘の状態変更
	m_pSheath->ChangeStaticMesh(!_isDraw);
}

//範囲攻撃
//第二引数(回転はFRotator::ZeroRotatorだった場合プレイヤーの回転が代入される)
void APlayPawn::DoAreaAttack(FVector _location, FRotator _rotation = FRotator::ZeroRotator, FVector _collisiionSize = FVector(100.0f, 100.0f, 100.0f))
{
	//ヌルチェック
	if (!m_pAreaAttackCollisition)
	{
		return;
	}

	//コリジョンの有効化
	m_pAreaAttackCollisition->SetActive(true);

	m_pAreaAttackCollisition->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//コリジョンの位置を設定
	m_pAreaAttackCollisition->SetWorldLocation(_location);
	//コリジョンの回転を設定
	if (_rotation == FRotator::ZeroRotator) _rotation = GetActorRotation();
	m_pAreaAttackCollisition->SetWorldRotation(_rotation);
	//コリジョンの大きさを設定
	m_pAreaAttackCollisition->SetBoxExtent(_collisiionSize);

	//デバッグ用
	//コリジョンの可視化
	m_pAreaAttackCollisition->bHiddenInGame = AttackCollisionHiddenInGame;
}

//範囲攻撃終了処理
void APlayPawn::FinishAreaAttack()
{
	//ヌルチェック
	if (!m_pAreaAttackCollisition)
	{
		return;
	}

	m_pAreaAttackCollisition->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//コリジョンの位置を設定
	m_pAreaAttackCollisition->SetWorldLocation(FVector::ZeroVector);
	//コリジョンの大きさを設定
	m_pAreaAttackCollisition->SetBoxExtent(FVector::ZeroVector);
	//攻撃済みアクタークリア
	ClearAttackedActors();

	//デバッグ用
	if(!m_pAreaAttackCollisition->bHiddenInGame)
	m_pAreaAttackCollisition->bHiddenInGame = true;
}


//---------------------------------------------------
// ゲッター ・　セッター
//---------------------------------------------------

//カメラコンポーネントの取得
UCameraComponent* APlayPawn::GetCameraComponent()
{
	return CameraComp;
}

//スプリングアームコンポーネントの取得
USpringArmComponent* APlayPawn::GetSpringArmComponent()
{
	return SpringArmComp;
}

//挙動状態を取得
EPlayerAnimState APlayPawn::GetPlayerAnimState()
{
	return m_ePlayerAnimState;
}

void APlayPawn::SetPlayerAnimState(EPlayerAnimState _state) {
	m_ePlayerAnimState = _state;
}

//攻撃動作送信デリゲートの設定
void APlayPawn::SetAttackScenceReceiver(FReceivePlayerAttackScence _delegate) {
	m_AttackScenceDeleGate = _delegate;
}

//ターゲットエネミー取得用デリゲートの設定関数
void APlayPawn::SetTargetEnemyDelegate(FGetTargetEnemy _delegate) {
	m_GetTargetEnemyDeleGate = _delegate;
}

//インタラクトモンタージュ設定関数☆菅澤加筆
void APlayPawn::SetInteractMontage(UAnimMontage* _targetMontage) {
	m_InteractTargetMontage = _targetMontage;
	m_ePlayerAnimState = EPlayerAnimState::MontaInteract;
}

//打ち上げ攻撃可能か返す
bool APlayPawn::GetCanKnockUp()
{
	return GetAttackAsset().CanKnockUp;
}

//カメラの位置を更新するかを設定
void APlayPawn::SetDoUpdateCameraLocation(bool _do)
{
	m_bDoUpdateCameraLocation = _do;

	if (m_bDoUpdateCameraLocation)
	{
		CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepWorldTransform);
		CameraComp->SetRelativeRotation(FRotator::ZeroRotator);
	}
	else
	{
		CameraComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

//プレイヤー状態に対応するAttackAssetを返す
FAttackAsset APlayPawn::GetAttackAsset()
{
	FAttackAsset attackAsset;

	switch (m_ePlayerAnimState)
	{
	case EPlayerAnimState::Attack_Up:
		//空中攻撃が打てるか？
		if (m_bLockOn == true && m_pTargetEnemy && m_pTargetEnemy->m_bIsKnockUpping == true) {
			UE_LOG(LogTemp, Warning, TEXT("SKYKSSSS"));
			if (AttackAirs.IsValidIndex(m_attackCounter))
			{
				//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, m_pTargetEnemy->GetActorLocation().Z));
				attackAsset = AttackAirs[m_attackCounter];
				//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			}
			else
			{
				attackAsset = AttackAirs[0];
			}
		}
		//打てなかったら上段攻撃
		else
		{
			//有効チェック
			if (AttackUps.IsValidIndex(m_attackCounter))
			{
				attackAsset = AttackUps[m_attackCounter];
			}
			//有効でないなら
			else
			{
				attackAsset = AttackUps[0];
			}
		}

		break;
	case EPlayerAnimState::Attack_Under:
		//空中攻撃が打てるか？
		if (m_bLockOn==true&&m_pTargetEnemy&& m_pTargetEnemy->m_bIsKnockUpping==true) {
			UE_LOG(LogTemp, Warning, TEXT("SKYKSSSS"));
			if (AttackAirs.IsValidIndex(m_attackCounter))
			{
				//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, m_pTargetEnemy->GetActorLocation().Z));
				attackAsset = AttackAirs[m_attackCounter];
				//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			}
			else
			{
				attackAsset = AttackAirs[0];
			}
		}
		//打てなかったら下段攻撃
		else {
			//有効チェック
			if (AttackUnders.IsValidIndex(m_attackCounter))
			{
				attackAsset = AttackUnders[m_attackCounter];
			}
			//有効でないなら
			else
			{
				attackAsset = AttackUnders[0];
			}
		}
		break;

	case EPlayerAnimState::Attack_Parry:

		attackAsset = ParryAttack;
		break;

	default:
		break;
	}

	return attackAsset;
}

//HUDを登録する関数
void APlayPawn::SetHUDtoPlayer(AWire_HUD* _hud) {
	if (!_hud) {
		UE_LOG(LogTemp, Warning, TEXT("Erregular Set:HUD Class Is Nullptr In  SetHUDtoPlayer_PlayerPawn"));
		return;
	}
	HUD = _hud;

	//ItemCompにHUDを伝達
	//ItemComp->HUD = HUD;
}

//ターゲットエネミー取得
AEnemy_Base* APlayPawn::GetTargetEnemy(float _searchDistance, FVector _searchAngle, FVector _searcherPostion, float _nextTargetDirection, AEnemy_Base* _previousTarget)
{
	//戻り値用
	AEnemy_Base* enemy = nullptr;

	//有効チェック
	if (m_GetTargetEnemyDeleGate.IsBound())
	{
		enemy = m_GetTargetEnemyDeleGate.Execute(_searchDistance, _searchAngle, _searcherPostion, _nextTargetDirection, _previousTarget);
	}

	return enemy;
}

//攻撃の種類の取得
EAttackPattarnState APlayPawn::GetAttackPattarnState()
{
	//上下段攻撃廃止したため戻り値固定

	//戻り値用
	EAttackPattarnState state = EAttackPattarnState::None;

	//プレイヤーの状態が上段攻撃だったら
	if (m_ePlayerActionState == EPlayerActionState::Attack_Up)
	{
		//戻り値を上段攻撃に
		state = EAttackPattarnState::UpAttack;
	}
	//プレイヤーの状態が下段攻撃だったら
	else if (m_ePlayerActionState == EPlayerActionState::Attack_Under)
	{
		//戻り値を下段攻撃に
		state = EAttackPattarnState::DownAttack;
	}
	
	//戻り値を返す
	return EAttackPattarnState::UpAttack;
}

//アニメーションモンタージュの再生速度を取得
float APlayPawn::GetAnimMantegePlayRate()
{
	return m_montagePlayRate;
}

//---------------------------------------------------
//更新処理群
//---------------------------------------------------


//カメラの角度更新
void APlayPawn::UpdateCameraAngle(float _DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Camera Vector : %f , %f"), m_cameraAngle.X, m_cameraAngle.Y);

	if (!m_bDoUpdateCameraLocation) return;

	//落下中はカメラを引きにする☆菅澤追記
	//線形移動
	float targetCamLength = (m_ePlayerAnimState == EPlayerAnimState::Fall_Out) ? 1000.0f : 330.0f;
	float NextLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, targetCamLength, _DeltaTime, 5.f);
	SpringArmComp->TargetArmLength = NextLength;

	//カメラリセット時
	if (m_bResetingCamera && !m_bLockOn)
	{
		static bool isFirst = true;

		//プレイヤーの前方向
		static FRotator targetRot;

		if (isFirst)
		{
			targetRot = GetActorRotation();
			//Pitchの制限
			targetRot.Pitch = LockOnPitch;

			isFirst = false;
		}
		//線形補間
		FRotator newRot = FMath::RInterpTo(SpringArmComp->GetRelativeRotation(), targetRot, _DeltaTime, CameraResetSpeed);

		if (FVector::DotProduct(targetRot.Vector(), SpringArmComp->GetRelativeRotation().Vector()) > 0.99f)
		{
			m_bResetingCamera = false;
			isFirst = true;
		}

		//角度の設定
		SpringArmComp->SetRelativeRotation(newRot);

		return;
	}

	//--------------------------------------------------------------
	// ロックオン時
	//--------------------------------------------------------------

	//ロックオン時かつターゲットエネミーが有効なら
	if(m_bLockOn && m_pTargetEnemy)
	{
		//また死亡状態でなければ
		if (!m_pTargetEnemy->Tags.Contains(FName("Dead")))
		{
			//ターゲットエネミーの方向
			//（　エネミー位置 － プレイヤー位置　）. 正規化　＝　方向
			FVector targetDire = (m_pTargetEnemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();

			//	カメラの方向
			FRotator cameraRot = targetDire.Rotation();

			//ターゲットロック時のPitchを設定
			cameraRot.Pitch = LockOnPitch;

			//線形補間
			FRotator cameraAfterRot = FMath::Lerp(SpringArmComp->GetRelativeRotation().Vector(), cameraRot.Vector(), LockOnLerpAlpha).Rotation();

			//エネミー方法にカメラを向ける
			SpringArmComp->SetRelativeRotation(cameraAfterRot);
		}
		//死亡状態なら
		else
		{
			//ターゲットをクリア
			WidgetComp->SetVisibility(false);
			m_pTargetEnemy = nullptr;
			UE_LOG(LogTemp, Log, TEXT("Lock On Target Is Dead"))
		}
	}

	//ロックオン時かつ有効でなければ
	else if (m_bLockOn && !m_pTargetEnemy)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player : Target Enemy Is None!!"));

		//スプリングアームとプレイヤーの回転を内積し同じ向きを向いているかを判別
		if (FVector::DotProduct(SpringArmComp->GetRelativeRotation().Vector(), GetActorRotation().Vector()) <= 0.99)
		{
		//	//向いていないなら線形補間
		//	FVector lerpVec = FMath::Lerp(SpringArmComp->GetRelativeRotation().Vector(), GetActorRotation().Vector(), LockOnLerpAlpha);
		//	//FRotator newRot = FMath::RInterpTo(SpringArmComp->GetRelativeRotation(), FRotator::ZeroRotator, _DeltaTime, LockOnLerpAlpha);
		//	SpringArmComp->SetRelativeRotation(lerpVec.Rotation());
		}
		//向いているなら
		else
		{
			//ロックオン状態解除
			m_bLockOn = false;

			m_bResetingCamera = false;

			//UE_LOG(LogTemp, Log, TEXT("Look Forward"))
		}
	}

	

	//-------------------------------------------------------------------
	//通常時
	//-------------------------------------------------------------------
	else if (!m_bLockOn)
	{
		//入力された値をFRotatorに変換
		FRotator inputRot(m_cameraAngle.X, m_cameraAngle.Y, 0.0f);

		//入力がある時
		if (inputRot != FRotator::ZeroRotator)
		{
			//加速処理
			m_cameraRotationSpeed += CameraRotationAcceleration;

			//速度の上限を超えていたら
			if (m_cameraRotationSpeed > CameraRotationSpeedMax)
			{
				//上限速度にする
				m_cameraRotationSpeed = CameraRotationSpeedMax;
			}
		}
		//入力がない時
		else
		{
			//減速処理
			m_cameraRotationSpeed -= CameraRotationDeceleration;

			//速度が0以下になっていたら
			if (m_cameraRotationSpeed < 0.0f)
			{
				//ゼロに補正
				m_cameraRotationSpeed = 0.0f;
			}
		}

		//新しい角度
		FRotator newRot = inputRot * m_cameraRotationSpeed * _DeltaTime + SpringArmComp->GetRelativeRotation();

		//㍶での入力だった場合
		float mouseX, mouseY;
		GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(mouseX, mouseY);
		if (mouseX != 0.0f && mouseY != 0.0f)
		{
			newRot = inputRot * PCTurningSpeed + SpringArmComp->GetRelativeRotation();
		}

		//上下に角度制限
		newRot.Pitch = FMath::Clamp(newRot.Pitch, CameraPitchMin, CameraPitchMax);

		//適用
		SpringArmComp->SetRelativeRotation(newRot.Quaternion());
	}

	

	//入力値の初期化
	m_cameraAngle = FVector2D::ZeroVector;
}

//カメラの移動処理
void APlayPawn::UpdateCameraMove(float _DeltaTime)
{
	float fov = FMath::FInterpTo(CameraComp->FieldOfView, m_cameraFOV, _DeltaTime, 3.0f);

	CameraComp->FieldOfView = fov;

	float length = FMath::FInterpTo(SpringArmComp->TargetArmLength, m_springArmLength, _DeltaTime, 20.0f);

	SpringArmComp->TargetArmLength = length;

	if (m_bDoUpdateCameraLocation)
	{
		if (!CameraComp->GetAttachParent())
		{
			CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepWorldTransform);
			CameraComp->SetRelativeRotation(FRotator::ZeroRotator);
		}
		
		FVector pos = FMath::VInterpTo(CameraComp->GetRelativeLocation(), m_cameraRelativeLocation, _DeltaTime, 10.0f);
		CameraComp->SetRelativeLocation(pos);
		/*FVector pos = FMath::VInterpTo(CameraComp->GetComponentLocation(), SpringArmComp->GetSocketLocation(SpringArmComp->SocketName), _DeltaTime, 10.0f);
		CameraComp->SetWorldLocation(pos);*/
	}
	else
	{
		if (CameraComp->GetAttachParent())
		{
			CameraComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
}

//移動の更新
void APlayPawn::UpdateMoving()
{
	//待機状態か走り状態でなければ無視
	if (m_ePlayerActionState != EPlayerActionState::Idle)
	{
		return;
	}

	//☆菅澤追記
	if (m_ePlayerAnimState == EPlayerAnimState::Fall_Out||m_ePlayerAnimState==EPlayerAnimState::Fall_GlassBreak|| m_ePlayerAnimState == EPlayerAnimState::MontaInteract || m_ePlayerAnimState == EPlayerAnimState::Attack_Up) {
		return;
	}

	//入力値がゼロでなければ移動する
	if (m_moveDirection != FVector2D::ZeroVector)
	{
		SetDoUpdateCameraLocation(true);

		//プレイキャラクターの状態を移動状態を設定
		m_ePlayerAnimState = EPlayerAnimState::Run;

		// 入力された値を角度に変換したもの(弧度法)
		float inputAngle = FMath::Atan2(m_moveDirection.Y, m_moveDirection.X);

		//プレイヤーが向くべき方向(度数法)
		FRotator playerDirection = FRotator::ZeroRotator;

		//カメラがなければ無視
		if (!SpringArmComp) return;

		//カメラの向き
		FRotator cameraDirection = SpringArmComp->GetRelativeRotation();

		//プレイヤーが向くべき方向(Yaw) = 動く方向(Yaw) + スプリングアーム・カメラの向き(Yaw)
		playerDirection.Yaw = FMath::RadiansToDegrees(inputAngle) + cameraDirection.Yaw;

		//移動量の計算//////////////////////////////////////////////////////////
		//初期値はwalkSpeed
		float moveValue = m_walkSpeed;

		//補正値初期値は1
		float speedCorrectionValue = 1.0;

		//もしロックオン状態なら
		if (m_bLockOn)
		{
			//ロックオン時の移動速度補正値にする
			speedCorrectionValue = LockOnMoveSpeedCorrectionValue;
		}

		//移動
		AddMovementInput(playerDirection.Vector(), 2.5f);

		//カメラのズームアウト処理
		m_cameraFOV = DefaultCameraFOV * 1.07;
		m_springArmLength = DefaultSpringArmLength * 1.15;

		//プレイヤーの向き更新////////////////////////////////////////////////////
		//ロックオン時なら
		if (m_bLockOn)
		{
			//Yawだけを使う
			playerDirection.Yaw = cameraDirection.Yaw;
			//ターゲット方向に向く
			SetActorRotation(playerDirection.Quaternion());
		}
		//非ロックオン時なら
		else
		{
			//移動方向に向く
			SetActorRotation(playerDirection.Quaternion());
		}
	}
	//もし移動入力が無くプレイキャラクターの状態が走るであれば待機に変更
	else
	{
		if (m_ePlayerAnimState == EPlayerAnimState::Run)
		{
			m_ePlayerAnimState = EPlayerAnimState::Idle;
			m_ePlayerActionState = EPlayerActionState::Idle;

			//カメラのズームアウト解除処理
			m_cameraFOV = DefaultCameraFOV;
			m_springArmLength = DefaultSpringArmLength;
		}
	}

	//入力値を初期化
	m_moveDirection = FVector2D::ZeroVector;
}

//ターゲットマーカーの更新
void APlayPawn::UpdateTargetMarker()
{
	//カメラ方向に向ける
	WidgetComp->SetWorldRotation((CameraComp->GetForwardVector() * -1.0f).ToOrientationQuat());

	//ターゲットが有効なら
	if (m_pTargetEnemy)
	{

		UWidgetTargetMarker* targetMarker = Cast<UWidgetTargetMarker>(WidgetComp->GetUserWidgetObject());
		if (targetMarker)
		{
			targetMarker->SetTargetHPRatio(m_pTargetEnemy->GetHPRatio());
		}
	}
}

//バフの更新
void APlayPawn::UpdateBuffs(float _deltaTime)
{
	//バフがあるなら☆彡この条件式は無効です（EBuffPattern::Noneが入っているため）
	// if (Buffs.Num() > 0)
	//☆彡解決のための仮処理
	bool Is_BaffCheck=false;
	for (int i = 0;i < Buffs.Num();i++) {
		if (Buffs[i] != EBuffPattern::None) {
			Is_BaffCheck = true;
			break;
		}
	}
	if(Is_BaffCheck)
	{
		//ストップ中なら
		if (m_buffLifeTimer > 0.0f)
		{
			//バフの停止時間を更新
			m_buffLifeTimer -= _deltaTime;
			//UE_LOG(LogTemp, Log, TEXT("Stop Time : %f"), m_buffLifeTimer);
		}
		else
		{
			//バフ攻撃済みなら
			if (m_bDoBuffAttack)
			{
				//バフ攻撃獲得時間を更新
				m_buffAditionalTimer -= _deltaTime;

				//UE_LOG(LogTemp, Log, TEXT("Duration Time : %f"), m_buffAditionalTimer);
			}
			
		}

		//ビットの継続時間が少ないかつ既に点滅状態でなければ
		if (m_buffAditionalTimer+m_buffLifeTimer < 3.0f&&BuffBitManager->GetIsPoint()==false)
		{
			//点滅状態へ
			BuffBitManager->SetIsPoint(true);
			HUD->SetBuffIsPoint(true);
			//UE_LOG(LogTemp, Log, TEXT("NowwTime : %f"), m_buffAditionalTimer+ m_buffLifeTimer);

		}
		//そうでなければ点滅状態を切る
		else if (m_buffAditionalTimer + m_buffLifeTimer > 3.0f)
		{
			//点滅状態を切る
			BuffBitManager->SetIsPoint(false);
			HUD->SetBuffIsPoint(false);
			//UE_LOG(LogTemp, Warning, TEXT("GUYGUY"));
		}

		//バフの継続時間が0以下なら
		if (m_buffAditionalTimer <= 0.0 && m_buffLifeTimer <= 0.0f)
		{
			//バフをクリア
			ResetBuff();

			//ビットの点滅停止
			if (BuffBitManager->GetIsPoint())
			{
				BuffBitManager->SetIsPoint(false);
				HUD->SetBuffIsPoint(false);
			}

			//ビットにバフを通知
			BuffBitManager->SetBuffs(Buffs);

			//バフ攻撃をしたことがない状態に
			m_bDoBuffAttack = false;

			//UE_LOG(LogTemp, Log, TEXT("Buff Clear"));
		}
	}
}

//ガード強度の更新
void APlayPawn::UpdateGuardPower(float _deltaTime)
{
	if (m_ePlayerActionState == EPlayerActionState::Guard) return;

	GuardPowerFloat += GuardPowerIncreasePerSecond * _deltaTime;

	if (GuardPowerFloat >= 1.0f)
	{
		int addPoint = StaticCast<int>(GuardPowerFloat);

		GuardPowerFloat -= addPoint;

		GuardPower += addPoint;

		GuardPower = FMath::Clamp(GuardPower, 0, GuardPowerMax);
	}

	//武器に伝達
	if(GuardPower > GuardPowerMax * 0.5f &&m_pWeapon->GetIsPoint()){
		m_pWeapon->ChangeColor(m_pWeapon->Color);
	}
}

//回避処理の更新
void APlayPawn::UpdateAvoid(float _deltaTime)
{
	//回避での移動量ベクトル(コリジョンの位置座標で計算)
	//FVector moveVecByAvoid = GetCapsuleComponent()->GetComponentLocation() - AvoidBeginPosition;
	//移動距離(コリジョンの位置座標で計算)
	float  moveDis = (GetCapsuleComponent()->GetComponentLocation() - AvoidBeginPosition).Length();

	//新しい大サイズ
	FVector newSize(AvoidCollision->GetUnscaledBoxExtent());
	newSize.X = moveDis / 2.0f;
	//サイズ更新
	AvoidCollision->SetBoxExtent(newSize);

	//相対位置更新
	AvoidCollision->SetRelativeLocation(FVector(-moveDis / 2.0f, 0.0f, 0.0f));
}

//死亡処理の更新
void APlayPawn::UpdateDead(float _deltaTime)
{
	//死亡からの経過時間を更新
	m_deathDuration += _deltaTime;

	//もし死亡に掛かる時間に死亡からの経過時間が超えたら
	if (TimeTakenToDead <= m_deathDuration)
	{
		//リスポーン
		OnRespawn();
	}
}

//入力の更新
void APlayPawn::UpdateInput(float _deltaTime)
{
	
}

//----------------------------------------------------
// イベント関数
//----------------------------------------------------

//攻撃
void APlayPawn::OnAttack()
{
	//★新仕様試運転
	//敵に方向は吸い付く
	if (!m_pTargetEnemy||!m_bLockOn) {
		AEnemy_Base* target = GetTargetEnemy(300.0f, CameraComp->GetRightVector(), GetActorLocation());
		if (target) {
			FaceTargetLocation(target->GetActorLocation());
		}
	}
	else {
		FaceTargetLocation(m_pTargetEnemy->GetActorLocation());
	}

	//飛んでいたら
	if (m_bDoFly)
	{
		SetDoUpdateCameraLocation(true);
	}

	//もし上段アニメーション中なら
	if (m_ePlayerAnimState == EPlayerAnimState::Attack_Up)
	{
		//ステートを上段攻撃に
		m_ePlayerActionState = EPlayerActionState::Attack_Up;

		//攻撃バフを計算
		m_AttackBuffRate = ComboBuffRate * m_comboBuffNum;

		if (m_AttackBuffRate == 0.0f)
		{
			m_AttackBuffRate = 1.0f;
		}
	}
	//もし下段アニメーション中なら下段攻撃に
	else if (static_cast<uint8>(m_ePlayerAnimState) >= static_cast<uint8>(EPlayerAnimState::Attack_Under))
	{
		//ステートを下段攻撃に
		m_ePlayerActionState = EPlayerActionState::Attack_Under;

		//攻撃バフを初期化
		m_AttackBuffRate = 1.0f;

		//コンボバフ数が上限でなければ
		if (m_comboBuffNum < ComboBuffRimitNum)
		{
			//コンボバフ追加
			++m_comboBuffNum;
		}
	}

	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, AttackSE, GetActorLocation(), SE_Volume, 0.5f + 0.5 * m_comboBuffNum);

	//武器の設定
	//武器のコリジョンを有効にする
	m_pWeapon->SetAttacking(true);
	//エフェクトの設定
	m_pWeapon->SetBladeOrbitColor(FLinearColor::Blue * (0.1f + 9.9f / ComboBuffRimitNum * m_comboBuffNum));
	m_pWeapon->SetBladeOrbitWidth(10.0f + 60.0f / ComboBuffRimitNum * m_comboBuffNum);
	m_pWeapon->SetBladeOrbitEffectParam(50.0f / ComboBuffRimitNum * m_comboBuffNum, 50.0f + 100.0f / ComboBuffRimitNum * m_comboBuffNum);

	//デバック用
	//武器のコリジョンの可視化
	m_pWeapon->m_pWeaponCollision->bHiddenInGame = AttackCollisionHiddenInGame;

	// 範囲攻撃 /////////////////////////////////////////////////////////

	FAttackAsset attackAsset = GetAttackAsset();

	//もしコリジョンサイズがZeroVectorなら無視
	if (attackAsset.AreaCollisionSize != FVector::ZeroVector)
	{
		//コリジョンの位置を計算
		FVector collisionPos = attackAsset.AreaCollisionRelativeLocation;

		float yaw = FMath::DegreesToRadians(GetActorRotation().Yaw);

		collisionPos.X = FMath::Cos(yaw) * collisionPos.Length();
		collisionPos.Y = FMath::Sin(yaw) * collisionPos.Length();

		//絶対位置にする
		collisionPos += GetActorLocation();

		//位置(第一引数)：絶対位置にする為にプレイヤーの位置を足す
		DoAreaAttack(collisionPos, FRotator::ZeroRotator, attackAsset.AreaCollisionSize);
	}

	//UE_LOG(LogTemp, Warning, TEXT("PlayerActionState : %s"), *UEnum::GetValueAsString(m_ePlayerActionState));
	//UE_LOG(LogTemp, Warning, TEXT("ComboNum : %d"), m_comboBuffNum);

	//敵に通知
	/*ReceiveAttackScence(pattran, GetActorLocation(), GetActorForwardVector(),
		300, 150);*/
}

//攻撃終了時
void APlayPawn::OnFinishAttack()
{
	//飛んでいたら
	if (m_bDoFly)
	{
		SetDoUpdateCameraLocation(false);
	}

	//武器のコリジョンを被有効にする
	m_pWeapon->SetAttacking(false);
	m_pWeapon->m_pWeaponCollision->bHiddenInGame = true;

	//行動状態を待機にする
	if (m_ePlayerActionState != EPlayerActionState::Guard) {
		m_ePlayerActionState = EPlayerActionState::Idle;
	}

	// 追撃バフ処理フェーズ ///////////////////////////////////////////////
	if (Buffs.Contains(EBuffPattern::AdditionalAttack))
	{
		int attackNum = 0;

		//バフの分追撃する
		for (int i = 0; i < Buffs.Num(); ++i)
		{
			if (Buffs[i] == EBuffPattern::AdditionalAttack)
			{
				++attackNum;
			}
		}

		SpawnAttackActor(GetAttackAsset().AttackPoint * m_AttackBuffRate / 10, attackNum);
	}
	////////////////////////////////////////////////////////////////////////

	m_AttackBuffRate = 1.0f;

	//バフがあるなら☆彡この条件式は無効です（EBuffPattern::Noneが入っているため）
	// if (Buffs.Num() > 0)
	//☆彡解決のための仮処理
	bool Is_BaffCheck = false;
	for (int i = 0;i < Buffs.Num();i++) {
		if (Buffs[i] != EBuffPattern::None) {
			Is_BaffCheck = true;
			break;
		}
	}
	//バフがあるなら
	if (Is_BaffCheck)
	{
		//バフ攻撃済み
		m_bDoBuffAttack = true;
	}

	//バフの停止時間を初期化（最大にする）☆これいる？？？
	m_buffLifeTimer = BuffDurationStopTime;

	//下段攻撃終了時なら
	if (m_ePlayerAnimState == EPlayerAnimState::Attack_Under)
	{
		if (m_attackedActors.Num() > 0)
		{
			//バフ継続時間延長☆これだけでよくね？？？
			m_buffAditionalTimer = BuffDurationByUnderAttack * m_attackCounter;

			//UE_LOG(LogTemp, Log, TEXT("Add Buff Duration By UnderAttack : %f"), m_buffAditionalTimer)
		}

		//攻撃数追加
		++m_attackCounter;
	}

	//上段攻撃終了時なら
	if (m_ePlayerAnimState == EPlayerAnimState::Attack_Up)
	{

		if (m_attackedActors.Num() > 0)
		{
			//バフ継続時間延長☆これだけでよくね？？？☆彡後これ0.N秒の時やばくない？
			m_buffAditionalTimer *= BuffDurationRateByUpAttack;

			//UE_LOG(LogTemp, Log, TEXT("Add Buff Duration By UpAttack : %f"), m_buffAditionalTimer)
		}
		//コンボバフを切らす
		m_attackCounter = 0;
	}

	//空中攻撃継続用処理
	for (int i = 0; i < m_attackedActors.Num(); ++i)
	{
		//攻撃済みアクターが有効でなければ無視
		if (!m_attackedActors[i]) continue;
		//キャスト
		AFighterCharacter* target = Cast<AFighterCharacter>(m_attackedActors[i]);
		//もしファイターかつ打ち上げ中なら
		if (target && target->m_bIsKnockUpping && m_ePlayerAnimState != EPlayerAnimState::Attack_Up)
		{
			//よろけをいれる
			target->OnHitAttack(0, 0, EAttackPattarnState::UpAttack, Cast<AFighterCharacter>(this), true);
		}
	}

	//範囲攻撃を消す
	FinishAreaAttack();

	UE_LOG(LogTemp, Log, TEXT("ComboNum : %d"), m_attackCounter);
	//UE_LOG(LogTemp, Log, TEXT("ComboBuffNum : %d"), m_comboBuffNum);
}

//弾き
void APlayPawn::OnParry()
{
	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, ParrySE, GetActorLocation(), SE_Volume);

	//行動状態を弾きに
	m_ePlayerActionState = EPlayerActionState::Parry;

	//武器の軌道を有効化
	m_pWeapon->SetBladeOrbitActive(true);

	//UE_LOG(LogTemp, Warning, TEXT("Parry Start"));
}

//弾き終了時
void APlayPawn::OnFinishParry()
{
	//行動状態を待機にする
	m_ePlayerActionState = EPlayerActionState::Idle;

	//パリィ攻撃入力していたら
	if (m_bIsInputParryAttack && m_pParriedFighter)
	{
		
	}
	else
	{
		//武器の軌道を無効化
		m_pWeapon->SetBladeOrbitActive(false);
		//スロウを止めさせる
		EndSlow();
	}

	//フラグの初期化
	m_bCanParryAttack = false;
	m_bIsInputParryAttack = false;
}

//弾き攻撃時
void APlayPawn::OnParryAttack()
{
	if (!m_pParriedFighter) {
		//ステート変更
		m_ePlayerActionState = EPlayerActionState::Idle;

		//武器の軌道を無効化
		m_pWeapon->SetBladeOrbitActive(false);

		//無敵解除
		m_bInvincible = false;

		m_bDoUpdateCameraLocation = true;
		return;
	}

	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, ParryAttackSE, GetActorLocation(), SE_Volume);

	//範囲攻撃設置
	DoAreaAttack(m_pParriedFighter->GetActorLocation(), FRotator::ZeroRotator, FVector(200.0f, 200.0f, 100.0f));

	//エフェクトを出す
	if (AvoidEffect && AvoidEffectComp)
	{
		//回避エフェクトを出す
		AvoidEffectComp->SetAsset(AvoidEffect);
		AvoidEffectComp->SetActive(true);
	}

	//エフェクトを出す
	if (ParryAttackEffect)
	{
		UNiagaraComponent* niagaraComp;
		niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryAttackEffect, m_pParriedFighter->GetActorLocation());
	}
}

//弾き攻撃終了時
void APlayPawn::OnFinishParryAttack()
{
	//範囲攻撃終了
	FinishAreaAttack();

	//ステート変更
	m_ePlayerActionState = EPlayerActionState::Idle;

	//武器の軌道を無効化
	m_pWeapon->SetBladeOrbitActive(false);

	//エフェクトを消す
	AvoidEffectComp->SetActive(false);

	// Pawnチャンネルをブロックに戻す
	m_pParriedFighter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//パリィした対象をクリア
	m_pParriedFighter = nullptr;

	//無敵解除
	m_bInvincible = false;

	m_bDoUpdateCameraLocation = true;

	//攻撃済みのアクターをクリア
	ClearAttackedActors();
}

//回避時
void APlayPawn::OnAvoid()
{
	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, AvoidSE, GetActorLocation(), SE_Volume);

	//行動状態を回避に
	m_ePlayerActionState = EPlayerActionState::Avoidance;

	//回避開始時の座標を保存
	AvoidBeginPosition = GetCapsuleComponent()->GetComponentLocation();
	//回避用コリジョンの有効化
	AvoidCollision->SetActive(true);
	//回避エフェクトを出す
	AvoidEffectComp->SetAsset(AvoidEffect);
	AvoidEffectComp->SetActive(true);
}

//回避終了時
void APlayPawn::OnFinishAvoid()
{
	//行動状態を待機にする
	m_ePlayerActionState = EPlayerActionState::Idle;

	//回避用コリジョンのトランスフォームを初期化
	//サイズ
	float radius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float halfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	AvoidCollision->SetBoxExtent(FVector(radius, radius, halfHeight));
	//位置
	AvoidCollision->SetRelativeLocation(FVector::ZeroVector);

	//回避用コリジョンの非有効化
	AvoidCollision->SetActive(false);

	//バフ攻撃済みなら
	if (m_bDoBuffAttack)
	{
		//バフ停止時間を0に
		m_buffLifeTimer = 0.0f;
	}

	AvoidEffectComp->SetActive(false);
}

//致命ダメージ時
void APlayPawn::OnCriticalDamage()
{
	if (!m_pCriticalTarget) return;

	SpawnEffect(EAttackReaction::Hit);

	m_pCriticalTarget->Damaged(999999);
}

//致命終了時
void APlayPawn::OnFinishCriticalAttack()
{
	m_bInvincible = false;

	m_pWeapon->SetBladeOrbitActive(false);
}

//アニメーションモンタージュ終了時
void APlayPawn::OnFinishAnimMontage()
{
	//上段攻撃終了時ならコンボバフを切らす
	if (m_ePlayerAnimState == EPlayerAnimState::Attack_Up) {
		//UE_LOG(LogTemp, Warning, TEXT("ComboCOunt:%d"), m_comboBuffNum);
		m_comboBuffNum = 0;

		//カメラズーム終了
		m_springArmLength = DefaultSpringArmLength;
	}
	//アニメーション状態を待機にする
	if (m_ePlayerAnimState != EPlayerAnimState::Guard) {
		m_ePlayerAnimState = EPlayerAnimState::Idle;
		m_ePlayerActionState = EPlayerActionState::Idle;
	}

	//キャラクタームーブメントが歩きでないなら
	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking) {
		//歩きにする
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	//飛んでいたら
	if (m_bDoFly)
	{
		//カメラの移動を更新
		SetDoUpdateCameraLocation(true);
	}

	//UE_LOG(LogTemp, Log, TEXT("Fuckyou"));

	//攻撃数リセット
	m_attackCounter = 0;
}

//アニメーションでインタラクトを利用
void APlayPawn::ActivationInteractor() {
	if (m_nowTargetInteractor) {
		m_nowTargetInteractor->UseInteraction();
	}
}

//回復時
void APlayPawn::OnRecovery()
{
	HUD->ButtonPoint(5);

	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, RecoverySE, GetActorLocation(), SE_Volume);

	ItemComp->UseItem(EItemKinds::HpRecovery);

	//HP表現用アクターに通知
	HpExpressionActor->ChangeColor_ByHp(GetHPRatio());

	//HUDに減少を伝える
	HUD->SetPlayerHpPercent((float)m_hitPoint / (float)m_Max_HitPoint);
}

//死亡時
void APlayPawn::OnDead()
{
	//既に死亡しているまたは不死モードなら無視
	if (IsUndeadMode || m_ePlayerActionState == EPlayerActionState::Dead)
	{
		return;
	}

	//アニメーション再生されていても上書き
	m_ePlayerActionState = EPlayerActionState::Idle;
	//アニメーション再生
	PlayAnimMantage(DeadMontage);

	//ステート変更
	m_ePlayerAnimState = EPlayerAnimState::Dead;
	m_ePlayerActionState = EPlayerActionState::Dead;

	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, DeadSE, GetActorLocation(), SE_Volume);

	//無敵状態にする
	m_bInvincible = true;

	//バフ解除
	ResetBuff();

	//プレイヤーコントローラー取得
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	//フェードアウト処理
	pc->ClientSetCameraFade(
		  true
		, FColor::Black
		, FVector2D(0.0f, 1.0f)
		, TimeTakenToDead
		, true
		, true
	);
}

//リスポーン時
void APlayPawn::OnRespawn()
{
	//ステート変更
	m_ePlayerActionState = EPlayerActionState::Idle;
	m_ePlayerAnimState = EPlayerAnimState::Idle;

	//リスポーン地点に移動
	SetActorLocation(RespawnPoint);

	//回転を初期化
	SetActorRotation(FRotator::ZeroRotator);
	//ロックオン解除
	m_bLockOn = false;
	//カメラの角度リセット
	m_bResetingCamera = false;
	//バフリセット
	m_comboBuffNum = 0;
	m_attackCounter = 0;
	ResetBuff();

	//プレイヤーコントローラー取得
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	//フェードアウト処理
	pc->ClientSetCameraFade(
		true
		, FColor::Black
		, FVector2D(1.0f, 0.0f)
		, 0.5f
		, true
		, true
	);

	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, RespawnSE, GetActorLocation(), SE_Volume);

	//死亡経過時間の初期化
	m_deathDuration = 0.0f;

	//HPを初期化
	m_hitPoint = m_Max_HitPoint;

	//HP表現用アクターに通知
	HpExpressionActor->ChangeColor_ByHp(GetHPRatio());

	//HUDに減少を伝える
	HUD->SetPlayerHpPercent((float)m_hitPoint / (float)m_Max_HitPoint);

	//無敵状態解除
	m_bInvincible = false;
}

//攻撃ヒット時のイベント関数
void APlayPawn::OnAttackHitEvent()
{
	// カメラの揺れ ////////////////////////////////////////////
	if (m_ePlayerActionState == EPlayerActionState::Attack_Up)
	{
		DoCameraShaking(1.0f * m_comboBuffNum);
	}
}

//被攻撃時
EAttackReaction APlayPawn::OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp)
{
	//HUDの確認
	if (!HUD) {
		UE_LOG(LogTemp, Warning, TEXT("Not Found: HUD pointer In Player_Damaged"));
		return EAttackReaction::None;
	}
	//攻撃パターンが有効でなければ終了
	if (_attackState == EAttackPattarnState::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player : Attack Pattarn Is None"));
		return EAttackReaction::None;
	}

	////☆ノックバック自己判断
	//if (_state == EAttackPattarnState::UpAttack) {
	//	KnockBacked(GetActorForwardVector() * -200.0f, 0.25f);
	//}

	//自身の被攻撃時のリアクション
	EAttackReaction attackReaction = EAttackReaction::None;

	//ダメージを受けているか
	bool isDamage(true);

	//回避しているか
	bool isAvoid(false);

	//ガード判定の確認↓

	//上段判定
	if (m_ePlayerActionState == EPlayerActionState::Guard && _attackState == EAttackPattarnState::UpAttack)
	{
		isDamage = false;
	}
	//下段判定
	if (m_ePlayerActionState == EPlayerActionState::Guard && _attackState == EAttackPattarnState::DownAttack)
	{
		isDamage = false;
	}

	//回避判定
	if (m_ePlayerActionState == EPlayerActionState::Avoidance)
	{
		isDamage = false;
		isAvoid = true;
	}

	//プレイヤーが弾き状態なら
	if (m_ePlayerActionState == EPlayerActionState::Parry)
	{
		//SE再生
		UGameplayStatics::PlaySoundAtLocation(this, JustParrySE, GetActorLocation(), SE_Volume);

		//UE_LOG(LogTemp, Log, TEXT("Player Parrying"));
		
		//リアクションを弾きに
		attackReaction = EAttackReaction::Parry;

		//コンボバフを最大に
		 m_comboBuffNum = ComboBuffRimitNum;

		 //パリィ攻撃フラグ
		 m_bCanParryAttack = true;

		 if (!m_pParriedFighter)
		 {
			 m_pParriedFighter = _attacker;
		 }

		//スロウ突入☆菅澤加筆
		BeSlow(2.0f,0.1f);
	}
	else
	{
		//被弾していたら
		if (isDamage)
		{
			DoCameraShaking(10.0f);

			//UE_LOG(LogTemp, Log, TEXT("Player Hitting"));
			//ダメージ処理
			Damaged(_damage);
			//リアクションを当たったに
			attackReaction = EAttackReaction::Hit;

			Super::OnHitAttack(_damage, _breakPower, _attackState, _attacker, _canKnockUp);
		}
		//防御できていたら
		else if(! isDamage && !isAvoid)
		{
			DoCameraShaking(5.0f);

			//リアクションを防御に
			attackReaction = EAttackReaction::Guard;

			//バフの追加
			AddBuff(EBuffPattern::AdditionalAttack);

			//ガード強度の更新
			GuardPower -= _breakPower;
			
			//半分以下なら点滅
			if (GuardPower <= GuardPowerMax * 0.5f && m_pWeapon->GetIsPoint() == false) {
				m_pWeapon->ChangeColor(m_pWeapon->Color*0.05f,true);
			}
			//ガード強度が切れたら
			if (GuardPower <= 0)
			{
				m_ePlayerActionState = EPlayerActionState::Idle;
				//モンタージュを再生
				PlayAnimMantage(GuardBreakMontage);

				//ステート更新
				m_ePlayerActionState = EPlayerActionState::Stag;
				m_ePlayerAnimState = EPlayerAnimState::Stag;

				//点滅やめ
				if (GuardPower > GuardPowerMax * 0.5f && m_pWeapon->GetIsPoint()) {
					m_pWeapon->ChangeColor(m_pWeapon->Color);
				}

				//SE再生
				UGameplayStatics::PlaySoundAtLocation(this, GuardBreakSE, GetActorLocation(), SE_Volume);
			}
			else
			{
				//SE再生
				UGameplayStatics::PlaySoundAtLocation(this, JustGuardSE, GetActorLocation(), SE_Volume);
			}
		}
		//回避成功時
		else if (!isDamage && isAvoid)
		{
			//SE再生
			UGameplayStatics::PlaySoundAtLocation(this, JustAvoidSE, GetActorLocation(), SE_Volume);

			//リアクションを無しに
			attackReaction = EAttackReaction::None;

			//バフの追加
			AddBuff(EBuffPattern::SpeedUp);

			//スローにする
			BeSlow(2.0f,0.1f);
		}
	}

	SpawnEffect(attackReaction);

	//リアクションを攻撃側に返す
	return attackReaction;
}

//被ダメージ時
void APlayPawn::Damaged(int _damagePoint)
{
	//無敵なら無視
	if (m_bInvincible) return;

	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, DamageSE, GetActorLocation(), SE_Volume);

	//バフ解除
	ResetBuff();

	//hp減少減少
	m_hitPoint -= _damagePoint;

	//死亡判定
	if (m_hitPoint <= 0)
	{
		OnDead();
	}

	//HP表現用アクターに通知
	HpExpressionActor->ChangeColor_ByHp(GetHPRatio());

	//HPが減ったのでHUDを出す
	ShowDamageOverlay();

	//HUDに減少を伝える
	HUD->SetPlayerHpPercent((float)m_hitPoint / (float)m_Max_HitPoint);
	//UE_LOG(LogTemp, Log, TEXT("Player Damage : player hp %d"), m_hitPoint);
}

//弾かれたとき
void APlayPawn::Parried()
{
	Super::Parried();
}

//上段攻撃入力時
void APlayPawn::OnInputAttackUp()
{
	//落下中は許さない
	if (m_ePlayerAnimState == EPlayerAnimState::Fall_GlassBreak || m_ePlayerAnimState == EPlayerAnimState::Fall_Out || m_ePlayerAnimState == EPlayerAnimState::Attack_Up) {
		return;
	}
	//スロウ中はパリィにのみ割り込み可能★菅澤加筆
	//待機状態なら攻撃する
	if (m_ePlayerActionState == EPlayerActionState::Idle || (m_ePlayerActionState == EPlayerActionState::Avoidance || m_ePlayerActionState == EPlayerActionState::Parry && m_IsSlow))
	{
		// 空中攻撃 ///////////////////////////////////////////////////////////////////////////////////////////////
		//打ち上げた敵がいるなら
		if (m_pKnockCaughtUpEnemies.Num() > 0)
		{
			//有効チェック
			if (!AttackStrongAirs.AnimMantage) {
				UE_LOG(LogTemp, Warning, TEXT("PlayPawn : StrongAirs_Is_NULL"));
				return;
			}

			//攻撃のメインターゲットを選出
			AFighterCharacter* targetChara = nullptr;
			for (int i = m_pKnockCaughtUpEnemies.Num() - 1; 0 <= i; --i)
			{
				if (m_pKnockCaughtUpEnemies[i] != nullptr && m_pKnockCaughtUpEnemies[i]->m_bIsKnockUpping)
				{
					targetChara = m_pKnockCaughtUpEnemies[i];
				}
				else
				{
					m_pKnockCaughtUpEnemies.RemoveAt(i);
					UE_LOG(LogTemp, Warning, TEXT("PlayPawn : Not Target"));
				}
			}
			if (!targetChara)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayPawn : target Is Null"));
				m_pKnockCaughtUpEnemies.Empty();

				//流そうとしているアニメーションの有効チェック
				if (AttackUps.IsValidIndex(m_comboBuffNum))
				{
					PlayAnimMantage(AttackUps[m_comboBuffNum].AnimMantage);
				}
				//バフ数が要素数を上回っていたら
				else if (AttackUps.Num() - 1 < m_comboBuffNum)
				{
					PlayAnimMantage(AttackUps[AttackUps.Num() - 1].AnimMantage);
				}

				return;
			}

			//アニメーション再生
			PlayAnimMantage(AttackStrongAirs.AnimMantage);
			UE_LOG(LogTemp, Warning, TEXT("PlayPawn : Play StrongAirs Anim"));
			//キャラクタームーブメントの設定を飛んでいるに変更
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

			//メインターゲットの近くに飛ぶ
			FVector Vec = GetActorLocation() - targetChara->GetActorLocation();
			Vec.Normalize();
			FVector AtPos = targetChara->GetActorLocation() + Vec * 90.0f;

			//カメラ
			SetDoUpdateCameraLocation(true);

			//移動
			SetActorLocation(FVector(AtPos.X, AtPos.Y, targetChara->GetActorLocation().Z));

			//変数の初期化
			m_AirTargetEnemys.Empty();
			m_pKnockCaughtUpEnemies.Empty();
			m_bDoFly = true;
		}
		// 地上攻撃 ///////////////////////////////////////////////////////////////////////////////////////////////
		else if (m_pKnockCaughtUpEnemies.Num() == 0)
		{
			//流そうとしているアニメーションの有効チェック
			if (AttackUps.IsValidIndex(m_comboBuffNum))
			{
				PlayAnimMantage(AttackUps[m_comboBuffNum].AnimMantage);
			}
			//バフ数が要素数を上回っていたら
			else if (AttackUps.Num() - 1 < m_comboBuffNum)
			{
				PlayAnimMantage(AttackUps[AttackUps.Num() - 1].AnimMantage);
			}
		}
		//ステート更新
		m_ePlayerAnimState = EPlayerAnimState::Attack_Up;
		m_ePlayerActionState = EPlayerActionState::Ready;

		//スロウを止めさせる☆菅澤加筆
		EndSlow();
	}
}

//下段攻撃入力時
void APlayPawn::OnInputAttackUnder()
{
	//落下中は許さない
	if (m_ePlayerAnimState == EPlayerAnimState::Fall_GlassBreak || m_ePlayerAnimState == EPlayerAnimState::Fall_Out) {
		return;
	}
	//スロウ中はパリィにのみ割り込み可能★菅澤加筆
	//待機状態なら攻撃する
	if (m_ePlayerActionState == EPlayerActionState::Idle || (m_ePlayerActionState == EPlayerActionState::Avoidance || m_ePlayerActionState == EPlayerActionState::Parry && m_IsSlow))
	{
		// 空中攻撃 ///////////////////////////////////////////////////////////////////////////////////////////////
		//打ち上げた敵がいるならなら
		if (m_pKnockCaughtUpEnemies.Num() > 0)
		{
			//有効チェック
			if (!AttackAirs.IsValidIndex(m_attackCounter)) {
				UE_LOG(LogTemp, Warning, TEXT("AirCombo_Is_NULL:%d"),m_attackCounter);
				return;
			}

			//攻撃のメインターゲットを選出
			AFighterCharacter* targetChara = nullptr;
			for (int i = m_pKnockCaughtUpEnemies.Num() - 1; 0 <= i; --i)
			{
				if(m_pKnockCaughtUpEnemies[i] != nullptr && m_pKnockCaughtUpEnemies[i]->m_bIsKnockUpping)
				{
					targetChara = m_pKnockCaughtUpEnemies[i];
				}
				else
				{
					m_pKnockCaughtUpEnemies.RemoveAt(i);
					//UE_LOG(LogTemp, Warning, TEXT("PlayPawn : Not Target"));
				}
			}
			if (!targetChara)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayPawn : target Is Null"));
				m_pKnockCaughtUpEnemies.Empty();
				return;
			}

			//アニメーション再生
			PlayAnimMantage(AttackAirs[m_attackCounter].AnimMantage);
			//キャラクタームーブメントの設定を飛んでいるに変更
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

			//メインターゲットの近くに飛ぶ
			FVector Vec = GetActorLocation() - targetChara->GetActorLocation();
			Vec.Normalize();
			FVector AtPos = targetChara->GetActorLocation()+Vec * 90.0f;

			//カメラ
			SetDoUpdateCameraLocation(false);

			//移動
			SetActorLocation(FVector(AtPos.X, AtPos.Y, targetChara->GetActorLocation().Z));

			//変数の初期化
			m_AirTargetEnemys.Empty();
			m_pKnockCaughtUpEnemies.Empty();
			m_bDoFly = true;
		}
		// 地上攻撃 ///////////////////////////////////////////////////////////////////////////////////////////////
		if(m_pKnockCaughtUpEnemies.Num() == 0)
		{
			//流そうとしているアニメーションが有効でなければ終了
			if (!AttackUnders.IsValidIndex(m_attackCounter)) {
				UE_LOG(LogTemp, Warning, TEXT("Combo_Is_NULL:%d"), m_attackCounter);
				return;
			}

			PlayAnimMantage(AttackUnders[m_attackCounter].AnimMantage);
		}
		m_ePlayerAnimState = EPlayerAnimState::Attack_Under;
		m_ePlayerActionState = EPlayerActionState::Ready;

		//スロウを止めさせる☆菅澤加筆
		EndSlow();
	}
}

//パリィ攻撃入力時
void APlayPawn::OnInputParryAttack()
{
	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, ParryAttackInputSE, GetActorLocation(), SE_Volume);

	//m_bIsInputParryAttack = true;

	m_ePlayerActionState = EPlayerActionState::Idle;

	//アニメーション再生
	PlayAnimMantage(ParryAttack.AnimMantage);

	//ステート変更
	m_ePlayerActionState = EPlayerActionState::Attack_Parry;
	m_ePlayerAnimState = EPlayerAnimState::Attack_Parry;

	// Pawnチャンネルをオーバーラップに変更
	m_pParriedFighter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//無敵にする
	m_bInvincible = true;

	//エフェクトを出す
	if (ParryAttackActivationEffect)
	{
		UNiagaraComponent* niagaraComp;
		niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryAttackActivationEffect, m_pWeapon->GetActorLocation());
		//色の変更
		niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor(0.0f, 0.0f, 10.0f));
		//サイズの変更
		niagaraComp->SetVariableFloat(TEXT("Size"), 300.0f);
	}

	//フラグの初期化
	m_bCanParryAttack = false;
	m_bIsInputParryAttack = false;
	m_bDoUpdateCameraLocation = false;

	//スロウを止めさせる
	EndSlow();
}

//上段防御入力時
void APlayPawn::OnInputGuard()
{
	//落下中は許さない
	if (m_ePlayerAnimState == EPlayerAnimState::Fall_GlassBreak || m_ePlayerAnimState == EPlayerAnimState::Fall_Out) {
		return;
	}
	//待機状態なら
	if (m_ePlayerActionState == EPlayerActionState::Idle)
	{
		m_ePlayerAnimState = EPlayerAnimState::Guard;
		m_ePlayerActionState = EPlayerActionState::Guard;

		//カメラをリセット
		m_bResetingCamera = true;

		//スロウを止めさせる☆菅澤加筆
		EndSlow();
	}
}

//----------------------------------------------------
//入力関係
//----------------------------------------------------

void APlayPawn::InputCameraAngle(const FInputActionValue& _Value = FInputActionValue())
{
	m_cameraAngle = _Value.Get<FVector2D>();

	//UE_LOG(LogTemp, Warning, TEXT("Camera Vector : %f , %f"), m_cameraAngle.X, m_cameraAngle.Y);
}

//カメラ入力終了
void APlayPawn::InputCameraAngleFin(const FInputActionValue& _Value = FInputActionValue())
{
	m_cameraAngle = _Value.Get<FVector2D>();
}

void APlayPawn::InputMoveDirection(const FInputActionValue& _Value = FInputActionValue())
{
	m_moveDirection = _Value.Get<FVector2D>();

	//UE_LOG(LogTemp, Warning, TEXT("Move Vector : %f , %f"), m_moveDirection.X, m_moveDirection.Y);
}

//攻撃入力
void APlayPawn::InputAttackUp(const FInputActionValue& _Value = FInputActionValue())
{
	

	//パリィ攻撃可能時なら
	if (m_bCanParryAttack)
	{
		OnInputParryAttack();

		return;
	}

	HUD->ButtonPoint(1);

	//上段攻撃
	OnInputAttackUp();
}

void APlayPawn::InputAttackUnder(const FInputActionValue& _Value = FInputActionValue())
{
	//パリィ攻撃可能時なら
	if (m_bCanParryAttack)
	{
		OnInputParryAttack();

		return;
	}

	HUD->ButtonPoint(0);

	//下段攻撃
	OnInputAttackUnder();
}

//防御入力
void APlayPawn::InputGuard(const FInputActionValue& _Value = FInputActionValue())
{
	// 10/1仕様変更により機能を停止

	////防御
	//OnInputGuard();

	//HUD->ButtonPoint(2);

	////バフ攻撃済みなら
	//if (m_bDoBuffAttack)
	//{
	//	//バフ停止時間を0に
	//	m_buffLifeTimer = 0.0f;
	//}

	//カメラの角度リセット
	m_bResetingCamera = true;
}

//防御入力終了
void APlayPawn::InputGuardFin(const FInputActionValue& _Value = FInputActionValue())
{
	// 10/1仕様変更により機能を停止

	//if (m_ePlayerAnimState == EPlayerAnimState::Guard)
	//{
	//	//UE_LOG(LogTemp, Log, TEXT("Guard  Fin"))
	//	m_ePlayerAnimState = EPlayerAnimState::Idle;
	//	m_ePlayerActionState = EPlayerActionState::Idle;

	//	//カメラの角度リセットを停止
	//	m_bResetingCamera = false;
	//}

	//カメラの角度リセットを停止
	//m_bResetingCamera = false;
}

//上段弾き入力
void APlayPawn::InputParryUp(const FInputActionValue& _Value = FInputActionValue())
{
	//
	if (m_ePlayerActionState == EPlayerActionState::Idle)
	{
		HUD->ButtonPoint(4);
		PlayAnimMantage(ParryMontage);

		m_ePlayerAnimState = EPlayerAnimState::Parry_Up;
		m_ePlayerActionState = EPlayerActionState::Ready;
	}
}

//回避入力
void APlayPawn::InputAvoidance(const FInputActionValue& _Value = FInputActionValue())
{
	//落下中は許さない
	if (m_ePlayerAnimState == EPlayerAnimState::Fall_GlassBreak || m_ePlayerAnimState == EPlayerAnimState::Fall_Out) {
		return;
	}
	if (m_ePlayerActionState == EPlayerActionState::Idle && m_ePlayerAnimState != EPlayerAnimState::Avoidance)
	{
		HUD->ButtonPoint(3);

		FVector v = GetLastMovementInputVector();
		v.Normalize();

		SetActorRotation(v.Rotation());

		//アニメーション再生
		PlayAnimMantage(AvoidanceMontage);

		m_ePlayerAnimState = EPlayerAnimState::Avoidance;
		m_ePlayerActionState = EPlayerActionState::Ready;

		//加速度を与える
		LaunchCharacter(v * AvoidanceMoveSpeed, true, true);
	}
}

//ロックオン入力
void APlayPawn::InputLockOn(const FInputActionValue& _Value = FInputActionValue())
{
	if (m_bLockOn)
	{
		m_bLockOn = false;
		WidgetComp->SetVisibility(false);
		//UE_LOG(LogTemp, Warning, TEXT("Lock On : False"))

		UE_LOG(LogTemp, Log, TEXT("Lock On Clear"))
	}
	else
	{
		//ターゲット取得
		m_pTargetEnemy = GetTargetEnemy(LockOnDistance, CameraComp->GetRightVector(), GetActorLocation());
		if (m_pTargetEnemy)
		{
			HUD->ButtonPoint(7);
			m_bLockOn = true;
			WidgetComp->AttachToComponent(m_pTargetEnemy->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			WidgetComp->SetRelativeLocation(FVector::ZeroVector);
			WidgetComp->SetVisibility(true);

			UE_LOG(LogTemp, Warning, TEXT("Lock On : True"));

			UE_LOG(LogTemp, Log, TEXT("Player : Lock On Target Is %s"), *m_pTargetEnemy->GetName());
		}
		else
		{
			//WidgetComp->SetVisibility(false);
			UE_LOG(LogTemp, Log, TEXT("Player : Lock On Target Is Nullptr"));
		}
	}
}

//カメラターゲット切り替え入力
void APlayPawn::InputCameraShift(const FInputActionValue& _Value = FInputActionValue())
{
	if (m_bLockOn && m_bCameraShift)
	{
		//ターゲット取得
		m_pTargetEnemy = GetTargetEnemy(LockOnDistance, CameraComp->GetRightVector(), GetActorLocation(), _Value.Get<float>(), m_pTargetEnemy);
		UE_LOG(LogTemp, Log, TEXT("Player : Target Shift!  | Value Is %f"), _Value.Get<float>());

		if (m_pTargetEnemy)
		{
			HUD->ButtonPoint(8);
			m_bCameraShift = false;

			WidgetComp->AttachToComponent(m_pTargetEnemy->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			WidgetComp->SetRelativeLocation(FVector::ZeroVector);
			WidgetComp->SetVisibility(true);

			UE_LOG(LogTemp, Log, TEXT("Player : Lock On Target Is %s"), *m_pTargetEnemy->GetName());
		}
		else
		{
			WidgetComp->SetVisibility(false);
			UE_LOG(LogTemp, Log, TEXT("Player : Lock On Target Is Nullptr"));
		}
	}
}

//カメラターゲット切り替え入力終了
void APlayPawn::InputCameraShiftFin(const FInputActionValue& _Value = FInputActionValue())
{
	m_bCameraShift = true;
}

//回復入力時
void APlayPawn::InputRecovery(const FInputActionValue& _Value = FInputActionValue())
{
	if (m_ePlayerAnimState != EPlayerAnimState::Idle && m_ePlayerAnimState != EPlayerAnimState::Run)
	{
		return;
	}
	//回復アイテムに残量が無ければ無視
	if (ItemComp->GetItemNumber(EItemKinds::HpRecovery) <= 0)
	{
		return;
	}

	m_ePlayerActionState = EPlayerActionState::Idle;

	PlayAnimMontage(RecoveryMontage);

	m_ePlayerActionState = EPlayerActionState::Recovery;
	m_ePlayerAnimState = EPlayerAnimState::Recovery;
}

//シフト入力
void APlayPawn::InputShift(const FInputActionValue& _Value = FInputActionValue())
{
	m_bShiftOn = true;
	//UE_LOG(LogTemp, Log, TEXT("Shift On"));
}

//シフト入力終了
void APlayPawn::InputShiftFin(const FInputActionValue& _Value = FInputActionValue())
{
	m_bShiftOn = false;
	//UE_LOG(LogTemp, Log, TEXT("Shift Off"));
}

//インタラクション入力☆菅澤追記
void APlayPawn::InputInteraction(const FInputActionValue& _Value = FInputActionValue()) {
	HUD->ButtonPoint(6);
	InputInteractionOrigin();
}

void APlayPawn::InputInteractionOrigin() {
	UE_LOG(LogTemp, Warning, TEXT("SwitchCalled!"));
	if (m_nowTargetInteractor) {
		m_nowTargetInteractor->BeUnVisibleWidget();
		if (m_nowTargetInteractor->m_UseMontage == true) {
			UAnimMontage* Target = m_nowTargetInteractor->GetInteractMontage();
			if (Target) {
				m_InteractTargetMontage = Target;

				if (m_InteractTargetMontage) {
					PlayAnimMontage(m_InteractTargetMontage);
				}

				m_ePlayerAnimState = EPlayerAnimState::MontaInteract;
			}
		}
		else {
			m_nowTargetInteractor->UseInteraction();
		}
	}
}

//----------------------------------------------------
//衝突判定関係
//----------------------------------------------------

//武器の重なった時のイベント
void APlayPawn::OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult)
{
	// 致命 //////////////////////////////////////////////////////////////////////////////////////////////

	//致命攻撃対象
	m_pCriticalTarget = Cast<AFighterCharacter>(_otherActor);

	//致命攻撃の処理
	if (_otherActor->Tags.Contains("Enemy") && m_pCriticalTarget && m_pCriticalTarget->m_bEnableCritical)
	{
		// 敵の向きが致命可能か判定 ////////////////////////

		//内積の結果
		float dotResult = (float)FVector::DotProduct(this->GetActorForwardVector(), _otherActor->GetActorForwardVector());

		//致命成功時
		if (dotResult >= 0.8f)
		{
			OnFinishAttack();
			BeSlow(1.5f, 0.375f);

			m_ePlayerAnimState = EPlayerAnimState::Idle;
			m_ePlayerActionState = EPlayerActionState::Idle;

			PlayAnimMantage(CriticalAttack.AnimMantage);

			m_ePlayerAnimState = EPlayerAnimState::CriticalAttack;
			m_ePlayerActionState = EPlayerActionState::CriticalAttack;

			m_pCriticalTarget->OnCriticalAttacked(99999);

			m_bInvincible = true;

			UE_LOG(LogTemp, Log, TEXT("Critical !!"));

			return;
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("Nooooooob  Dot Is %f"), dotResult);
		}
	}

	m_AirTargetEnemys.Add(Cast<AEnemy_Base>(m_pCriticalTarget));

	// 打ち上げ攻撃処理 ///////////////////////////////////////////////////////////////

	//打ち上げ攻撃か
	if (GetCanKnockUp() == true && Cast<AFighterCharacter>(_otherActor) && _otherActor->Tags.Contains("Enemy"))
	{
		m_pKnockCaughtUpEnemies.Add(Cast<AFighterCharacter>(_otherActor));
	}

	if (m_ePlayerActionState == EPlayerActionState::Attack_Up)
	{
		UE_LOG(LogTemp, Log, TEXT("PL : Fly Attack Hit"));
		AFighterCharacter* chara = Cast<AFighterCharacter>(_otherActor);
		if (chara && chara->m_bIsKnockUpping)
		{
			chara->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}

	// 親クラスの処理 ///////////////////////////////////////////////////////////////
	Super::OnWeaponOverlap(_overlappedComponent, _otherActor,
		_otherComp, _otherBodyIndex, _bFromSweep, _sweepResult);

	
}

//予備動作報告
void APlayPawn::ReceiveAttackScence(EAttackPattarnState _pattarn, FVector _PlayerPos, FVector _playerFrontVec, float _AttackRange, float _AttackDeg) {
	if (m_AttackScenceDeleGate.IsBound())
	{
		m_AttackScenceDeleGate.Execute(_pattarn, _PlayerPos, _playerFrontVec, _AttackRange, _AttackDeg);
	}
}

//インタラクションオブジェクトとの重なりの検知用☆菅澤追記
void APlayPawn::OnBeginOverlap(
	UPrimitiveComponent* _overlappedComponent,
	AActor* _otherActor,
	UPrimitiveComponent* _otherComp,
	int32 _otherBodyIndex,
	bool _bFromSweep,
	const FHitResult& _sweepResult) {
	//対象物がインタラクション用アクターなら登録確認
	if (AInteractionActor* Act = Cast<AInteractionActor>(_otherActor)) {
		UE_LOG(LogTemp, Warning, TEXT("SwitchHited"));
		//有効なものなら登録
		if (Act->GetIsValiable()) {
			m_nowTargetInteractor = Act;
		}
	}
}
//☆菅澤追記
void APlayPawn::OnEndOverlap(
	UPrimitiveComponent* _overlappedComponent,
	AActor* _otherActor,
	UPrimitiveComponent* _otherComp,
	int32 _otherBodyIndex) {
	//離れたものが現在対象としているインタラクションアクターなら忘却
	if (_otherActor == m_nowTargetInteractor) {
		UE_LOG(LogTemp, Warning, TEXT("SwitchEscaped"));
		m_nowTargetInteractor = nullptr;
	}
}

//--------------------------------------------------------

//スロウ状態にする関数☆菅澤加筆
void APlayPawn::BeSlow(float _slowTime,float _slowPower) {

	//スロウ状態に
	m_IsSlow = true;

	// 時間をスロウに
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), _slowPower);

	// 指定時間後に元に戻すタイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		TimeSlowTimerHandle,
		this,
		&APlayPawn::EndSlow,
		_slowTime * _slowPower, // ← 実時間で何秒にするか
		false
	);
	//ポストプロセスでクソ明るくor暗く?★どっちがいい？
	CameraComp->PostProcessSettings.AutoExposureBias = 12.0f;
}

//スロウ状態を終える関数☆菅澤加筆
void APlayPawn::EndSlow() {
	if (!m_IsSlow) {
		return;
	}
	// 時間を元に
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	//スロウ状態を解除状態
	m_IsSlow = false;
	//ポストプロセスをもとに戻す
	CameraComp->PostProcessSettings.AutoExposureBias = 10.0f;
}

//（ダメージ時に呼ぶ）グリッジポストプロセスを出現させる関数
void APlayPawn::ShowDamageOverlay() {
	if (!DamageOverlayInstance) {
		return;
	}

	DamageOverlayInstance -> SetScalarParameterValue(TEXT("DamageAmount"), 1.0f);

	FTimerHandle OverlayTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(OverlayTimerHandle, [this]() 
	{
			if (DamageOverlayInstance) { 
				DamageOverlayInstance->SetScalarParameterValue(TEXT("DamageAmount"), 0.0f);
			}
	}, DamageOverlaySowTime, false);
}