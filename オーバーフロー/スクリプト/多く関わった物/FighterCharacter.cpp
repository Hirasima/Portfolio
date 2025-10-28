// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterCharacter.h"
#include "FighterPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFighterCharacter::AFighterCharacter()
	:m_AttackBuffRate(1.0f)
	,m_montagePlayRate(1.0f)
	,m_bEnableCritical(true)
	,m_bIsKnockUpping(false)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void AFighterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//武器の初期化
	if (m_pWeaponClass) {
		m_pWeapon = GetWorld()->SpawnActor<AWeapon>(m_pWeaponClass);
		m_pWeapon->GetRootComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("R_WeaponSocket"));
		m_pWeapon->m_pWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AFighterCharacter::OnWeaponOverlap);
		m_pWeapon->SetAttacking(false);
	}

	//鞘の初期化
	if (m_pSheathClass) {
		m_pSheath = GetWorld()->SpawnActor<AWeapon>(m_pSheathClass);
		m_pSheath->GetRootComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("L_WeaponSocket"));
		m_pSheath->m_pWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AFighterCharacter::OnWeaponOverlap);
		m_pSheath->SetAttacking(false);
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	UE_LOG(LogTemp, Log, TEXT("Channel"));

	//アニムインスタンスの取得
	m_pAnimInstance = GetMesh()->GetAnimInstance();
	//アニムインスタンスのヌルチェック
	if (!m_pAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("%s PlayPawn : Anim Instance Is None"), *this->GetName());
	}

	//HPの初期化
	m_hitPoint = m_Max_HitPoint;
	//UE_LOG(LogTemp, Warning, TEXT("HP Init : %d"), m_hitPoint);
}

// Called every frame
void AFighterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ノックバック更新処理
	UpdateKnockBack(DeltaTime);

	//ヒットポイントが０以下であれば「Dead」タグを追加
	if (m_hitPoint <= 0)
	{
		Tags.Add("Dead");
	}
}

// Called to bind functionality to input
void AFighterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//攻撃されたアクターのリストの中身を消す
void AFighterCharacter::ClearAttackedActors()
{
	m_attackedActors.Empty();
}

//エフェクトを出す
void AFighterCharacter::SpawnEffect(EAttackReaction _attackReaction)
{
	
}

//ノックバック中更新関数☆菅澤加筆
void AFighterCharacter::UpdateKnockBack(float DeltaTime) {
	if (!bIsKnockBack) {
		return;
	}

	//ノックバック座標の計算と反映
	KnockBackElapsed += DeltaTime;
	float Alpha = FMath::Clamp(KnockBackElapsed / KnockBackDuration, 0.f, 1.0f);
	FVector NextPos = FMath::Lerp(KnockBackStartPos, KnockBackEndPos, Alpha);
	SetActorLocation(NextPos);

	//ノックバックの修了確認
	if (Alpha >= 1.0f) {
		bIsKnockBack = false;
	}
}

void AFighterCharacter::PlayAnimMantage(UAnimMontage* _animMontage)
{
	//ヌルチェック
	if (!_animMontage)
	{
		return;
	}

	//アニメーションモンタージュを再生
	m_pAnimInstance->Montage_Play(_animMontage, m_montagePlayRate);
}

//---------------------------------------------------
// ゲッター ・　セッター
//---------------------------------------------------

//攻撃の種類の取得
EAttackPattarnState AFighterCharacter::GetAttackPattarnState()
{
	return EAttackPattarnState::None;
}

//現在の状態に対応するAttackAssetを返す
FAttackAsset AFighterCharacter::GetAttackAsset()
{
	FAttackAsset attackAsset;

	return attackAsset;
}

//ガード強度を削る力を返す
int AFighterCharacter::GetGuardBreakPower()
{
	return 20;
}

//打ち上げ攻撃可能か返す
bool AFighterCharacter::GetCanKnockUp()
{
	return false;
}

//現在のHP割合を返す
float AFighterCharacter::GetHPRatio()
{
	float ratio = (float)m_hitPoint / (float)m_Max_HitPoint;

	//UE_LOG(LogTemp, Log, TEXT("HP Ratio : %f"), ratio);

	return ratio;
}

