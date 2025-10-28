// camera3D.cpp
// 23cu0228 hanawa

#include "camera3D.h"

// Sets default values
Acamera3D::Acamera3D()
	: m_RotationValue(0.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Acamera3D::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void Acamera3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCamera(DeltaTime);
}

// Called to bind functionality to input
void Acamera3D::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// ƒJƒƒ‰‚ÌXVˆ—
void Acamera3D::UpdateCamera(float DeltaTime)
{
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
	{
		AddActorLocalRotation(FRotator(0.0f, -m_RotationValue, 0.0f));
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
	{
		AddActorLocalRotation(FRotator(0.0f, m_RotationValue, 0.0f));
	}



}

