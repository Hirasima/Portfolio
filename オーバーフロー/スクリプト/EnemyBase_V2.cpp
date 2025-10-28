// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase_V2.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include"NavigationSystem.h"
#include "AIController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

//更新処理群///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//次のフレームの頭にDestroyする状態をtrueにするかチェック
void AEnemyBase_V2::CheckDestroy() {
	//すでにtrueなら無視
	if (m_IsNextDestroy == true) {
		return;
	}

	//フラグチェックブロック///////////////////////////////////////////////////////////////////////////////////
	//次のフレームにdestroyするかどうか
	bool DestroyFlag = false;

	//アニメーションイベントで予兆があったなら反映
	if (m_IsEventNextDestroy == true) {
		DestroyFlag = true;
	}


	//trueだったなら発生する処理////////////////////////////////////////////////////////////////////////////////

	//次フレームにDestroyすることが決まったなら
	if (DestroyFlag == false) {
		return;
	}

	//自身の上記フラグをtrueで確定
	m_IsNextDestroy = true;;
	//EnemyManagerのDestroy対象配列に自信を登録
	if (m_RegisterToDestroyerCallBack.IsBound()) {
		m_RegisterToDestroyerCallBack.Execute(this);
	}
}

//Tick()君
void AEnemyBase_V2::GameTick(float _deltaTime) {
	//普通にTickは呼ぶ
	Super::Tick(_deltaTime);

	//死んでたら流す
	if (m_NowSituation == EEnemySituation_V2::Dead) {
		return;
	}
}

//実際のEnemyのDestroy実行処理(☆Destroy()が含まれているのでガチ最期に呼ぶこと！！)
void AEnemyBase_V2::ExecuteDestroy() {
	//ここで消滅時トリガー関数を呼ぶ

	//自身を破棄
	Destroy();
}


//ゲッター・セッター/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Enemyマネージャの破壊予定配列へのレジスタコールバックをセットする関数
void AEnemyBase_V2::SetRegisterToDestroyerCallBack(FRegisterToDestroyer _callback) {
	//コールバックの設定
	m_RegisterToDestroyerCallBack=_callback;
}

//現在のAnimStateを取得する関数
EEnemyAnimState_V2 AEnemyBase_V2::GetEnemyAnimState_V2() {
	return m_NowAnimState;
}
//次のフレームのAnimStateを設定する関数
void AEnemyBase_V2::SetAnimState_V2(EEnemyAnimState_V2 _state) {

	//上書き可能なら反映:比較ブロック//////////////////////////////////////////////////
	
	//①死は何よりも優先される
	if (_state == EEnemyAnimState_V2::Dead||m_NextAnimState==EEnemyAnimState_V2::Dead||m_NowAnimState==EEnemyAnimState_V2::Dead) {
		m_NextAnimState = EEnemyAnimState_V2::Dead;
		return;
	}

	//②★優先順位どうしようねぇ、、、:とりあえずプレイヤーに有利なようにStag優先設定
	if (_state == EEnemyAnimState_V2::Stag || m_NextAnimState == EEnemyAnimState_V2::Stag) {
		m_NextAnimState = EEnemyAnimState_V2::Stag;
	}

	//★そもそも同一フレーム内の話だからとりあえず優先順位もくそもないか部分もあるか？
	m_NextAnimState = _state;
}

//プレイヤーの座標を設定する関数
void AEnemyBase_V2::SetPlayerPos(FVector _pos) {
	m_PlayerPos=_pos;
}
//待機座標を設定する関数
void AEnemyBase_V2::SetWaitPos(FVector _pos) {
	m_WaitPos=_pos;
}
//一時的な移動対象座標（アラーム等）を設定する関数,第2引数:trueなら待機座標に戻るように指示
void AEnemyBase_V2::SetTargetAlartPos(FVector _pos, bool _resetFlag) {

	//リセットフラグがtrueならリセットして処理を流す
	if (_resetFlag == true) {
		//警戒場所を待機座標に初期化
		m_AlartPos=m_WaitPos;
		//警戒状態を解除
		m_IsAlart=false;
		return;
	}

	//警戒対象座標を設定
	m_AlartPos=_pos;
	//警戒状態に入る
	m_IsAlart=true;
}

//イベント関数・イベントで呼ばれる関数で上記以外//////////////////////////////////////////////////////
//次フレームに受ける総被ダメージを加算する関数
void AEnemyBase_V2::AddNextFlameDamage(int _damage) {
	m_NextFlameDamage+=_damage;
}