//----------------------------------------------------
// イベント関数
//----------------------------------------------------

//攻撃を受けたとき
EAttackReaction AFighterCharacter::OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp)
{
	//打ち上げ攻撃だったら
	if (_canKnockUp)
	{
		m_bIsKnockUpping = true;
		UE_LOG(LogTemp, Log, TEXT("%s : KnockUpping!!!!!!!!!!!!!!"), *this->GetName());
	}

	return EAttackReaction::None;
}

//被ダメージ時
void AFighterCharacter::Damaged(int _damagePoint)
{
	m_hitPoint -= _damagePoint;
}

//被致命攻撃時
void AFighterCharacter::OnCriticalAttacked(int _damage)
{
	Damaged(_damage);

	UE_LOG(LogTemp, Log, TEXT("Critical Attack Damaged"));
}

//被パリィ時
void AFighterCharacter::Parried()
{
	//武器の攻撃状態を非有効にする
	m_pWeapon->SetCollisionActive(false);
}

//被ノックバック時
void AFighterCharacter::KnockBacked(FVector _direction, float _Time) {
	KnockBackStartPos = GetActorLocation();
	KnockBackEndPos = KnockBackStartPos + _direction;
	KnockBackDuration = _Time;
	KnockBackElapsed = 0.0f;
	bIsKnockBack = true;
}

//攻撃ヒット時のイベント関数
void AFighterCharacter::OnAttackHitEvent()
{

}

//----------------------------------------------------
//衝突判定関係
//----------------------------------------------------

//武器の重なった時のイベント
void AFighterCharacter::OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult)
{
	//自身に当たっていたら無視
	if (_otherActor == this)
	{
		return;
	}

	//アクターの入れ物
	//★現在ヒット確認のため
	AFighterCharacter* fighter = nullptr;
	//キャスト
	fighter = Cast<AFighterCharacter>(_otherActor);

	//当たったアクターがファイターなら
	if (fighter)
	{
		//既に攻撃されているアクターなら無視
		if (m_attackedActors.Contains(fighter))
		{
			return;
		}
		//敵同士なら無視
		if (Tags.Contains("Enemy") && fighter->Tags.Contains("Enemy"))
		{
			return;
		}

		//UE_LOG(LogTemp, Warning, TEXT("%s : Attack Is Hit"), *this->GetName());

		//被攻撃側の状態受け取り用
		EAttackReaction attackReaction = EAttackReaction::None;
		//攻撃側(このオブジェクト)の状態通知用
		EAttackPattarnState attackState = GetAttackPattarnState();
		
		//ガードに対する破壊力
		int breakPower = GetGuardBreakPower();

		//打ち上げ攻撃か
		bool canKnockUp = GetAttackAsset().CanKnockUp;

		//攻撃済みアクターに追加
		m_attackedActors.Add(fighter);

		//攻撃力を取得
		int attackPoint = (int)((float)GetAttackAsset().AttackPoint * m_AttackBuffRate);

		//被攻撃側のイベント関数を呼び出し、状態を受け取る
		attackReaction = fighter->OnHitAttack(attackPoint, breakPower, attackState, Cast<AFighterCharacter>(this), canKnockUp);
		//UE_LOG(LogTemp, Log, TEXT("%s : Attack Point Is : %d"), *this->GetName(), attackPoint);
		//UE_LOG(LogTemp, Warning, TEXT("%s : %s Reaction is %s"), *this->GetName(), *fighter->GetName(), *UEnum::GetValueAsString(attackReaction));

		OnAttackHitEvent();

		//パリィされていたら
		if (attackReaction == EAttackReaction::Parry)
		{
			//パリィ時イベント関数
			Parried();
		}
	}
}

//特定方向を向く関数★菅澤加筆
void AFighterCharacter::FaceTargetLocation(const FVector _targetLocation) {
	FVector Dir = _targetLocation - GetActorLocation();
	Dir.Z = 0.0f;

	FRotator Rot = Dir.Rotation();

	FRotator NextRot = GetActorRotation();
	NextRot.Yaw = Rot.Yaw;

	SetActorRotation(NextRot);
}