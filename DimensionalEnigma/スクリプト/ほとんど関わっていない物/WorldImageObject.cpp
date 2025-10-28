// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldImageObject.h"

// Sets default values
AWorldImageObject::AWorldImageObject()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// メッシュコンポーネントの設定
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// デフォルトメッシュの適用
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (DefaultMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(DefaultMesh.Object);
	}
}

// Called when the game starts or when spawned
void AWorldImageObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (!StaticMeshComponent->GetStaticMesh())
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
		if (DefaultMesh.Succeeded())
		{
			StaticMeshComponent->SetStaticMesh(DefaultMesh.Object);
		}
	}
}

// Called every frame
void AWorldImageObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 現在の回転を取得
	FRotator CurrentRotation = GetActorRotation();

	// 回転を定義
	FRotator NewRotation = CurrentRotation + FRotator(0.0f, RotateAmount * DeltaTime, 0.0f);

	// アクターの回転を更新
	SetActorRotation(NewRotation);
}

// Called to bind functionality to input
void AWorldImageObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

