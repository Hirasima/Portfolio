// Fill out your copyright notice in the Description page of Project Settings.


#include "Girl_Boss.h"
#include "TimerManager.h"
#include "AIController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AGirl_Boss::BeginPlay() {
	Super::BeginPlay();
	
	m_CanKnockUp = false;
	//致命不能にする
	m_bEnableCritical = false;
	StageLv = 0;
	//1本目の武器を回収
	if (!m_WeaponsInfo[0].Weapon) {
		return;
	}
	m_WeaponsInfo[0].Weapon->Initialize(this, m_WeaponsInfo[0].WaitHandle.HandHandleName, m_WeaponsInfo[0].WaitHandle.WaitTransform);

	Timer = 0.0f;
	m_firstPlayerPos = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	//☆Enemy_Baseに移植予定カードリストの初期化////////////////////////////////////////////////////////////////////////////////////
	//手札はから
	Now_Hand.Empty();

	//カードリスト（UPROPERTY）予定
	FIntPoint ThrowDuoKnife = FIntPoint(3, 1);
	FIntPoint SingleBlade = FIntPoint(3, 1);
	FIntPoint DuoBlade = FIntPoint(2, 3);
	FIntPoint BlinkDuoBlade = FIntPoint(2, 5);
	CardsData.Add(ThrowDuoKnife);
	CardsData.Add(SingleBlade);
	CardsData.Add(DuoBlade);
	CardsData.Add(BlinkDuoBlade);
	//デッキ初期化
	//カードのデータ数ぶん回す
	for (int i = 0;i < CardsData.Num();i++) {
		//デッキに枚数分追加
		for (int j = 0;j < CardsData[i].X;j++) {
			CardsDeck.Add(i);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

EGirlBossAttackState AGirl_Boss::GetGirlBossAnimState() {
	return m_AttackState;
}

int AGirl_Boss::GetStageLv(){
	return StageLv;
}

//武器の重なった時のイベント
void AGirl_Boss::OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
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

		//攻撃済みアクターに追加
		m_attackedActors.Add(fighter);

		//攻撃力を取得
		int attackPoint = m_attackPoint;

		//被攻撃側のイベント関数を呼び出し、状態を受け取る
		attackReaction = fighter->OnHitAttack(attackPoint, 55, attackState, Cast<AFighterCharacter>(this));
		UE_LOG(LogTemp, Log, TEXT("%s : Attack Point Is : %d"), *this->GetName(), attackPoint);
		//UE_LOG(LogTemp, Warning, TEXT("%s : %s Reaction is %s"), *this->GetName(), *fighter->GetName(), *UEnum::GetValueAsString(attackReaction));

	}
}

void AGirl_Boss::ReturnLHandWeaponToWait() {
	if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
		return;
	}
	m_WeaponsInfo[0].Weapon->Initialize(this, m_WeaponsInfo[0].WaitHandle.HandHandleName, m_WeaponsInfo[0].WaitHandle.WaitTransform);
	m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(1.0f);
	m_WeaponsInfo[0].Weapon->SetIsWait(true);
}
void AGirl_Boss::ReturnRHandWeaponToWait() {
	if (m_WeaponsInfo[1].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
		return;
	}
	m_WeaponsInfo[1].Weapon->Initialize(this, m_WeaponsInfo[1].WaitHandle.HandHandleName, m_WeaponsInfo[1].WaitHandle.WaitTransform);
	m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(1.0f);
	m_WeaponsInfo[1].Weapon->SetIsWait(true);
}