//Tick内で呼ぶ関数//////////////////////////////////////////////////////////////////////////////////

//前フレーム総被ダメージ→HPへの反映
void AEnemyBase_V2::ApplyHp() {
	m_hitPoint-=m_NextFlameDamage;
	//0に補正
	if (m_hitPoint < 0) {
		m_hitPoint=0;
	}
	//最大値に補正
	if (m_hitPoint > m_Max_HitPoint) {
		m_hitPoint=m_Max_HitPoint;
	}
}

//大枠ステートの変更チェック/////////////////////////////////////////////////
void AEnemyBase_V2::DefineSituation(float _deltaTime) {
	//死は絶対なので既に死なら流す
	if (m_NowSituation == EEnemySituation_V2::Dead) {
		return;
	}

	//HP切れなら死
	if (m_hitPoint <= 0) {
		m_NowSituation=EEnemySituation_V2::Dead;
		return;
	}

	//現在がパトロールなら：プレイヤーを視認したらバトルに移行
	if (m_NowSituation == EEnemySituation_V2::Patrol) {
		if (CheckPlayerFound() == true) {
			m_NowSituation=EEnemySituation_V2::Battle;
		}
		return;
	}

	//現在がバトルなら：一定時間プレイヤーを見失ったらパトロールに移行
	if (m_NowSituation == EEnemySituation_V2::Battle) {
		//プレイヤー見失い継続時間の更新
		if (CheckPlayerFound() == false) {
			m_LostPlayerTime+=_deltaTime;
		}
		else {
			m_LostPlayerTime=0.0f;
		}

		//プレイヤー見失い時間が規定時間以上ならパトロールへ
		if (m_LostPlayerTime >= m_KeepBattleTime) {
			//見失い時間の初期化
			m_LostPlayerTime=0.0f;
			m_NowSituation=EEnemySituation_V2::Patrol;
		}
	}

}
//プレイヤーを視認しているかの確認
bool AEnemyBase_V2::CheckPlayerFound() {
	//距離チェック
	if (FVector::Dist(m_PlayerPos, GetActorLocation()) > m_ViewLength) {
		return false;
	}

	//視野角チェック★マジックナンバーで視野角９０になってる
	FVector ToTargetDirection = (m_PlayerPos - GetActorLocation()).GetSafeNormal();
	//ドット積
	float Dot = FVector::DotProduct(GetActorForwardVector(), ToTargetDirection);
	//角度をデグリーに変換
	float TargetDegree = FMath::Acos(Dot) * (180.f / PI);
	if (TargetDegree > 180 / 2.0f) {
		return false;
	}

	//遮蔽チェック
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), m_PlayerPos, ECC_Visibility, Params);
	if (!bHit) {
		return false;
	}
	//ヒットしたアクターがプレイヤーならtrue
	if (bHit && HitResult.GetActor()) {
		if (HitResult.GetActor()->ActorHasTag(FName("Player"))) {
			return true;
		}
	}

	return false;
}


//大枠ステートごとの分岐処理:Deadはそもそもreturnなので無し,☆ここでAnimState決めちゃおう/////////////

//パトロール
void AEnemyBase_V2::DoPatrol() {
	//警戒時の処理/////////////////////////////////////////////////////
	if (m_IsAlart == true) {
		return;
	}


	//一時警戒位置を目指して移動する：巡回など////////////////////////////
	//AIコントローラの取得
	AAIController* m_pController= Cast<AAIController>(GetController());
	
	//一時警戒位置との距離が離れていて、ステートがWaitなら:多重呼び出し防止
	if(FVector::Dist(GetActorLocation(),m_AlartPos)>50.0f&&m_NowAnimState==EEnemyAnimState_V2::Wait){
		//次フレーム予定アニメーションをWalkに
		SetAnimState_V2(EEnemyAnimState_V2::Walk);
		if (m_NextAnimState == EEnemyAnimState_V2::Walk) {
			//一時警戒位置に移動
			m_pController->MoveToLocation(m_AlartPos);
		}
	}
	//ほぼ到着しているなら
	else if(m_NowAnimState==EEnemyAnimState_V2::Walk){
		//次フレーム予定アニメーションをWaitに
		SetAnimState_V2(EEnemyAnimState_V2::Wait);
		//停止
		m_pController->StopMovement();
	}
	
}

//バトル///////////
void AEnemyBase_V2::DoBattle() {

}

//決定されたNextAnimStateをNowAnimStateに反映+Nextの初期化
void AEnemyBase_V2::ApplyAnimState() {

}