// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Base.h"
#include "Components/CapsuleComponent.h"
#include "EnemyHpBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Weapon.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include"Engine/StaticMeshActor.h"
#include "GameFramework/Controller.h"
#include"NavigationSystem.h"
#include "AIController.h"
#include "TimerManager.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "PlayPawn.h"
#include "NiagaraComponent.h"

// Sets default values
AEnemy_Base::AEnemy_Base()
	:m_Situation(EEnemySituationState::Patrol), m_MotionState(EEnemyMotionState::Wait), m_isAlartPoin(false), m_StagState(EStagType::None)
{	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Enemyタグを追加
	Tags.Add("Enemy");
}

// Called when the game starts or when spawned
void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	//致命封印
	m_bEnableCritical = false;

	//初期位置を保存
	if (m_IsAnotherFirstPos==false) {
		m_firstPos = GetTransform();
	}

	//ステータスの初期化
	Mp = Max_Mp;
	At = 0;
	//ガード時間を初期化
	m_guardTime = 0.0f;

	//面をかぶる
	if (m_pFaceMesh) {
		// StaticMeshComponentを生成
		UStaticMeshComponent* FaceMeshComp = NewObject<UStaticMeshComponent>(this);

		// メッシュを設定
		FaceMeshComp->SetStaticMesh(m_pFaceMesh);

		// MobilityをMovableに（アタッチやTransform変更のため）
		FaceMeshComp->SetMobility(EComponentMobility::Movable);

		// 所有アクターに登録（登録しないと表示されない）
		FaceMeshComp->RegisterComponent();

		// ソケットにアタッチ（例: HeadSetSocket）
		FaceMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("HeadSetSocket"));
	}

}

//エネミーの状況を設定する関数
void AEnemy_Base::SetEnemySituation(EEnemySituationState _situ) {
	m_Situation = _situ;
}

void AEnemy_Base::BeAlarm(FVector _alartPos) {
	m_IsAlart = true;
	m_isAlartPoin = false;
	//警告位置を伝達
	m_alartPos = _alartPos;
}
void AEnemy_Base::EndAlarm() {
	m_IsAlart = false;
}

//エネミーの予知している攻撃を設定する関数
void AEnemy_Base::SetScenceOfPlayerAttack(EAttackPattarnState _pattarn) {
	m_NowScenceOfPlayerAttack = _pattarn;
	UE_LOG(LogTemp, Warning, TEXT("GETTT"));
}

