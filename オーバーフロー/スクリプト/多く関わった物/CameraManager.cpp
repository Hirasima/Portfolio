// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//スプリングアームの初期化
	m_pSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	m_pSpringArmComp->SetupAttachment(RootComponent);
	m_pSpringArmComp->SetRelativeLocation(m_springArmOffset);

	m_pSpringArmComp->bDoCollisionTest = false;
	m_pSpringArmComp->ProbeChannel = ECC_Camera;

	//カメラの初期化
	m_pCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	m_pCameraComp->SetupAttachment(m_pSpringArmComp);
}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	//自動ポストプロセスを黙らせる
	m_pCameraComp->PostProcessSettings.bOverride_AutoExposureMethod = true;
	m_pCameraComp->PostProcessSettings.bOverride_AutoExposureMinBrightness = true;
	m_pCameraComp->PostProcessSettings.bOverride_AutoExposureMaxBrightness = true;
	m_pCameraComp->PostProcessSettings.bOverride_AutoExposureBias = true;

	m_pCameraComp->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
	m_pCameraComp->PostProcessSettings.AutoExposureMinBrightness = 1.0f;
	m_pCameraComp->PostProcessSettings.AutoExposureMaxBrightness = 1.0f;
	m_pCameraComp->PostProcessSettings.AutoExposureBias = 10.0f;
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

