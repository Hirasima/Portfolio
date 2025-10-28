// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include"Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values
AWeapon::AWeapon()
	:m_pStaticMeshComp(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//メッシュの初期化
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = m_pStaticMeshComp;

	//コリジョンの初期化
	m_pWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	m_pWeaponCollision->SetupAttachment(RootComponent);

	m_pWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//ナイアガラコンポーネント初期化
	m_pNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	m_pNiagaraComponent->SetupAttachment(RootComponent);
	m_pNiagaraComponent->AttachToComponent(
		m_pStaticMeshComp,
		FAttachmentTransformRules::KeepRelativeTransform,
		FName("BladeOrbit")
	);
	m_pNiagaraComponent->SetActive(false);

	//m_pWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlap);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	//武器の色を反映
	UMaterialInterface* BaseMat = m_pStaticMeshComp->GetMaterial(2);

	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(2, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), Color);
	UE_LOG(LogTemp, Warning, TEXT("CHANGECOOO"));

	//ナイアガラコンポーネントにナイアガラシステムをセット
	m_pNiagaraComponent->SetAsset(m_pBladeOrbit);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWeapon::GetIsPoint() {
	return m_IsPoint;
}
void AWeapon::SetIsPoint(bool _point,int _index) {
	m_IsPoint = _point;
	ChangeColor(Color, _point, _index);
}

//コリジョンの有効化設定
void AWeapon::SetCollisionActive(bool _enable)
{
	ECollisionEnabled::Type enable;

	if (_enable)
	{
		enable = ECollisionEnabled::QueryOnly;
	}
	else
	{
		enable = ECollisionEnabled::NoCollision;
	}

	m_pWeaponCollision->SetCollisionEnabled(enable);
}

//武器の軌道の有効設定
void AWeapon::SetBladeOrbitActive(bool _enable)
{
	//ナイアガラコンポーネントの有効チェック
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetActive(_enable);
}

//武器の軌道の色の設定
void AWeapon::SetBladeOrbitColor(FLinearColor _linearColor)
{
	//ナイアガラコンポーネントの有効チェック
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetNiagaraVariableLinearColor(TEXT("Color"), _linearColor);
}

//武器の軌道の幅の設定
void AWeapon::SetBladeOrbitWidth(float _width)
{
	//ナイアガラコンポーネントの有効チェック
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetVariableFloat(TEXT("Width"), _width);
}

//武器のエフェクトのパラメータ設定
void AWeapon::SetBladeOrbitEffectParam(float _spawnRate, float _size)
{
	//ナイアガラコンポーネントの有効チェック
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetVariableFloat(TEXT("SpawnRate"), _spawnRate);
	m_pNiagaraComponent->SetVariableFloat(TEXT("ParticleSize"), _size);
}

//武器の攻撃状態設定用
void AWeapon::SetAttacking(bool _enable)
{
	m_bAttacking = _enable;

	//コリジョンを変更
	SetCollisionActive(_enable);

	//武器の軌道を変更
	SetBladeOrbitActive(_enable);
}

//プリミティブコンポーネントの取得
UPrimitiveComponent* AWeapon::GetPrimitiveComponent()
{
	return Cast<UPrimitiveComponent>(m_pWeaponCollision);
}

void AWeapon::OnWeaponOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
	bool _bFromSweep, const FHitResult& _sweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon : Overlap!!!!!!"))
}

void AWeapon::ChangeColor(FLinearColor _color,bool _point,int _materialIndex) {
	m_IsPoint = _point;

	//武器の色を反映
	UMaterialInterface* BaseMat = m_pStaticMeshComp->GetMaterial(_materialIndex);

	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(_materialIndex, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), _color);
	DyMat->SetScalarParameterValue(FName("IsPoint"), _point);
	
}