// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

//�O���錾
class AFighterCharacter;
class UNiagaraSystem;
class AWire_HUD;

//�A�C�e���̎�ޗ񋓌^
UENUM(BlueprintType)
enum class EItemKinds : uint8
{
	HpRecovery	UMETA(DisplayName = "HpRecovery")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORIGINALACTION_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �I���W�i�����\�b�h //////////////////////////////////////////

	//�A�C�e�����g�p����ۂɌĂ΂��֐�
	void UseItem(EItemKinds _itemKinds);

	//HP�񕜏���
	void RecoveryHp();

	//HUD�ɓ`�B����
	void CommunicateToHUD();

	// �v���p�e�B //////////////////////////////////////////////////

public:

	//HP�񕜗�
	UPROPERTY(EditAnywhere)
	int HPRecoveryAmount;

	//HP�񕜃A�C�e���ő吔
	UPROPERTY(EditAnywhere)
	int HPRecoveryItemNumMax;

	//HP�񕜃A�C�e���Ďg�p����
	UPROPERTY(EditAnywhere)
	float HpRecoveryItemRecastTime;

	//HUD
	UPROPERTY()
	AWire_HUD* HUD;

	//�e�t�@�C�^�[�L�����N�^�[
	UPROPERTY()
	AFighterCharacter* m_pParentFighter;
private:

	//HP�񕜃A�C�e����
	UPROPERTY(VisibleAnywhere)
	int m_hpRecoveryItemNum;

	//HP�񕜃A�C�e���Ďg�p�^�C�}�[
	UPROPERTY(VisibleAnywhere)
	float m_hpRecoveryItemRecastTimer;

	//---------------------------
	// �G�t�F�N�g�֌W
	//---------------------------
public:

	//HP�񕜎��̃G�t�F�N�g
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HPRecoveryEffect;

};
