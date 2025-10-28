// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChaseOrderCamera.generated.h"

UCLASS()
class AChaseOrderCamera : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditANywhere, Category = "Property")
	float m_AddHeight = 50.0f;
	FVector m_CameraPos;

public:
	// Sets default values for this pawn's properties
	AChaseOrderCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditANywhere, Category = "Actor")
	AActor* m_pPlayer;
	UPROPERTY(EditANywhere, Category = "Actor")
	AActor* m_pCamera;
	
};
