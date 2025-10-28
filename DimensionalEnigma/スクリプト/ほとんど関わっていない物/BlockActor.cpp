// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockActor.h"

ABlockActor::ABlockActor()
{
	m_defaultPath = FString("/Game/3DModel/Block.Block");
	UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *m_defaultPath));

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

//String型のセーブデータを取得
FString ABlockActor::GetSaveDataToString()
{
	/*FString meshPath = GetStaticMeshComponent()->GetStaticMesh()->GetPathName();
	return meshPath;*/
	return TEXT("");
}

//String型のセーブデータを設定
void ABlockActor::SetSaveDataToString(const FString _saveData)
{
	/*UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *_saveData));

	if (mesh)
	{
		UE_LOG(LogTemp, Log, TEXT("StaticMesh was found"));
		GetStaticMeshComponent()->SetStaticMesh(mesh);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMesh was not found."))
	}*/
}