// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherEffecter.generated.h"

//クラスの前方宣言
class UNiagaraSystem;
class UCameraComponent;
class UNiagaraComponent;

UCLASS()
class ORIGINALACTION_API AWeatherEffecter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherEffecter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//雨エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather_Effect")
	UNiagaraSystem* RainEffect;

private:
	UPROPERTY()
	UNiagaraComponent* WeatherNiagaraComp;
};