void AGirl_Boss::Call_L_Weapon() {
	SpawnCausionEffect(m_WeaponsInfo[0].Weapon->GetActorLocation());
	FVector target = m_WeaponsInfo[0].HandHandle.WaitTransform.GetLocation();
	target.Z -= FVector::Dist(GetMesh()->GetSocketLocation(m_WeaponsInfo[0].HandHandle.HandHandleName), m_PlayerPos);
	FTransform T = m_WeaponsInfo[0].HandHandle.WaitTransform;
	T.SetLocation(target);
	m_WeaponsInfo[0].Weapon->Initialize(this, m_WeaponsInfo[0].HandHandle.HandHandleName, m_WeaponsInfo[0].HandHandle.WaitTransform);
	m_WeaponsInfo[0].Weapon->HandWarpQuickly();
	m_WeaponsInfo[0].Weapon->SetIsWait(false);

}

void AGirl_Boss::Call_R_Weapon() {
	SpawnCausionEffect(m_WeaponsInfo[1].Weapon->GetActorLocation());
	FVector target = m_WeaponsInfo[1].HandHandle.WaitTransform.GetLocation();
	target.Z -= FVector::Dist(GetMesh()->GetSocketLocation(m_WeaponsInfo[1].HandHandle.HandHandleName), m_PlayerPos);
	FTransform T = m_WeaponsInfo[1].HandHandle.WaitTransform;
	T.SetLocation(target);

	m_WeaponsInfo[1].Weapon->Initialize(this, m_WeaponsInfo[1].HandHandle.HandHandleName, m_WeaponsInfo[1].HandHandle.WaitTransform);
	m_WeaponsInfo[1].Weapon->HandWarpQuickly();
	m_WeaponsInfo[1].Weapon->SetIsWait(false);
}

void AGirl_Boss::L_WeaponActive() {
	m_WeaponsInfo[0].Weapon->SetAttacking(true);
}

void AGirl_Boss::R_WeaponActive() {
	m_WeaponsInfo[1].Weapon->SetAttacking(true);
}

void AGirl_Boss::EndAnim() {
	m_MotionState = EEnemyMotionState::Wait;
	NowMotion = false;
	m_AttackState = EGirlBossAttackState::Wait;
}
void AGirl_Boss::EndAttackMotion() {
	m_WeaponsInfo[0].Weapon->SetAttacking(false);
	m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::None);
	m_attackedActors.Empty();
}
void AGirl_Boss::EndAttackMotion2() {
	m_WeaponsInfo[1].Weapon->SetAttacking(false);
	m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::None);
	m_attackedActors.Empty();
}

