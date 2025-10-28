// camera3D
// 23cu0228 hanawa

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "camera3D.generated.h"

UCLASS()
class Acamera3D : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	Acamera3D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateCamera(float DeltaTime);

public:
	UPROPERTY(EditAnywhere, category = "RotationCam")
	float m_RotationValue;
};
