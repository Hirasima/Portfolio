#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemSubsystem.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	

	int GetCount = 0;

	bool GetItemFlag(int _Index);
	void SetItemFlag(int _Index ,bool _Flag);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	TArray<bool> ItemFlag;
};

