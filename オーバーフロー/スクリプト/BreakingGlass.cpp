// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakingGlass.h"
#include "Components/BoxComponent.h"
#include "PlayPawn.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ABreakingGlass::ABreakingGlass() {
	//�C���^���N�V�����p�R���W�����̍쐬
	m_pNiagaraTarget = CreateDefaultSubobject<UBoxComponent>(TEXT("NiagaraTargetCollision"));
	m_pNiagaraTarget->SetupAttachment(RootComponent);

	//��L�R���W�����̐ݒ�///////////////////////////////////////////////////////////////////////

	//���������A����̂�
	m_pNiagaraTarget->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//�S�`�����l��
	m_pNiagaraTarget->SetCollisionObjectType(ECC_WorldDynamic);
	m_pNiagaraTarget->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_pNiagaraTarget->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	m_pNiagaraTarget->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//�I�[�o�[���b�v�C�x���g����
	m_pNiagaraTarget->SetGenerateOverlapEvents(false);

	////////////////////////////////////////////////////////////////////////////////////////////////
}

void ABreakingGlass::BeginPlay() {
	Super::BeginPlay();
	//�ŏ��̓C���^���N�V�����L��
	m_isValiable = true;
}

void ABreakingGlass::UseInteraction() {
	if (!m_pBreakingGlassNia) {
		return;
	}

	Super::UseInteraction();

	//�������I�ɕǌ������Ƀ��[�v�����Ă݂�
	//���W�������ύX
	APlayPawn* Pl=Cast<APlayPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayPawn::StaticClass()));
	if (!Pl) { 
		return; 
	}	
	Pl->GetCharacterMovement()->DisableMovement();
	Pl->TeleportTo(m_pNiagaraTarget->GetComponentLocation(), Pl->GetActorRotation(), false, true);
	Pl->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	Pl->ConsumeMovementInputVector();
	Pl->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	//�����p�K���X�Ȃ�v���C���[�𗎉��p�A�j���[�V�����ɐݒ�
	Pl->SetPlayerAnimState(EPlayerAnimState::Fall_GlassBreak);
	Pl->ReverceCamYaw();
		
	//�X���E
	Pl->BeSlow(1.0f,0.5f);

	FVector SpawnOffset = m_pNiagaraTarget->GetComponentTransform().GetLocation();
	//�K���X�̔j�Ђ��o�������Ď���
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, m_pBreakingGlassNia, SpawnOffset, GetActorRotation());

	GetStaticMeshComponent()->SetVisibility(false);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_isValiable = false;
	this->Destroy();
}

