// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChangePawnTest.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelChangePawnTest::ALevelChangePawnTest()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelChangePawnTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelChangePawnTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Y)) {
		UE_LOG(LogTemp, Warning, TEXT("Y key is pressed"));
		ChangeLevel();
	}
}

// Called to bind functionality to input
void ALevelChangePawnTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void ALevelChangePawnTest::ChangeLevel()
{
	// ˆÚ“®æ‚ÌƒŒƒxƒ‹–¼
	FName LevelName = "LevelChangeTest_B";

	// ƒŒƒxƒ‹‚ğ•ÏX
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

