
#include "TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SMainMenuWidget.h"
#include "InGameHUD.h"
#include "InGamePlayerController.h"

ATitleGameMode::ATitleGameMode()
{
    DefaultPawnClass = nullptr; // 自動生成されたPawnを無効化
    HUDClass = AInGameHUD::StaticClass();
    PlayerControllerClass = AInGamePlayerController::StaticClass();
}
void ATitleGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 現在のレベル名を取得
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);

    // タイトル画面のみ表示
    if (CurrentLevelName == "GameTitle")
    {
        if (!MainMenuWidget.IsValid())
        {
            MainMenuWidget = SNew(SMainMenuWidget);
            if (GEngine && GEngine->GameViewport)
            {
                GEngine->GameViewport->AddViewportWidgetContent(MainMenuWidget.ToSharedRef());
            }
        }
    }
    else
    {
        RemoveTitleScreen();
    }
}

void ATitleGameMode::OnLevelChanged(UWorld* LoadedWorld)
{
    if (LoadedWorld && LoadedWorld->GetName() != "GameTitle")
    {
        RemoveTitleScreen();
    }
}

void ATitleGameMode::RemoveTitleScreen()
{
    if (MainMenuWidget.IsValid() && GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->RemoveViewportWidgetContent(MainMenuWidget.ToSharedRef());
        MainMenuWidget.Reset(); // メモリ解放
    }
}