//エネミーの更新関数*エネミーマネージャのみ呼び出す*
void AEnemy_Base::EnemyTick(float DeltaTime,FVector PlayerPos)
{
	Super::Tick(DeltaTime);

	if (m_bIsKeepZAxis==true&&bKnockBack==true) {
		SetActorLocation(KeepPos);
		return;
	}
	//死んでたらorノックバック中は何もしない
	if (m_Situation == EEnemySituationState::Dead || bKnockBack == true) {
		return;
	}

	//★仮置き死亡処理
	//HPが0なら死状態になる
	if (m_hitPoint <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
		//状況登録
		m_Situation = EEnemySituationState::Dead;
		//アニメーション用登録★きも
		m_MotionState = EEnemyMotionState::Dead;
		PlayAnimMantage(m_pDeadMontage);
		return;
	}


	//致命中も何もしない
	if (m_MotionState == EEnemyMotionState::Assacinated) {
		return;
	}

	//プレイヤーの座標を保存
	m_PlayerPos = PlayerPos;

	//ガン見
	if (m_LookPlayer) {
		LookforPlayer();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//現在の状況に合わせてTick処理を分ける
	switch (m_Situation)
	{
	case EEnemySituationState::Battle:
		InBattleMove();
		//攻撃タイマーを進める
		if (At > 0) {
			At -= DeltaTime;
		}
		//フロー利用者のためのフロー更新処理
		UpdateFlow(DeltaTime);
		break;
	case EEnemySituationState::Patrol:
		Patrol();
		break;
	default:
		break;
	}
	/////////////////////////////////////////////////////////////////////////////////////////


	// アニメーション状態別処理★関数化しようねぇ////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//はじかれ硬直状態の終了確認と時間加算//////////////////////////////////////////////////////////////////////
	if (m_bParried)
	{
		//弾かれ時間に加算
		m_parriedTime += DeltaTime;

		m_MotionState = EEnemyMotionState::Parried;
		PlayAnimMantage(m_pUpParriedMontage);

		//弾かれ終了確認
		//フロー中は短め
		if (m_IsFlow) {
			if (m_parriedTime >= m_parriedMaxTime * 0.5f)
			{
				//アニメーションを待機に
				m_MotionState = EEnemyMotionState::Wait;

				//武器の軌道を無効化
				m_pWeapon->SetBladeOrbitActive(false);

				//弾かれ時間を初期化
				m_parriedTime = 0.0f;
				m_bParried = false;

				UE_LOG(LogTemp, Warning, TEXT("Enemy : Parried Finish"));
			}
		}
		//非フロー時
		else {
			if (m_parriedTime >= m_parriedMaxTime)
			{
				//アニメーションを待機に
				m_MotionState = EEnemyMotionState::Wait;

				//武器の軌道を無効化
				m_pWeapon->SetBladeOrbitActive(false);

				//弾かれ時間を初期化
				m_parriedTime = 0.0f;
				m_bParried = false;

				UE_LOG(LogTemp, Warning, TEXT("Enemy : Parried Finish"));
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//待機座標を設定する関数
void AEnemy_Base::SetWaitPosition(FTransform _targetPos) {
	m_firstPos = _targetPos;
}

//巡回中（非戦闘時）の挙動
void AEnemy_Base::Patrol() {
	//視認範囲にプレイヤーがいるかチェック
	if (PlayerFindCheck()) {
		if (RegisterToBattleMngCallBack.IsBound()) {
			RegisterToBattleMngCallBack.Execute(this);
			m_IsAlart = false;
			return;
		}
	}

	//暗闇中は何もできない
	if (m_IsBlind) {
		Cast<AAIController>(GetController())->StopMovement();
		return;
	}

	//警戒状態なら目的地を目指す
	if (m_IsAlart) {
		//警告位置に着いたことがないなら
		if (!m_isAlartPoin) {
			//警告位置との距離が一定以内なら警戒を解除
			if (float Dist = FVector2D::Distance(FVector2D(m_alartPos.X,m_alartPos.Y), FVector2D(GetActorLocation().X,GetActorLocation().Y)) < 200.0f) {
				// 指定時間後に元に戻すタイマーをセット
				GetWorld()->GetTimerManager().SetTimer(
					AlartEndTimerHandle,
					this,
					&AEnemy_Base::EndAlarm,
					5.0f, // ← 実時間で何秒にするか
					false
				);
				m_isAlartPoin = true;
				m_MotionState = EEnemyMotionState::Wait;
				//止まれ
				AAIController* AICon = Cast<AAIController>(GetController());
				if (AICon) {
					AICon->StopMovement();
				}
				//周り見るべき
				SetActorRotation(GetActorRotation() * -1);
				return;
			}
		}
		else {
			//★きょろきょろしたみはある
			m_MotionState = EEnemyMotionState::Wait;
			return;
		}

		//初期位置を見る
		FVector Direction = (m_alartPos - GetActorLocation()).GetSafeNormal();
		FRotator Rotation = Direction.Rotation();
		Rotation.Pitch = 0;
		SetActorRotation(Rotation);
		//初期位置に移動
		AAIController* AICon = Cast<AAIController>(GetController());
		if (AICon) {
			AICon->MoveToLocation(m_alartPos);
		}
		//可能なら移動アニメーションにしたい感はある
		m_MotionState = EEnemyMotionState::Dash;
		return;
	}

	//警戒中じゃないなら元の場所に戻る
	if (float Dist = FVector2D::Distance(FVector2D(m_firstPos.GetLocation().X, m_firstPos.GetLocation().Y), FVector2D(GetActorLocation().X,GetActorLocation().Y)) < 200.0f) {
		m_MotionState = EEnemyMotionState::Wait;
		FRotator Rotation = m_firstPos.GetRotation().Rotator();
		Rotation.Pitch = 0;
		SetActorRotation(Rotation);
		return;
	}
	//初期位置を見る
	FVector Direction = (m_firstPos.GetLocation() - GetActorLocation()).GetSafeNormal();
	FRotator Rotation = Direction.Rotation();
	Rotation.Pitch = 0;
	SetActorRotation(Rotation);
	//初期位置に移動
	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon) {
		AICon->MoveToLocation(m_firstPos.GetLocation());
	}
	//可能なら移動アニメーションにしたい感はある
	m_MotionState = EEnemyMotionState::Dash;
	return;
}

//視界にプレイヤーがいるかチェック
bool AEnemy_Base::PlayerFindCheck() {

	//距離チェック
	if (FVector::Dist(m_PlayerPos, GetActorLocation())>GetViewLange()) {
		return false;
	}

	//視野角チェック★マジックナンバーで視野角９０になってる
	FVector ToTargetDirection = (m_PlayerPos - GetActorLocation()).GetSafeNormal();
	//ドット積
	float Dot=FVector::DotProduct(GetActorForwardVector(), ToTargetDirection);
	//角度をデグリーに変換
	float TargetDegree = FMath::Acos(Dot) * (180.f / PI);
	if (TargetDegree > 180 / 2.0f) {
		return false;
	}

	//遮蔽チェック
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), m_PlayerPos, ECC_Visibility,Params);
	if (!bHit) {
		return true;
	}
	if (bHit && HitResult.GetActor()) {
		if (HitResult.GetActor()->ActorHasTag(FName("Player"))) {
			return true;
		}
	}

	return false;
}

//死亡アニメーション終了時に呼ぶやつ（消去処理）
void AEnemy_Base::DeleteOut() {
	//死亡時処理
	if (m_Situation == EEnemySituationState::Dead) {
		//エネミーマネージャから解放
		if (UnregisterFromEnemyMngCallBack.IsBound()) {
			UnregisterFromEnemyMngCallBack.Execute(this);
		}
		//バトルマネージャから解放
		if (UnregisterFromBattleMngCallBack.IsBound()) {
			UnregisterFromBattleMngCallBack.Execute(this);
		}
		if (m_pWeapon)
		{
			m_pWeapon->Destroy();
		}
		//削除
		this->Destroy();

		return;
	}
}

//視界範囲を取得する関数
float AEnemy_Base::GetViewLange() {
	float ViewDist = (m_IsBlind) ? m_BlindBattleSearchRange : m_BattleSearchRange;
	return ViewDist;
}

//戦闘中の移動周り
void AEnemy_Base::InBattleMove() {
	//行動中は移動しない
	if (m_MotionState != EEnemyMotionState::Wait && m_MotionState != EEnemyMotionState::Dash) {
		return;
	}

	//範囲外ならバトルを抜ける
	CheckOutBattleManager(m_PlayerPos);
	if (m_Situation != EEnemySituationState::Battle) {
		return;
	}

	//プレイヤーの方を向く
	LookforPlayer();

	//プレイヤーとの距離を測定
	float PlayerDist = FVector::Dist(m_PlayerPos, GetActorLocation());
	//近すぎるなら離れる
	if (PlayerDist < m_NearestRange) {
		//プレイヤーの逆方向に移動
		SetActorLocation(GetActorForwardVector() * -m_walkSpeed *5.0f* UGameplayStatics::GetGlobalTimeDilation(GetWorld()) + GetActorLocation());
		//★バックステップにしたみはある
		m_MotionState = EEnemyMotionState::Dash;
		return;
	}
	//攻撃範囲より遠くにいるなら移動
	if (PlayerDist > m_AttackRange) {
		//プレイヤーの方向に移動
		AAIController* AICon = Cast<AAIController>(GetController());
		if (AICon) {
			AICon->MoveToLocation(m_PlayerPos);
		}
		//可能なら移動アニメーションにしたい感はある
		m_MotionState = EEnemyMotionState::Dash;
		return;
	}
	//ちょうどいい距離なら戦闘
	//waitにしてから
	m_MotionState = EEnemyMotionState::Wait;
	//止まれ
	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon) {
		AICon->StopMovement();
	}
	EnemyBattleAnimTick();

}

//エネミーの戦闘アニメーションステート切り替え
void AEnemy_Base::EnemyBattleAnimTick() {

	//待機、歩き中ならステートを切り替え可
	if (m_MotionState == EEnemyMotionState::Wait || m_MotionState == EEnemyMotionState::Dash) {
		//時間が来てたら攻撃(優先度1)
		Attack();
	}
}

//アニメーションモンタージュ取得関数
UFUNCTION(BlueprintCallable)
UAnimMontage* AEnemy_Base::GetNowTargetMontage() {
	UAnimMontage* monta = nullptr;

	//アニメーション状況によって処理を分別
	switch (m_MotionState)
	{
	case EEnemyMotionState::Wait:
		break;
	case EEnemyMotionState::Dash:
		break;
	case EEnemyMotionState::UpGuard:
		break;
	case EEnemyMotionState::DownGuard:
		break;
	case EEnemyMotionState::Parried:
		break;
	case EEnemyMotionState::Damaged :
		break;
	case EEnemyMotionState::Attack:
		monta = GetAttackMontage();
		break;
	case EEnemyMotionState::Stag:
		monta = GetStagMontage();
		break;
	case EEnemyMotionState::Dead:
		monta = m_pDeadMontage;
		break;
	case EEnemyMotionState::Assacinated:
		monta = m_pAssacinatedMontage;
		break;
	default:
		break;
	}

	return monta;
}

//攻撃時にアニメーションモンタージュを取得する関数
UFUNCTION(BlueprintCallable)
UAnimMontage* AEnemy_Base::GetAttackMontage() {
	return nullptr;
}

//攻撃時にアニメーションモンタージュを取得する関数
UFUNCTION(BlueprintCallable)
UAnimMontage* AEnemy_Base::GetStagMontage() {
	UAnimMontage* Monta = nullptr;

	switch (m_StagState)
	{
	case EStagType::None:
		break;
	case EStagType::SmallStag:
		Monta = m_pMiniStagMontage;
		break;
	case EStagType::BigStag:
		Monta = m_pBigStagMontage;
		break;
	case EStagType::Down:
		break;
	default:
		break;
	}
	return Monta;
}

//現在のモーション状況を取得する関数
UFUNCTION(BlueprintCallable)
EEnemyMotionState AEnemy_Base::GetNowMotionState()const {
	return m_MotionState;
}

//現在のモーション状況を設定する関数
UFUNCTION(BlueprintCallable)
void AEnemy_Base::SetNowMotionState(EEnemyMotionState _state) {
	m_MotionState = _state;
}

//エネミーの戦闘更新関数*バトルマネージャからのみ呼ぶこと*
void AEnemy_Base::EnemyBattleTick(float _DeltaTime, FVector _PlayerPos) {

}

//バトルマネージャから離脱する関数
void AEnemy_Base::CheckOutBattleManager(FVector _PlayerPos) {
	float PlayerDist = FVector::Dist(GetActorLocation(), _PlayerPos);
	//プレイヤーとの距離が視界より遠いなら//////////////////////////////////////////////////////
	
	//失明状況で視界を決定する
	float ViewDist = (m_IsBlind) ? m_BlindBattleSearchRange : m_BattleSearchRange;

	if (PlayerDist >= ViewDist) {
		//待機へ
		m_Situation = EEnemySituationState::Patrol;
		m_MotionState = EEnemyMotionState::Wait;

		//バトルマネージャから解放
		if (UnregisterFromBattleMngCallBack.IsBound()) {
			UnregisterFromBattleMngCallBack.Execute(this);
		}
		return;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//プレイヤーの方を向く関数
void AEnemy_Base::LookforPlayer() {
	FVector Direction = (m_PlayerPos - GetActorLocation()).GetSafeNormal();
	FRotator Rotation = Direction.Rotation();
	Rotation.Pitch = 0;
	SetActorRotation(Rotation);
}

//照明状況の伝達関数
void AEnemy_Base::SetIsBlind(bool _isBlind) {
	m_IsBlind = _isBlind;
}


//HP取得関数
int AEnemy_Base::GetEnemyHp() {
	return m_hitPoint;
}

//攻撃したアクターを空に
void AEnemy_Base::ClearAttackedActors() {
	m_attackedActors.Empty();
}

//攻撃時
void AEnemy_Base::OnAttack()
{
	m_pWeapon->SetAttacking(true);
	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, AttackSE, GetActorLocation(), SE_Volume);
}

//攻撃終了時
void AEnemy_Base::OnFinishAttack()
{
	m_pWeapon->SetAttacking(false);
	
	m_MotionState = EEnemyMotionState::Wait;

	ClearAttackedActors();

	m_LookPlayer = false;
}

//攻撃予兆スポーン
void AEnemy_Base::SpawnAttackScenceEffect()
{
	if (!AttackScenceEffect) {
		return;
	}
	UNiagaraComponent* niagaraComp;

	niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackScenceEffect, m_pWeapon->GetActorLocation(), GetActorRotation());
	niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor::Yellow*50);
}

