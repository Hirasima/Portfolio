// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleTile_Up.h"

ATriangleTile_Up::ATriangleTile_Up()
{
	FString path = FString("/Game/3DModel/2Dtile1.2Dtile1");
	UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *path));

	if (mesh)
	{
		//UE_LOG(LogTemp, Log, TEXT("StaticMesh was found"));
		GetStaticMeshComponent()->SetStaticMesh(mesh);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("StaticMesh was not found."))
	}
}