//Tick
void AGirl_Boss::EnemyTick(float _DeltaTime, FVector _PlayerPos) {
	Super::EnemyTick(_DeltaTime, _PlayerPos);
	Timer += _DeltaTime;
	if (m_MotionState == EEnemyMotionState::Stag) {
		m_MotionState = EEnemyMotionState::Wait;
	}
	//HPが到達してたらステップアップ
	if (StageLv == 0) {
		if (m_hitPoint <= m_Max_HitPoint * 0.9f) {
			StepUp0_1();
		}
	}

	if (m_hitPoint <= m_Max_HitPoint * 0.2f) {
		StepUp1_2();
	}
	


	//時間時攻撃処理
	if (StageLv == 1) {
		//攻撃カウントを減少
		At -= _DeltaTime;
		//止まれ
		if (float PlayerDist = FVector::Dist(m_PlayerPos, GetActorLocation()) <= 200.0f) {
			AAIController* AICon = Cast<AAIController>(GetController());
			if (AICon) {
				AICon->StopMovement();
			}
		}
		else {
			if (m_AttackState != EGirlBossAttackState::Montage) {
				GetCharacterMovement()->MaxWalkSpeed = 300.0f * FMath::Sin(Timer);
			}
		}
		//移動中に攻撃カウントがたまったら攻撃
		if (At <= 0.0f&&m_MotionState==EEnemyMotionState::Dash&&!NowMotion) {
			//手札切れていたら使用カード数をリセットしてタイマーもリセットする
			if (!Now_Hand.IsValidIndex(NowUsedCardsNum)&&NowUsedCardsNum!=0) {
				NowUsedCardsNum = 0;
				At = Max_At;
				UE_LOG(LogTemp, Warning, TEXT("Costs Hand End"));
				return;
			}

			//ターン開始時に手札を補充
			if (NowUsedCardsNum == 0) {
				DrewHand();
			}
			if (!Now_Hand.IsValidIndex(NowUsedCardsNum)) {
				UE_LOG(LogTemp, Warning, TEXT("Irregular Card Selected to Use"));
				return;
			}

			//カードを選出
			int32 RandomValue = Now_Hand[NowUsedCardsNum];
			//使用カード数を加算
			++NowUsedCardsNum;
			//攻撃モーション1つが終わるまで行動をロック
			NowMotion = true;

			
			if (RandomValue == 0) {
				if (FVector::Dist(m_PlayerPos, GetActorLocation()) >= 600.0f) {
					//攻撃パターンA：ビットを雑2つ放つ//////////////////////////////////////////////////////////
					//1本目
					m_WeaponsInfo[0].Weapon->SetDamage(20);
					m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
					FVector Target = m_PlayerPos;
					Target.Z = 0.0f;
					m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
					m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
					SpawnCausionEffect(m_WeaponsInfo[0].Weapon->GetActorLocation());
					//2本目
					FTimerHandle DuoAttackHandle;
					GetWorld()->GetTimerManager().SetTimer(DuoAttackHandle, [this]()
						{
							m_WeaponsInfo[1].Weapon->SetDamage(20);
							m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
							FVector Target = m_PlayerPos;
							Target.Z = 0.0f;
							m_WeaponsInfo[1].Weapon->StartAttack(Target, 1.0f, 0.5f);
							m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(5.0f);
							SpawnCausionEffect(m_WeaponsInfo[1].Weapon->GetActorLocation());
						}, 0.7f, false);

					//行動権回復タイマーを設定
					FTimerHandle DuoAttackEndHandle;
					GetWorld()->GetTimerManager().SetTimer(DuoAttackEndHandle, [this]()
						{
							//行動権を回復
							NowMotion = false;
						}, 2.5f, false);

					/////////////////////////////////////////////////////////////////////////////////
				}
				else {

					//距離が近いなら二連切り////////////////////////
					m_MotionState = EEnemyMotionState::Attack;
					m_AttackState = EGirlBossAttackState::Montage;
					m_attackPoint = 35;
					PlayAnimMantage(m_ProtDuoSlashMontage);
					/////////////////////////////////////////
				}
			}
			//単発切り////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 1) {
				m_MotionState = EEnemyMotionState::Attack;
				m_AttackState = EGirlBossAttackState::Montage;
				m_attackPoint = 35;
				PlayAnimMantage(m_ProtSlashMontage);
			}
			/////////////////////////////////////////////////////////////////////////////////////////////
			
			//2連切り////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 2) {
				m_MotionState = EEnemyMotionState::Attack;
				m_AttackState = EGirlBossAttackState::Montage;
				m_attackPoint = 35;
				PlayAnimMantage(m_ProtDuoSlashMontage);
			}
			/////////////////////////////////////////////////////////////////////////////////////////////

			//ブリンク＋2連投擲//////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 3) {
				if (FVector::Dist(m_firstPos.GetLocation(), GetActorLocation()) > FVector::Dist(m_firstPlayerPos, GetActorLocation())) {
					//初期座標にワープ
					TeleportTo(m_firstPos.GetLocation(), GetActorRotation());
				}
				else {
					TeleportTo(m_firstPlayerPos, GetActorRotation());
				}
				//行動権回復タイマーを設定
				FTimerHandle AHandle;
				GetWorld()->GetTimerManager().SetTimer(AHandle, [this]()
					{
						SpawnCausionEffect(m_WeaponsInfo[0].Weapon->GetActorLocation());
						//1本目
						m_WeaponsInfo[0].Weapon->SetDamage(20);
						m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
						FVector Target = m_PlayerPos;
						Target.Z = 0.0f;
						m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
						m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
						//2本目
						FTimerHandle DuoAttackHandle;
						GetWorld()->GetTimerManager().SetTimer(DuoAttackHandle, [this]()
							{
								SpawnCausionEffect(m_WeaponsInfo[1].Weapon->GetActorLocation());
								m_WeaponsInfo[1].Weapon->SetDamage(20);
								m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
								FVector Target = m_PlayerPos;
								Target.Z = 0.0f;
								m_WeaponsInfo[1].Weapon->StartAttack(Target, 1.0f, 0.5f);
								m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(5.0f);
							}, 0.7f, false);

						//行動権回復タイマーを設定
						FTimerHandle DuoAttackEndHandle;
						GetWorld()->GetTimerManager().SetTimer(DuoAttackEndHandle, [this]()
							{
								//行動権を回復
								NowMotion = false;
							}, 2.5f, false);

					}, 1.0f, false);
				
				//手札に追撃を追加
				Now_Hand.Add(4);
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//眼前2連切り////////////////////////////////////////////////////////////////////////////////////
			else if (RandomValue == 4) {
				//プレイヤーの前にワープ
				FVector targetDire = (GetActorLocation() - m_PlayerPos).GetSafeNormal();
				TeleportTo(m_PlayerPos + targetDire * 200.0f,GetActorRotation());

				m_MotionState = EEnemyMotionState::Attack;
				m_AttackState = EGirlBossAttackState::Montage;
				m_attackPoint = 35;
				PlayAnimMantage(m_ProtDuoSlashMontage);
			}
			/////////////////////////////////////////////////////////////////////////////////////////////
		}
	}

}

