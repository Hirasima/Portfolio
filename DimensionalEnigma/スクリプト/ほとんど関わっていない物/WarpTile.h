// WarpTile.h
// 23cu0228 hanawa

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarpTile.generated.h"

UENUM()
enum class EWarpType : uint8
{
	UP = 0,
	DOWN = 1,
};

UCLASS()
class AWarpTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarpTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void WarpPlayer(AActor* Player);	// プレイヤーを錯視移動(ワープ)させる処理

	bool GetIsWarp() const;					// 移動(ワープ)可能かどうか返す処理

	bool GetIsWarped() const;

	FVector GetDownWarpLocation();

private:
	UPROPERTY(EditAnywhere, Category = "Warp")
	EWarpType m_WarpType;

	UPROPERTY(EditAnywhere, Category = "Warp")
	int m_WarpDirectionNums[5];		// 移動(ワープ)可能な画角番号

	UPROPERTY(EditANywhere, Category = "Warp")
	AActor* m_pWarpTiles[5];		// 移動(ワープ)先のタイルの情報

	FVector m_WarpDistination;		// 移動(ワープ)先のタイルの座標

	UPROPERTY(EditAnywhere, Category = "CameraDirection")
	AActor* m_pCameraActor;			// 画角番号を得るためのカメラ

	bool m_IsWarpable;				// 移動可能かどうか

	bool m_IsWarped;				// 下りのワープかどうか

private:
	UWorld* world;
	UEnigmaGameInstance* gameinstance;
	UStageSubSystem* gamemanager;
};