//攻撃を受けたときに呼ばれる関数
EAttackReaction AEnemy_Base::OnHitAttack(int _damage, int _breakPower, EAttackPattarnState _attackState, AFighterCharacter* _attacker, bool _canKnockUp) {
	//攻撃側の状態が有効でなければ
	if (_attackState == EAttackPattarnState::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy : Attack Pattarn Is None"));
		return EAttackReaction::None;
	}
	if (m_Situation == EEnemySituationState::Dead) {
		return EAttackReaction::None;
	}

	//攻撃リアクションの戻り値用
	EAttackReaction attackReaction = EAttackReaction::None;

	bool isDamage(true);
	bool isParried(false);

	//ガード一致ならダメージは受けない:★リワーク対象/////////////////////////
	//bool CheckGuardAttack()にして/////////////////////////////////////////////////////////////////////////////
	if (m_MotionState == EEnemyMotionState::UpGuard && _attackState == EAttackPattarnState::UpAttack&& _canKnockUp == false)
	{
		isDamage = false;
	}
	else if (m_MotionState == EEnemyMotionState::UpGuard && _attackState == EAttackPattarnState::DownAttack&&_canKnockUp==false)
	{
		isDamage = false;
	}

	if (_canKnockUp == true) {
		isDamage = true;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////

	//ダメージの有無による処理分岐（被弾側の影響処理）////////////////////////////////////////////////////////////////
	//ダメージを受けていたら
	if (isDamage&&m_hitPoint>0)
	{
		//リアクションを当たったに
		attackReaction = EAttackReaction::Hit;
		//キープ解除
		m_bIsKeepZAxis = false;
		//ノックアップ攻撃なら
		if (_canKnockUp == true&& m_CanKnockUp&&m_hitPoint>_damage) {
			bKnockBack = true;
			m_MotionState = EEnemyMotionState::Stag;
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCharacterMovement()->Velocity.X = 0;
			GetCharacterMovement()->Velocity.Y = 0;
			GetCharacterMovement()->SetPlaneConstraintEnabled(false);
			


			if (m_bIsKnockUpping == true) {
				PlayAnimMantage(m_pKnockUpKeepMontage);
				
			}else{
				PlayAnimMantage(m_pKnockUpMontage);
			}
			Super::OnHitAttack(_damage, _breakPower, _attackState, _attacker, _canKnockUp);

		}
		else {

			//奇襲だったら（非戦闘状態なら）とりあえずプレイヤーの方を見る
			if (m_Situation == EEnemySituationState::Patrol) {
				LookforPlayer();
			}
			//★仮置きよろけ処理
			//フロー中or死はよろけない
			if (!m_IsFlow && m_MotionState != EEnemyMotionState::Stag && m_hitPoint > _damage) {
				//一応攻撃処理停止保険
				OnFinishAttack();
				//物理挙動ないない
				GetCharacterMovement()->StopMovementImmediately();
				if (_attackState == EAttackPattarnState::UpAttack) {
					m_MotionState = EEnemyMotionState::Stag;
					m_StagState = EStagType::BigStag;
				}
				else {
					m_MotionState = EEnemyMotionState::Stag;
					m_StagState = EStagType::SmallStag;
				}
				PlayAnimMantage(GetStagMontage());
				//パワーゼロノックバック扱い
				bKnockBack = true;
			}
		}
		Damaged(_damage);

		//SE再生
		UGameplayStatics::PlaySoundAtLocation(this, DamageSE, GetActorLocation(), SE_Volume);
	}
	//ガードできていたら
	else if(m_hitPoint > 0)
	{
		//リアクションを防御に
		attackReaction = EAttackReaction::Guard;
		//フローゲージの減少と伝達
		//フローゲージを減らす
		Mp -= GuardGein_Mp;

		if (Mp <= 0) {
			Mp = 0;
		}

		//カウンターモーションに派生
		if (GuardCounterAttack.AnimMantage&&m_hitPoint>0) {
			m_MotionState = EEnemyMotionState::Wait;
			PlayAnimMantage(GuardCounterAttack.AnimMantage);
			m_MotionState = EEnemyMotionState::Attack;
			m_combo = 99;
		}

		//SE再生
		UGameplayStatics::PlaySoundAtLocation(this, JustGuardSE, GetActorLocation(), SE_Volume);

		UE_LOG(LogTemp, Warning, TEXT("Enemy : Guard!!!!!!!!!"));
		//UE_LOG(LogTemp, Warning, TEXT("EnemyAnim : %s"), *UEnum::GetValueAsString(m_AttackAnimState));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//対応したエフェクトを出す
	SpawnEffect(attackReaction);

	return attackReaction;
}

//エフェクトを出す
void AEnemy_Base::SpawnEffect(EAttackReaction _attackReaction)
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

		niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageEffect, GetCapsuleComponent()->GetComponentLocation(), GetActorRotation());
		niagaraComp->SetVariableLinearColor(TEXT("Color"), FLinearColor::Blue*50.0f);
	}
	//ガード時
	else if (_attackReaction == EAttackReaction::Guard)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GuardEffect, m_pWeapon->GetActorLocation(), GetActorRotation());
	}
	//弾き時
	else if (_attackReaction == EAttackReaction::Parry)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryEffect, m_pWeapon->GetActorLocation(), GetActorRotation());
	}
}

