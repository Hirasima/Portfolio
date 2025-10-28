// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffBitActor.h"
#include "Enemy_Base.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABuffBitActor::ABuffBitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//メッシュの初期化
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = m_pStaticMeshComp;

	//ナイアガラコンポーネント初期化
	m_pOrbitComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	m_pOrbitComp->SetupAttachment(m_pStaticMeshComp);
	m_pOrbitComp->SetActive(false);


}

// Called when the game starts or when spawned
void ABuffBitActor::BeginPlay()
{
	Super::BeginPlay();
	
	m_pMaterial = m_pStaticMeshComp->GetMaterial(0);

	m_pOrbitComp->SetAsset(m_pBitOrbit);
	m_pOrbitComp->SetActive(false);

	//m_pBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBitBeam, GetActorLocation());
	//m_pBeamComponent->Deactivate();

	m_eBitState = EBuffPattern::None;

	m_bIsAttacking = false;

	//上記のマテリアルに色を付与して登録
	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
	DyMat->SetVectorParameterValue(FName("Color"), NoneBuff_Color);
}

// Called every frame
void ABuffBitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMove(DeltaTime);

	UpdateAttack(DeltaTime);
}

//バフを伝達される関数
void ABuffBitActor::SetBuffType(EBuffPattern _bufftype) {
	//ステート設定
	m_eBitState = _bufftype;

	//ステートごとに分岐
	switch (_bufftype)
	{
	case EBuffPattern::None:
	{
		//メッシュを変更
		m_pStaticMeshComp->SetStaticMesh(m_pCircleModeMesh);
		//動的マテリアルを取得
		UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
		//色変更
		DyMat->SetVectorParameterValue(FName("Color"), NoneBuff_Color);
		//軌道エフェクトを消す
		m_pOrbitComp->SetActive(false);

		//点滅モードなら
		if (IsPoint)
		{
			//マテリアルを点滅に設定
			DyMat->SetScalarParameterValue(FName("IsPoint"), IsPoint);
		}

		break;
	}
	case EBuffPattern::SpeedUp:
	{
		//メッシュを変更
		m_pStaticMeshComp->SetStaticMesh(m_pCircleModeMesh);
		//動的マテリアルを取得
		UMaterialInstanceDynamic* DyMat1 = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
		//色変更
		DyMat1->SetVectorParameterValue(FName("Color"), SpeedBuff_Color);
		//軌道エフェクトを消す
		m_pOrbitComp->SetActive(false);
		
		//点滅モードなら
		if (IsPoint)
		{
			//マテリアルを点滅に設定
			DyMat1->SetScalarParameterValue(FName("IsPoint"), IsPoint);
		}

		break;
	}
	case EBuffPattern::AdditionalAttack:
	{
		//メッシュを変更
		m_pStaticMeshComp->SetStaticMesh(m_pAttackModeMesh);
		//動的マテリアルを取得
		UMaterialInterface* MetalMat=m_pAttackModeMesh->GetMaterial(0);
		UMaterialInstanceDynamic* Metal = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MetalMat);
		UMaterialInstanceDynamic* DyMat2 = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(1, m_pMaterial);

		//色変更
		DyMat2->SetVectorParameterValue(FName("Color"), AttackBuff_Color);
		//デタッチ
		RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		//
		FVector v = m_pParentSkeltalMeshComp->GetComponentLocation();
		v.Z += 100;

		m_pBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBitAwake, v);
		m_pBeamComponent->SetVariableVec3(FName("ConvergencePos"), GetActorLocation());

		//点滅モードなら
		if (IsPoint)
		{
			//マテリアルを点滅に設定
			DyMat2->SetScalarParameterValue(FName("IsPoint"), IsPoint);
		}

		break;
	}
	default:
		break;
	}
}

//ソケット名設定
void ABuffBitActor::SetSocketName(FName _idolSocket, FName _activeSocket)
{
	m_idolSocketName = _idolSocket;
	m_activeSocketName = _activeSocket;
}

//親スケルタルメッシュ設定
void ABuffBitActor::SetParentSkeltalMeshComp(USkeletalMeshComponent* _skeltalMeshComp)
{
	m_pParentSkeltalMeshComp = _skeltalMeshComp;
}

//点滅切り替え設定
void ABuffBitActor::SetBitIsPoint(bool _anable)
{
	IsPoint = _anable;

	SetBuffType(m_eBitState);

	/*if (IsPoint && m_eBitState != EBuffPattern::None)
	{
	//☆彡色の補填ができてなくて強制初期化（赤）になっている
		UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_pMaterial);
		DyMat->SetScalarParameterValue(FName("IsPoint"), _anable);
	}*/
}

