// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PopupArea.generated.h"

UCLASS()
class APopupArea : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APopupArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

    bool bIsOverlapping = false;

private:

    UPROPERTY(EditAnywhere, Category = "Collision")
    TSubclassOf<AActor> TargetActorClass;

    // ファイルパス用の FString 変数
    UPROPERTY(EditAnywhere, Category = "UI_Path")
    FString UI_Path = TEXT("/Game/UI/World1.World1");

    UPROPERTY(EditAnywhere, Category = "UI_Path")
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* StaticMeshComponent;

};