//攻撃実行関数(親でやるのはタイマーリセットのみ)
void AEnemy_Base::Attack() {
	//攻撃状態に
	m_MotionState = EEnemyMotionState::Attack;
	//攻撃待機時間を元に戻す
	At = Max_At;

	return;
}

//被ダメージ時
void AEnemy_Base::Damaged(int _damagePoint)
{
	//ダメージ処理
	//★フロー中か否かで被ダメ変更
	if (m_IsFlow && m_IsUseFlow) {
		//★謎式:フロー中は肉の代わりにフロー値減少
		Mp -= _damagePoint;
		//ついでにフロータイマーを0にする
		FlowTimer = 0.0f;
		//フロー割れか確認
		if (Mp <= 0) {
			BreakingFlowTrriger();
		}
	}
	else {
		//Hpをダメージ分減らす
		m_hitPoint -= _damagePoint;
	}
	//予知動作を無にする
	m_NowScenceOfPlayerAttack = EAttackPattarnState::None;

	//攻撃を受けたら攻撃タイマーを加速させる★上下で分けたい
	At -= UpperAnger;

	UE_LOG(LogTemp, Warning, TEXT("EnemyHp:%d"), m_hitPoint);

}

//被致命攻撃時
void AEnemy_Base::OnCriticalAttacked(int _damage)
{
	UE_LOG(LogTemp, Log, TEXT("Enemy : Critical Attack Damaged"));

	//もしフロー中だったら強制解除
	if (m_IsFlow == true) {
		m_IsFlow = false;
	}
	m_MotionState = EEnemyMotionState::Assacinated;
	PlayAnimMantage(m_pAssacinatedMontage);
}

