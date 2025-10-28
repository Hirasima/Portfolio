// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGameInstance.h"

UItemGameInstance::UItemGameInstance()
{
	ItemFlag.SetNum(10);
	for (int i = 0; i < 10; ++i)
	{
		ItemFlag[i] = false;
	}
}
