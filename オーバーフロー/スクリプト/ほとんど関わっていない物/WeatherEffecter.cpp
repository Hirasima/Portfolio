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

	//プレイヤーにナイアガラを設定
	AActor* TargetActor= UGameplayStatics::GetActorOfClass(GetWorld(), APlayPawn::StaticClass());
	//ナイアガラエフェクトを登録
	WeatherNiagaraComp->SetAsset(RainEffect);
	//プレイヤーの子オブジェクトに（追従のため）
	WeatherNiagaraComp->AttachToComponent(TargetActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//位置オフセット
	WeatherNiagaraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	
}

// Called every frame
void AWeatherEffecter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

