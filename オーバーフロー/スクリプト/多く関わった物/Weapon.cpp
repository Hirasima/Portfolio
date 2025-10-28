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

	//���b�V���̏�����
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = m_pStaticMeshComp;

	//�R���W�����̏�����
	m_pWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	m_pWeaponCollision->SetupAttachment(RootComponent);

	m_pWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//�i�C�A�K���R���|�[�l���g������
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
	
	//����̐F�𔽉f
	UMaterialInterface* BaseMat = m_pStaticMeshComp->GetMaterial(2);

	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(2, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), Color);
	UE_LOG(LogTemp, Warning, TEXT("CHANGECOOO"));

	//�i�C�A�K���R���|�[�l���g�Ƀi�C�A�K���V�X�e�����Z�b�g
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

//�R���W�����̗L�����ݒ�
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

//����̋O���̗L���ݒ�
void AWeapon::SetBladeOrbitActive(bool _enable)
{
	//�i�C�A�K���R���|�[�l���g�̗L���`�F�b�N
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetActive(_enable);
}

//����̋O���̐F�̐ݒ�
void AWeapon::SetBladeOrbitColor(FLinearColor _linearColor)
{
	//�i�C�A�K���R���|�[�l���g�̗L���`�F�b�N
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetNiagaraVariableLinearColor(TEXT("Color"), _linearColor);
}

//����̋O���̕��̐ݒ�
void AWeapon::SetBladeOrbitWidth(float _width)
{
	//�i�C�A�K���R���|�[�l���g�̗L���`�F�b�N
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetVariableFloat(TEXT("Width"), _width);
}

//����̃G�t�F�N�g�̃p�����[�^�ݒ�
void AWeapon::SetBladeOrbitEffectParam(float _spawnRate, float _size)
{
	//�i�C�A�K���R���|�[�l���g�̗L���`�F�b�N
	if (!m_pNiagaraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon : NiagaraComponent Is None"));
		return;
	}
	m_pNiagaraComponent->SetVariableFloat(TEXT("SpawnRate"), _spawnRate);
	m_pNiagaraComponent->SetVariableFloat(TEXT("ParticleSize"), _size);
}

//����̍U����Ԑݒ�p
void AWeapon::SetAttacking(bool _enable)
{
	m_bAttacking = _enable;

	//�R���W������ύX
	SetCollisionActive(_enable);

	//����̋O����ύX
	SetBladeOrbitActive(_enable);
}

//�v���~�e�B�u�R���|�[�l���g�̎擾
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

	//����̐F�𔽉f
	UMaterialInterface* BaseMat = m_pStaticMeshComp->GetMaterial(_materialIndex);

	UMaterialInstanceDynamic* DyMat = m_pStaticMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(_materialIndex, BaseMat);
	DyMat->SetVectorParameterValue(FName("Color"), _color);
	DyMat->SetScalarParameterValue(FName("IsPoint"), _point);
	
}