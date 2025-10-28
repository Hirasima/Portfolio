// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerForceMover.h"
#include "PlayPawn.h"
#include"Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include"GuideCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerForceMover::APlayerForceMover()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerForceMover::BeginPlay()
{
	Super::BeginPlay();
	m_IsValiable = true;
	m_IsTargetFalling = false;
	m_fallingTimer = 0.0f;

	if (m_pStartTrrigerCollision) {
		m_pStartTrrigerCollision->OnActorBeginOverlap.AddDynamic(this, &APlayerForceMover::StartFallOut);
	}
	if (m_pEndTrrigerCollision) {
		m_pEndTrrigerCollision->OnActorBeginOverlap.AddDynamic(this, &APlayerForceMover::EndFallOut);
	}
	
}

// Called every frame
void APlayerForceMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//現在ターゲットが落下中ならターゲットの座標にナイアガラを出す
	if (m_IsTargetFalling) {
		if (!m_pTargetPlayer) {
			return;
		}
		m_fallingTimer += DeltaTime;
		if (m_fallingTimer >= 0.03f) {
			FVector target = m_pTargetPlayer->GetActorLocation();
			target.Z += 100.0f;
			target.X += 20.0f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, m_pBreakingGlassNia, target, GetActorRotation());
			//タイマーリセット
			m_fallingTimer = 0.0f;
		}
	}
}

void APlayerForceMover::StartFallOut(AActor* _a, AActor* _b) {
	//プレイヤーじゃないなら処理を流す
	if (!_b->IsA(APlayPawn::StaticClass())) {
		return;
	}
	//座標を強制変更
	APlayPawn* Pl = Cast<APlayPawn>(_b);
	m_pTargetPlayer = Pl;
	Pl->SetPlayerAnimState(EPlayerAnimState::Fall_Out);
	Pl->GetCharacterMovement()->DisableMovement();
	Pl->TeleportTo(m_pStartTrrigerCollision->GetActorLocation(), GetActorRotation(), false, true);
	Pl->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	Pl->ConsumeMovementInputVector();
	Pl->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	//ガイドをプレイヤーの子供にする
	AGuideCharacter* Guid = Cast<AGuideCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AGuideCharacter::StaticClass()));
	if (Guid) {
		FVector RelativeLocation = FVector(20.0f, 0.0f, 30.0f );

		Guid->AttachToComponent(Pl->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		Guid->SetActorRelativeLocation(RelativeLocation);
		Guid->SetActorRelativeRotation(FRotator(0.0f,180.0f,0.0f));
		Guid->BeChild();
		if (auto Movement = Guid->GetCharacterMovement()) {
			Movement->DisableMovement();
		}
		Guid->SetActorEnableCollision(false);
	}

	m_IsTargetFalling = true;
}

void APlayerForceMover::EndFallOut(AActor* _a, AActor* _b) {
	if (m_IsValiable == false) {
		return;
	}

	//プレイヤーじゃないなら処理を流す
	if (!_b->IsA(APlayPawn::StaticClass())) {
		return;
	}

	APlayPawn* Pl = Cast<APlayPawn>(_b);
	Pl->SetPlayerAnimState(EPlayerAnimState::Idle);

	//ガイドをプレイヤーの子供にする
	AGuideCharacter* Guid = Cast<AGuideCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AGuideCharacter::StaticClass()));
	if (Guid) {
		FVector RelativeLocation = FVector(0.0f, 0.0f, 30.0f);
		Guid->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Guid->SetActorRelativeLocation(RelativeLocation);
	}

	m_IsValiable = false;
	m_IsTargetFalling = false;
}

