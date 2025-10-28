// Fill out your copyright notice in the Description page of Project Settings.


#include "BackGroundRotator.h"
#include "Kismet/GameplayStatics.h"
#include "PlayPawn.h"
#include "Camera/PlayerCameraManager.h"
#include"Camera/CameraComponent.h"


ABackGroundRotator::ABackGroundRotator() {
	PrimaryActorTick.bCanEverTick = true;
}

void ABackGroundRotator::BeginPlay() {
	Super::BeginPlay();
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	//ƒvƒŒƒCƒ„[‚Ìæ“¾
	m_pPlayer=Cast<APlayPawn>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ABackGroundRotator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	SetActorRotation(m_pPlayer->GetCameraComponent()->GetComponentRotation());
	//UE_LOG(LogTemp, Warning, TEXT("RRROROROROR"));
}