//攻撃
void AGirl_Boss::Attack() {

	if (StageLv == 0) {
		int32 RandomValue = FMath::RandRange(0, StageLv + 1);

		if (FVector::Dist(m_PlayerPos, GetActorLocation()) > 500.f) {
			RandomValue = 0;
		}
		else {
			RandomValue = 1;
		}
		if (RandomValue == 0) {
			//攻撃パターンA：ビットを雑に放つ//////////////////////////////////////////////////////////
			if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			m_WeaponsInfo[0].Weapon->SetDamage(20);
			m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
			FVector Target = m_PlayerPos;
			Target.Z = 0.0f;
			m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
			m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
			//タイマーリセット
			Super::Attack();
			m_MotionState = EEnemyMotionState::Wait;
			/////////////////////////////////////////////////////////////////////////////////
		}
		else if (RandomValue == 2) {
			if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			if (m_WeaponsInfo[1].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			//1本目
			m_WeaponsInfo[0].Weapon->SetAttacking(true);
			m_WeaponsInfo[0].Weapon->SetDamage(20);
			m_WeaponsInfo[0].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
			FVector Target = m_PlayerPos;
			Target.Z = 0.0f;
			m_WeaponsInfo[0].Weapon->StartAttack(Target, 1.0f, 0.5f);
			m_WeaponsInfo[0].Weapon->SetMoveSpeedAccelerator(5.0f);
			//2本目
			FTimerHandle DuoAttackHandle;
			GetWorld()->GetTimerManager().SetTimer(DuoAttackHandle, [this]()
				{
					m_WeaponsInfo[1].Weapon->SetAttacking(true);
					m_WeaponsInfo[1].Weapon->SetDamage(20);
					m_WeaponsInfo[1].Weapon->SetAttackType(EAttackPattarnState::UpAttack);
					FVector Target = m_PlayerPos;
					Target.Z = 0.0f;
					m_WeaponsInfo[1].Weapon->StartAttack(Target, 1.0f, 0.5f);
					m_WeaponsInfo[1].Weapon->SetMoveSpeedAccelerator(5.0f);
					Super::Attack();
					m_MotionState = EEnemyMotionState::Wait;
				}, 0.7f, false);
		}
		else {
			//攻撃パターンB:手のソケットにつけ振るう///////////////////////////////////////
			if (m_WeaponsInfo[0].Weapon->GetBitMoveState() != EBitWeaponState::Wait) {
				return;
			}
			m_MotionState = EEnemyMotionState::Attack;
			m_AttackState = EGirlBossAttackState::Montage;
			m_attackPoint = 35;
			PlayAnimMantage(m_ProtSlashMontage);
			//タイマーリセット
			Super::Attack();
			//////////////////////////////////////////////////////////////////////
		}
	}
	
}

EAttackPattarnState AGirl_Boss::GetAttackPattarnState() {
	return EAttackPattarnState::DownAttack;
}

void AGirl_Boss::ForgetDamagedActor() {
	ClearAttackedActors();
}

void AGirl_Boss::StepUp0_1() {
	if (StageLv != 0) {
		return;
	}
	//2本目の武器を回収
	if (!m_WeaponsInfo[1].Weapon) {
		return;
	}
	m_WeaponsInfo[1].Weapon->Initialize(this, m_WeaponsInfo[1].WaitHandle.HandHandleName, m_WeaponsInfo[1].WaitHandle.WaitTransform);

	//歩くように変更
	m_AttackRange = 1.0f;;

	StageLv = 1;
}

void AGirl_Boss::StepUp1_2() {
	if (StageLv != 1) {
		return;
	}
	//2本目の武器を回収
	if (!m_WeaponsInfo[2].Weapon) {
		return;
	}
	if (!m_WeaponsInfo[3].Weapon) {
		return;
	}
	if (!m_WeaponsInfo[4].Weapon) {
		return;
	}
	if (!m_WeaponsInfo[5].Weapon) {
		return;
	}
	m_WeaponsInfo[2].Weapon->Initialize(this, m_WeaponsInfo[2].WaitHandle.HandHandleName, m_WeaponsInfo[2].WaitHandle.WaitTransform);
	m_WeaponsInfo[3].Weapon->Initialize(this, m_WeaponsInfo[3].WaitHandle.HandHandleName, m_WeaponsInfo[3].WaitHandle.WaitTransform);
	m_WeaponsInfo[4].Weapon->Initialize(this, m_WeaponsInfo[4].WaitHandle.HandHandleName, m_WeaponsInfo[4].WaitHandle.WaitTransform);
	m_WeaponsInfo[5].Weapon->Initialize(this, m_WeaponsInfo[5].WaitHandle.HandHandleName, m_WeaponsInfo[5].WaitHandle.WaitTransform);

	//歩くように変更
	m_AttackRange = 1.0f;

	StageLv = 2;
}

//☆EnemyBaseに移植したいやつ
//手札選出
void AGirl_Boss::DrewHand() {
	//☆例えば

	//ステージごとに分ける
	if (StageLv == 0) {
		//こいつ１パターンしかないから
		return;
	}
	if (StageLv == 1) {
		//手札をリセット
		Now_Hand.Empty();
		//手札コスト数
		int Costs=0;
		//手札枚数
		int CardsNum=0;
		//手札を選出
		do {
			//最大が山札枚数の乱数発生
			int32 RandomValue = FMath::RandRange(0, CardsDeck.Num());
			if (CardsDeck.IsValidIndex(RandomValue)) {
				//手札に登録
				Now_Hand.Add(CardsDeck[RandomValue]);
				//手札枚数と現在コストを更新
				Costs += CardsData[CardsDeck[RandomValue]].Y;
				++CardsNum;
				//デバック用
				UE_LOG(LogTemp, Warning, TEXT("Costs_Size:%d"), Costs);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Irregular Cards Select To Hands"));
			}
		} while (Costs<=MaxHandsCosts&&CardsNum<HandsMax);

		//☆これの呼び出しよろしく☆
		
	}
}


void AGirl_Boss::SpawnCausionEffect(FVector _pos) {
	if (!AttackScenceEffect) {
		return;
	}
	UNiagaraComponent* niagaraComp;

	niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackScenceEffect, _pos, GetActorRotation());
	niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor::Red * 50);
}


