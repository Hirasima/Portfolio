// Fill out your copyright notice in the Description page of Project Settings.


#include "GirlsBitWeapon.h"
#include "Enemy_Base.h"
#include "Girl_Boss.h"
#include "TimerManager.h"

void AGirlsBitWeapon::Tick(float DeltaTime) {
	//親
	Super::Tick(DeltaTime);
	UpdateMoving(DeltaTime);
}

void AGirlsBitWeapon::Initialize(class AEnemy_Base* _user, FName _socketName, FTransform _waitTransform) {
	SetTargetSocket(_user, _socketName);
	SetWaitTransform(_waitTransform);
	m_State = EBitWeaponState::Return;
	m_IsAutoFly = true;
	m_AttackedDulationTimer = 0.0f;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AGirlsBitWeapon::BeginPlay() {
	Super::BeginPlay();
	m_SinTimer = 0.0f;
	m_IsWait = true;
	GetPrimitiveComponent()->SetSimulatePhysics(false);
	//攻撃ヒット時関数を設定
	GetPrimitiveComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGirlsBitWeapon::OnBitWeaponOverlap);
}

void AGirlsBitWeapon::SetTargetSocket(AEnemy_Base* _user, FName _socketName) {
	m_pUser = _user;
	HandleSocketName = _socketName;
}
void AGirlsBitWeapon::SetWaitTransform(FTransform _waitTransform) {
	m_TargetWaitPos = _waitTransform;
}

void AGirlsBitWeapon::SetBitMoveState(EBitWeaponState _state) {
	m_State = _state;
}

EBitWeaponState AGirlsBitWeapon::GetBitMoveState() {
	return m_State;
}

void AGirlsBitWeapon::SetMoveSpeedAccelerator(float _MoveSpeed) {
	m_nowSpeed = MoveSpeed * _MoveSpeed;
}

void AGirlsBitWeapon::SetDamage(int _damage) {
	m_Damage = _damage;
}
void AGirlsBitWeapon::SetAttackType(EAttackPattarnState _type) {
	m_AttackState = _type;
}

void AGirlsBitWeapon::StartAttack(FVector _targetPos, float AttackDulation, float Dulation) {
	if (m_State != EBitWeaponState::Wait) {
		return;
	}
	m_TargetAttackPos = _targetPos;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FRotator Target = (_targetPos - GetActorLocation()).Rotation();
	Target.Yaw -= 90;
	SetActorRotation(Target);
	m_AttackedDulation = AttackDulation;
	m_AttackedDulationTimer = 0.0f;
	m_IsAutoFly = true;
	FTimerHandle AtHandle;
	GetWorld()->GetTimerManager().SetTimer(AtHandle, [this,_targetPos,AttackDulation,Dulation]()
		{
			m_State = EBitWeaponState::Go;
			SetAttacking(true);
			m_attackedActors.Empty();
		}, Dulation, false);
}

void AGirlsBitWeapon::SetIsWait(bool _flag) {
	m_IsWait = _flag;
}

//強制的に手元に戻る関数
void AGirlsBitWeapon::HandWarpQuickly() {
	m_IsAutoFly = false;
	m_IsWait = false;
	m_State = EBitWeaponState::Wait;
	AttachToComponent(m_pUser->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandleSocketName);

	SetActorRelativeTransform(m_TargetWaitPos);
}

void AGirlsBitWeapon::UpdateMoving(float DeltaTime) {
	//ユーザーがいないならふわふわしない
	if (!m_pUser) {
		return;
	}
	//ソケットに管理されているなら流す
	if (m_IsAutoFly == false) {
		if (m_IsWait==true) {
			//上下に揺らす
			m_SinTimer += DeltaTime;
			FVector Pos = GetActorLocation();
			Pos.Z += 0.7f * FMath::Sin(m_SinTimer * 2);
			SetActorLocation(Pos);
			return;
		}
	}
	//攻撃中じゃないなら待機座標に帰ろうとする
	if (m_State == EBitWeaponState::Return) {
		//距離が一定以内なら待機ソケットにくっつく
		FVector Soc = m_pUser->GetMesh()->GetSocketLocation(HandleSocketName);
		if (FVector::Dist(GetActorLocation(), Soc )<200.0f) {
			AttachToComponent(m_pUser->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandleSocketName);
			SetActorRelativeTransform(m_TargetWaitPos);
			m_State = EBitWeaponState::Wait;
			m_IsAutoFly = false;
			//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Ret_Fin"));
			return;
		}

		//待機場所に戻る処理
		FVector CurrentPos = GetActorLocation();
		FVector NextPos = FMath::VInterpTo(CurrentPos, Soc, DeltaTime, MoveSpeed);
		FRotator NextRot = FMath::RInterpTo(GetActorRotation(), m_TargetWaitPos.GetRotation().Rotator(), DeltaTime, MoveSpeed);
		SetActorLocation(NextPos);
		SetActorRotation(NextRot);
		//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Ret"));
		return;
	}

	//攻撃中なら攻撃目標座標に飛んでいく
	if (m_State == EBitWeaponState::Go) {
		//攻撃対象座標なら
		if (FVector::Dist(GetActorLocation(), m_TargetAttackPos) < 10.0f) {
			m_AttackedDulationTimer += DeltaTime;
			//☆仮置き忘却
			m_attackedActors.Empty();
			m_AttackState = EAttackPattarnState::None;
			if (m_AttackedDulationTimer >= m_AttackedDulation) {
				//余韻後に戻る
				m_State = EBitWeaponState::Return;
				SetAttacking(false);
			}
			//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Go_Fin"));
			return;
		}

		//攻撃対象に向かう処理
		FVector CurrentPos = GetActorLocation();
		FVector NextPos = FMath::VInterpTo(CurrentPos, m_TargetAttackPos, DeltaTime, m_nowSpeed);
		SetActorLocation(NextPos);
		//UE_LOG(LogTemp, Warning, TEXT("WeaponMoving_Go"));
		return;
	}
}


//武器の重なった時のイベント
void AGirlsBitWeapon::OnBitWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult)
{
	//自身に当たっていたら無視
	if (_otherActor == this||_otherActor==m_pUser)
	{
		return;
	}

	if (m_IsAutoFly == false) {
		m_pUser->OnWeaponOverlap(_overlappedComponent, _otherActor, _otherComp, _otherBodyIndex, _bFromSweep, _sweepResult);
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

		//攻撃済みアクターに追加
		m_attackedActors.Add(fighter);


		//被攻撃側のイベント関数を呼び出し、状態を受け取る
		attackReaction = fighter->OnHitAttack(m_Damage, 55, m_AttackState, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("State: % s"), *UEnum::GetValueAsString(m_AttackState));

		//UE_LOG(LogTemp, Warning, TEXT("%s : %s Reaction is %s"), *this->GetName(), *fighter->GetName(), *UEnum::GetValueAsString(attackReaction));

		//パリィされていたら
		/*if (attackReaction == EAttackReaction::Parry)
		{
			パリィ時イベント関数
			Parried();
		}*/
	}
}