//はじかれた時の呼ばれる関数
void AEnemy_Base::Parried()
{
	Super::Parried();

	//物理挙動ないない
	GetCharacterMovement()->StopMovementImmediately();
	//はじかれた状態にする
	m_MotionState = EEnemyMotionState::Parried;
	//一応攻撃処理停止保険
	OnFinishAttack();

	//弾かれ時間初期化
	m_parriedTime = 0.0f;
	m_bParried = true;
}

//ノックバックを終了させる関数
void AEnemy_Base::KnockBackEnd() {
	if (auto* Move = GetCharacterMovement()) {
		Move->GroundFriction = m_Def_GroundFriction;
		Move->BrakingFrictionFactor = m_Def_BrakingFrictionFactor;
		Move->BrakingDecelerationWalking = m_Def_BrakingDecelWalking;
		Move->bOrientRotationToMovement = m_Def_bOrientRotationToMovement;
	}
	bKnockBack = false;
	UE_LOG(LogTemp, Warning, TEXT("ENDDD"));
}

void AEnemy_Base::KnockUpEnd() {
	bKnockBack = false;
	m_bIsKnockUpping = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

//ノックバックする関数
void AEnemy_Base::KnockBackStart(FVector _powerPos, float _power) {
	//パリィ中なら終了
	if (m_bParried) {
		m_bParried = false;
	}

	//一応攻撃処理停止保険
	OnFinishAttack();
	//アニメーション再生
	m_MotionState = EEnemyMotionState::Stag;
	PlayAnimMantage(m_pBigStagMontage);

	FVector Dir = GetActorLocation() - _powerPos;
	Dir.Z = 0.0f;

	auto* Move = GetCharacterMovement();
	if (Move) {
		//直前の移動ベクトルを全削除
		Move->StopMovementImmediately();

		//普段の移動処理用設定をいったん保存
		m_Def_GroundFriction = Move->GroundFriction;
		m_Def_BrakingFrictionFactor = Move->BrakingFrictionFactor;
		m_Def_BrakingDecelWalking = Move->BrakingDecelerationWalking;
		m_Def_bOrientRotationToMovement = Move->bOrientRotationToMovement;

		//完成やら摩擦やらを一旦消す
		Move->GroundFriction = 0;
		Move->BrakingFrictionFactor = 0;
		Move->BrakingDecelerationWalking = 0;
		Move->bOrientRotationToMovement = false;
	}

	FVector Push = Dir * _power;
	LaunchCharacter(Push, true, true);

	bKnockBack = true;
}

void AEnemy_Base::BreakingFlowTrriger() {
	if (m_IsUseFlow == false) {
		return;
	}
	//フロー割れたら吹っ飛ぶ
	m_IsFlow = false;
	if (CallOfHitStopCallBack.IsBound()) {
		CallOfHitStopCallBack.Execute();
		UE_LOG(LogTemp, Warning, TEXT("HitStopCallBackCalled!"));
	}

	//SE再生
	UGameplayStatics::PlaySoundAtLocation(this, FlowBreakSE, GetActorLocation(), SE_Volume);
}


void AEnemy_Base::UpdateFlow(float _deltaTime) {
	//フローを使わないなら流す
	if (m_IsUseFlow == false) {
		return;
	}
	//フロー中なら
	if (m_IsFlow) {
		//フローが削れていたら補填確認
		if (Mp < Max_Mp) {
			if (FlowTimer > 2.0f) {
				Mp += 1;
				UE_LOG(LogTemp, Warning, TEXT("MpRegainNow:%d"), Mp);
			}
			else {
				FlowTimer += _deltaTime;
			}
		}
	}
	//ブレイク中なら
	else {
		//ブレイク終了か確認
		FlowTimer += _deltaTime;
		if (FlowTimer >= m_FlowBreakedTime) {
			m_IsFlow = true;
			FlowTimer = 0.0f;
		}
	}
}

//ガードを開始する関数
void AEnemy_Base::GuardStart() {
	m_MotionState = EEnemyMotionState::UpGuard;
}

//ガン見開始
void AEnemy_Base::LockOnStart() {
	m_LookPlayer = true;
}