//引数の敵に攻撃する関数
void ABuffBitActor::AttackTargetActor(AActor* _actor)
{
	if (!_actor) {
		return;
	}
	
	//タイマー初期化
	attackTimer = 0.0f;

	//攻撃中フラグを有効に
	m_bIsAttacking = true;
	m_bAttacked = false;

	//攻撃対象設定
	m_pAttckActor = _actor;

	//攻撃する位置を設定//////////////////

	//攻撃対象の位置
	FVector actorPos = _actor->GetActorLocation();
	//攻撃位置
	FVector attackPos;

	//回転を決める
	FRotator attackRot = {
		FMath::RandRange(AttackMinRotation.Pitch, AttackMaxRotation.Pitch),
		FMath::RandRange(AttackMinRotation.Yaw, AttackMaxRotation.Yaw),
		FMath::RandRange(AttackMinRotation.Roll, AttackMaxRotation.Roll)
	};

	//回転とターゲット在庫座標から攻撃位置を決める
	attackPos.X = AttackRadius * FMath::Cos(FMath::DegreesToRadians(attackRot.Pitch)) * FMath::Cos(FMath::DegreesToRadians(attackRot.Yaw));
	attackPos.Y = AttackRadius * FMath::Cos(FMath::DegreesToRadians(attackRot.Pitch)) * FMath::Sin(FMath::DegreesToRadians(attackRot.Yaw));
	attackPos.Z = AttackRadius * FMath::Sin(FMath::DegreesToRadians(attackRot.Pitch));

	//ターゲット位置に設定
	m_attackPos = actorPos + attackPos;
	m_attackRot = (actorPos - m_attackPos).Rotation();
	//UE_LOG(LogTemp, Log, TEXT("Positon : %f, %f , %f"), m_attackRot.Pitch, m_attackRot.Yaw, m_attackRot.Roll);

	//メッシュのねじれを直したかった
	m_attackRot.Roll += -m_attackRot.Pitch;
	m_attackRot.Yaw -= 90.0f;

	//UE_LOG(LogTemp, Log, TEXT("Positon : %f, %f , %f"), m_attackRot.Pitch, m_attackRot.Yaw, m_attackRot.Roll);

	m_pOrbitComp->SetActive(true);
}

//移動更新
void ABuffBitActor::UpdateMove(float DeltaTime)
{
	//アタッチ済みなら移動しない
	if (GetAttachParentActor())
	{
		//UE_LOG(LogTemp, Log, TEXT("Attaching"));
		return;
	}

	//移動先の座標
	FVector targetPos;

	//移動先の回転
	FRotator targetRot;

	//アクティブ時
	if (m_eBitState == EBuffPattern::AdditionalAttack)
	{
		//攻撃時
		if (m_bIsAttacking)
		{
			//UE_LOG(LogTemp, Log, TEXT("BitAttack"));
			//攻撃地点設定
			targetPos = m_attackPos;
			targetRot = m_attackRot;
		}
		//追従時
		else
		{
			targetPos = m_pParentSkeltalMeshComp->GetSocketLocation(m_activeSocketName);
			targetRot = m_pParentSkeltalMeshComp->GetSocketRotation(m_activeSocketName);
		}
	}
	//非アクティブ時
	else
	{
		targetPos = m_pParentSkeltalMeshComp->GetSocketLocation(m_idolSocketName);
		targetRot = m_pParentSkeltalMeshComp->GetSocketRotation(m_idolSocketName);

		//もし距離が近かったら
		if ((targetPos - GetActorLocation()).Length() <= 50.0f)
		{
			//ソケットに接続
			AttachToComponent(m_pParentSkeltalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, m_idolSocketName);
			return;
		}
	}

	timer += DeltaTime;
	
	//移動量ベクトル
	FVector moveVec;
	FRotator moveRot;

	//線形補間で移動量を計算
	moveRot = FMath::RInterpTo(GetActorRotation(), targetRot, DeltaTime, 45.0f);
	moveVec = FMath::VInterpTo(GetActorLocation(), targetPos, DeltaTime, 10.0f);

	//上下に揺らす
	moveVec.Z += 0.7f * FMath::Sin(timer * 2);

	//移動
	SetActorRotation(moveRot);
	SetActorLocation(moveVec);

}

//攻撃処理
void ABuffBitActor::UpdateAttack(float DeltaTime)
{
	//攻撃状態でないなら終了
	if (!m_bIsAttacking)return;

	//攻撃時間更新
	attackTimer += DeltaTime;

	//攻撃する地点までの距離が近ければかつ攻撃済みでなければ
	if ((m_attackPos - GetActorLocation()).Length() <= 20.0f && !m_bAttacked)
	{
		//攻撃地点に移動
		SetActorLocation(m_attackPos);

		if (!m_pBeamComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Beam Niagara Comp Is None"));
			return;
		}

		//ビットビームエフェクトをスポーン
		m_pBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBitBeam, GetActorLocation());
		//ビーム終点を設定
		if (m_pAttckActor) {
			m_pBeamComponent->SetVariableVec3(FName("BeamEnd"), m_pAttckActor->GetActorLocation());
		}
		//攻撃済み
		m_bAttacked = true;

		//UE_LOG(LogTemp, Log, TEXT("Beeeeeeeeeeeeeeeam"));
	}

	//攻撃時間が終わったかつ攻撃済みなら
	if (attackTimer >= 0.5f && m_bAttacked)
	{
		//攻撃中フラグを下げる
		m_bIsAttacking = false;
		//攻撃済みフラグを下げる
		m_bAttacked = false;
		//軌道エフェクトを消す
		m_pOrbitComp->SetActive(false);
	}
}