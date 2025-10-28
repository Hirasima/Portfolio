// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "DoorSwitcher.generated.h"

/**
 * 
 */

 //子機と導線を管理する構造体
USTRUCT(BlueprintType)
struct FChildMachinePackage
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class ALineLight*> TargetLineLights;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AInteractionActor* ChildMachine;
};

//ドアと移動ご座標を管理する構造体
USTRUCT(BlueprintType)
struct FDoorInformation
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AStaticMeshActor* TargetDoor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform OpenTargetPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform CloseTargetPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CloseTime;
};

UCLASS()
class ORIGINALACTION_API ADoorSwitcher : public AInteractionActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay()override;

public:
	ADoorSwitcher();
	//インタラクション時呼ばれる関数
	virtual void UseInteraction()override;
	virtual void Tick(float DeltaTime)override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineLights")
	TArray<FChildMachinePackage> m_childPackage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	TArray<FDoorInformation> m_TargetInformations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	class ANavModifierVolume* m_pDoorNav;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MachineMesh")
	UStaticMesh* m_pLockedStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MachineMesh")
	UStaticMesh* m_pCausionStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MachineMesh")
	UStaticMesh* m_pUnLockedStaticMesh;

protected:
	//守護が全部壊れていたらtrue
	bool m_IsActive;
	//一度触れられてライトが光っていたらtrue
	bool m_IsUsedLight;
	//ドア開閉時間カウント
	float m_nowDoorTimer;
};
