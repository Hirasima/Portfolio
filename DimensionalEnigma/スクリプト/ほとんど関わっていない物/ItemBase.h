#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class MYPROJECT_API AItemBase : public APawn
{
    GENERATED_BODY()

public:
    AItemBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    int GetItemID();

private:

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MovementY = 1.2f;//縦の移動速度
    UPROPERTY(EditAnywhere, Category = "Movement")
    float GravityScale = 4.0f;//重力

    UPROPERTY(EditAnywhere, Category = "Movement")
    float RotateAmount_Idol = 50.0f;//待機時の回転量
    UPROPERTY(EditAnywhere, Category = "Movement")
    float RotateAmount_Out = 800.0f;//消滅時の回転量

    UPROPERTY(EditAnywhere, Category = "ItemID")
    int ItemID = 0;//アイテムのID　要素数が10個なので0～9の間で設定してください

    UPROPERTY(EditAnywhere, Category = "Collision")
    TSubclassOf<AActor> TargetActorClass;

    // ファイルパス用の FString 変数
    UPROPERTY(EditAnywhere,  Category = "SoundEffect")
    FString SE_Path = TEXT("/Game/Sound/GetItemSE.GetItemSE");

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* StaticMeshComponent;

    int IsGetting = false;
    float Counter = 0.0f;
    float RotateAmount = 50;//回転量

    float  m_DeleteHeight;//削除する高さ

};