// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideCharacter.h"
#include"Kismet/GameplayStatics.h"
#include"AIController.h"
#include"Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

// Sets default values
AGuideCharacter::AGuideCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�����ړ�Ai��ݒ�
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AGuideCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_AnimState = EGuideAnimState::Wait;
	m_ControlleState = EAutoMoveState::None;
	m_nowRouteNum = 0;
}

// Called every frame
void AGuideCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_AnimState == EGuideAnimState::Hug) {
		return;
	}
	if (!m_Routes.IsValidIndex(m_nowRouteNum)) {
		return;
	}
	//�R���g���[�����擾
	AAIController* m_AI = Cast<AAIController>(GetController());
	if (!m_AI) {
		return;
	}
	FAIMoveRequest Req;

	//���݂̈ړ����W�b�N�ɍ��킹�Ĉړ�/////////////////////////////////////////
	switch (m_Routes[m_nowRouteNum].MoveLogic)
	{
	case EAutoMoveState::None:
		break;
	case EAutoMoveState::Auto:
		//�ڕW�n�_�Ɍ�����
		Req.SetGoalLocation(m_Routes[m_nowRouteNum].TrrigerPos);
		Req.SetAcceptanceRadius(100.0f);
		break;
	case EAutoMoveState::Trace:
		Req.SetGoalActor(GetWorld()->GetFirstPlayerController());
		Req.SetAcceptanceRadius(150.0f);
		break;
	default:
		break;
	}

	//�ړ�
	FNavPathSharedPtr NavPtr;
	m_AI->MoveTo(Req, &NavPtr);

	//�A�j���[�V�����X�e�[�g��ݒ�
	if (GetVelocity().IsNearlyZero()) {
		m_AnimState = EGuideAnimState::Wait;
	}
	else {
		m_AnimState = EGuideAnimState::Dash;
	}
	//////////////////////////////////////////////////////////////////////////////////

	//���ݑΏۍ��W�ɓ��B���Ă���Ȃ�󋵂��X�V
	if (FVector::Dist(m_Routes[m_nowRouteNum].TrrigerPos, GetActorLocation()) <= 150.0f) {
		m_nowRouteNum += 1;
	}

}

EGuideAnimState AGuideCharacter::GetGuideAnimState() {
	return m_AnimState;
}

void AGuideCharacter::BeChild() {
	m_AnimState = EGuideAnimState::Hug;
}


