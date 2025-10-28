// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSelect.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Components/InputComponent.h" 
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h"

#include "Kismet/KismetSystemLibrary.h" // ’Ç‰Á
#include "Kismet/GameplayStatics.h" // ’Ç‰Á
#include "EnigmaGameInstance.h"
#include "GameManager.h"
#include "StageSubSystem.h"



// Sets default values
AWorldSelect::AWorldSelect()
	: m_WorldName()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldSelect::BeginPlay()
{
	Super::BeginPlay();

	// GameManager‚ðŽæ“¾
	world = GetWorld();
	if (world) {
		gameinstance = Cast<UEnigmaGameInstance>(world->GetGameInstance());
		if(gameinstance)
		{
			gamemanager = world->GetSubsystem<UStageSubSystem>();

		}

	}

	// Tag ‚ðÝ’è
	Tags.Add(FName(TEXT("WorldSelect")));
}

// Called every frame
void AWorldSelect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetNextWorld();

	DebugNextWorld();

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
	{
		UE_LOG(LogTemp, Log, TEXT("SelectLocation %s"), *SelectLocation.ToString());
	}
}

void AWorldSelect::SetNextWorld()
{
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + FVector(0.f, 0.f, 30.f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult OutHit;
	bool isHit = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, FCollisionObjectQueryParams::AllObjects, CollisionParams);
	if (isHit)
	{
		AActor* HitActor = OutHit.GetActor();

		if (HitActor->ActorHasTag("Player"))
		{
			if (gamemanager->GetIsSelect())
			{
				SelectLocation = GetActorLocation();
				LoadNextWorld();
				UE_LOG(LogTemp, Warning, TEXT("NextWorld %s"), *SelectLocation.ToString());
			}

		}
	}
}

void AWorldSelect::LoadNextWorld()
{
	UGameplayStatics::OpenLevel(GetWorld(), m_WorldName);
}

void AWorldSelect::DebugNextWorld()
{
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(m_WorldNumber))
	{
		LoadNextWorld();
	}
}

FVector AWorldSelect::GetSelectLocation()
{
	return SelectLocation;
}
