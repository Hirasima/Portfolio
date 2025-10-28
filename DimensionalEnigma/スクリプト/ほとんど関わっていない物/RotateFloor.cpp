// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateFloor.h"

// Sets default values
ARotateFloor::ARotateFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotateFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotateFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

