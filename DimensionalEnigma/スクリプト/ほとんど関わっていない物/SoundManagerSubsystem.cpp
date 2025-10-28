#include "SoundManagerSubsystem.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"

void USoundManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // BGM用の AudioComponent を作成（再初期化しない）
    /*
    * if (!BGMComponent)
    {
        BGMComponent = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass());
        if (BGMComponent)
        {
            BGMComponent->bAutoActivate = false;
            BGMComponent->bIsUISound = false;
            BGMComponent->bAllowSpatialization = false;
            BGMComponent->RegisterComponentWithWorld(GetWorld()); // 必ず World に登録
            UE_LOG(LogTemp, Log, TEXT("BGMComponent initialized."));
        }
    }
    */
    
}

void USoundManagerSubsystem::Deinitialize()
{
    if (BGMComponent)
    {
        BGMComponent->Stop();
        BGMComponent->DestroyComponent();
        BGMComponent = nullptr;
    }

    Super::Deinitialize();
}

void USoundManagerSubsystem::PlaySoundEffect(USoundBase* Sound, FVector Location)
{
    if (Sound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
    }
}

void USoundManagerSubsystem::PlayBGM(const FString& AssetPath)
{
    if (AssetPath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("AssetPath is empty!"));
        return;
    }

    // アセットマネージャーを利用してサウンドをロード
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    USoundBase* Sound = Cast<USoundBase>(Streamable.LoadSynchronous(FSoftObjectPath(AssetPath)));

    if (Sound)
    {
        // 既存の AudioComponent がある場合は停止して破棄
        if (BGMComponent && BGMComponent->IsPlaying())
        {
            BGMComponent->Stop();
            UE_LOG(LogTemp, Log, TEXT("Stopped previous BGM"));
        }

        // 新しい AudioComponent を作成してサウンドを設定
        if (!BGMComponent)
        {
            BGMComponent = NewObject<UAudioComponent>(this);
            BGMComponent->bAutoActivate = false;
            BGMComponent->bIsUISound = false;
            BGMComponent->bAllowSpatialization = false;
            BGMComponent->RegisterComponentWithWorld(GetWorld()); // 必ず World に登録
        }

        BGMComponent->SetSound(Sound);
        BGMComponent->Play();
        UE_LOG(LogTemp, Log, TEXT("BGM playback started: %s"), *Sound->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load sound from path: %s"), *AssetPath);
    }
}

void USoundManagerSubsystem::StopBGM()
{
    if (BGMComponent && BGMComponent->IsPlaying())
    {
        BGMComponent->Stop();
        UE_LOG(LogTemp, Log, TEXT("Stopped BGM"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BGM is not currently playing."));
    }
}

void USoundManagerSubsystem::SetBGMVolume(float Volume)
{
    if (BGMComponent)
    {
        BGMComponent->SetVolumeMultiplier(Volume);
    }
}

void USoundManagerSubsystem::PlaySoundFromPath(const FString& AssetPath)
{
    if (AssetPath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("AssetPath is empty!"));
        return;
    }
    if (FPaths::FileExists(AssetPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid sound path: %s"), *AssetPath);
        return;
    }

    // アセットマネージャーを利用してサウンドをロード
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    USoundBase* Sound = Cast<USoundBase>(Streamable.LoadSynchronous(FSoftObjectPath(AssetPath)));

    if (Sound)
    {
        if (AudioComponent)
        {
            AudioComponent->SetSound(Sound);
            AudioComponent->Play();
        }
        else
        {
            UGameplayStatics::PlaySound2D(GetWorld(), Sound);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load sound from path: %s"), *AssetPath);
    }
}
void USoundManagerSubsystem::PlaySoundFromPath(const FString& AssetPath,float _Volume)
{
    if (AssetPath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("AssetPath is empty!"));
        return;
    }
    if (FPaths::FileExists(AssetPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid sound path: %s"), *AssetPath);
        return;
    }

    // アセットマネージャーを利用してサウンドをロード
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    USoundBase* Sound = Cast<USoundBase>(Streamable.LoadSynchronous(FSoftObjectPath(AssetPath)));

    if (Sound)
    {
        if (AudioComponent)
        {
            AudioComponent->SetSound(Sound);
            AudioComponent->SetVolumeMultiplier(_Volume);  // 音量調整
            AudioComponent->Play();
        }
        else
        {
            UGameplayStatics::PlaySound2D(GetWorld(), Sound, _Volume);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load sound from path: %s"), *AssetPath);
    }
}