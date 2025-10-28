// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherEffecter.h"
#include "Camera/CameraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "PlayPawn.h"

// Sets default values
AWeatherEffecter::AWeatherEffecter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeatherNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeatherNiagaraComp"));
}

// Called when the game starts or when spawned
void AWeatherEffecter::BeginPlay()
{
	Super::BeginPlay();

	//�v���C���[�Ƀi�C�A�K����ݒ�
	AActor* TargetActor= UGameplayStatics::GetActorOfClass(GetWorld(), APlayPawn::StaticClass());
	//�i�C�A�K���G�t�F�N�g��o�^
	WeatherNiagaraComp->SetAsset(RainEffect);
	//�v���C���[�̎q�I�u�W�F�N�g�Ɂi�Ǐ]�̂��߁j
	WeatherNiagaraComp->AttachToComponent(TargetActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//�ʒu�I�t�Z�b�g
	WeatherNiagaraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	
}

// Called every frame
void AWeatherEffecter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

