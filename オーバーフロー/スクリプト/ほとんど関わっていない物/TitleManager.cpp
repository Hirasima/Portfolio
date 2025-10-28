// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleManager.h"
#include "Kismet/GameplayStatics.h"
#include"TitleWidget.h"

// Sets default values
ATitleManager::ATitleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATitleManager::BeginPlay()
{
	Super::BeginPlay();

	//WidgetÇèoÇ∑
	m_pWidget=CreateWidget<UTitleWidget>(GetWorld(), m_pTitleWidgetClass);
	m_pWidget->AddToViewport(99);
}

// Called every frame
void ATitleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

