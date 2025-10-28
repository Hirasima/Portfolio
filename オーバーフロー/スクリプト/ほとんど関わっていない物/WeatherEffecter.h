// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherEffecter.generated.h"

//�N���X�̑O���錾
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
	//�J�G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather_Effect")
	UNiagaraSystem* RainEffect;

private:
	UPROPERTY()
	UNiagaraComponent* WeatherNiagaraComp;
};
