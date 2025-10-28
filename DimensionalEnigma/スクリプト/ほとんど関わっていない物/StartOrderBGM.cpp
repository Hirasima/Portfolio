// Fill out your copyright notice in the Description page of Project Settings.


#include "StartOrderBGM.h"
#include "SoundManagerSubsystem.h"//BGMを流すためにインクルード

// Sets default values
AStartOrderBGM::AStartOrderBGM()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartOrderBGM::BeginPlay()
{
	Super::BeginPlay();
	
    //BGMを再生
    USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
    if (SoundManager)
    {
        // パスで音源を再生
        SoundManager->PlayBGM(BGM_Path);

        UE_LOG(LogTemp, Warning, TEXT("PlayBGM"))
    }
    //役割を終えたため削除しておく
    this->Destroy();
}

// Called every frame
void AStartOrderBGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStartOrderBGM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

