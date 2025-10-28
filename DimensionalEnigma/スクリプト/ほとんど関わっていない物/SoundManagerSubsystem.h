#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManagerSubsystem.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class MYPROJECT_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 効果音の再生
    UFUNCTION(BlueprintCallable, Category = "Sound")
    void PlaySoundEffect(USoundBase* Sound, FVector Location);

    // BGMの再生
    //UFUNCTION(BlueprintCallable, Category = "Sound")
    //void PlayBGM(USoundBase* BGM);
    UFUNCTION(BlueprintCallable, Category = "Sound")
    void PlayBGM(const FString& AssetPath);

    // BGMの停止
    UFUNCTION(BlueprintCallable, Category = "Sound")
    void StopBGM();

    // ボリュームの設定
    UFUNCTION(BlueprintCallable, Category = "Sound")
    void SetBGMVolume(float Volume);

    void PlaySoundFromPath(const FString& AssetPath);
    void PlaySoundFromPath(const FString& AssetPath, float _Volume);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    // BGM用のオーディオコンポーネント
    UPROPERTY()
    UAudioComponent* BGMComponent;

    UPROPERTY()
    UAudioComponent* AudioComponent;
};
