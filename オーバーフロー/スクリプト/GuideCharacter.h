// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuideCharacter.generated.h"

//�U���N���ʒm�X�e�[�g
UENUM(BlueprintType)
enum class EGuideAnimState : uint8
{
	None		UMETA(DisplayName = "None"),
	Wait		UMETA(DisplayName = "Wait"),
	Dash		UMETA(DisplayName = "Dash"),
	Hug			UMETA(DisplayName = "Hug")
};

//�U���N���ʒm�X�e�[�g
UENUM(BlueprintType)
enum class EAutoMoveState : uint8
{
	None		UMETA(DisplayName = "None"),
	Auto		UMETA(DisplayName = "Auto"),
	Trace		UMETA(DisplayName = "Trace")
};

//�h�A�ƈړ������W���Ǘ�����\����
USTRUCT(BlueprintType)
struct FGuideCharacterLogic
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAutoMoveState MoveLogic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector TrrigerPos;
};


UCLASS()
class ORIGINALACTION_API AGuideCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuideCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//���݂̃A�j���[�V�����X�e�[�g�擾�p�֐�
	UFUNCTION(BlueprintCallable)
	EGuideAnimState GetGuideAnimState();

	void BeChild();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovePattern")
	TArray<FGuideCharacterLogic> m_Routes;
protected:
	//���݂̃A�j���[�V�����X�e�[�g
	EGuideAnimState m_AnimState;

	//���݂̈ړ�����
	EAutoMoveState m_ControlleState;

	//���݂̈ړ��ԍ�
	int m_nowRouteNum;
};
