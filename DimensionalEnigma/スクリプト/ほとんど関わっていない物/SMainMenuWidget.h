#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWidget.h"  // 必要なウィジェットのインクルード
#include "Widgets/Images/SImage.h" // 画像関連のウィジェット

class MYPROJECT_API SMainMenuWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SMainMenuWidget) {}
        SLATE_ARGUMENT(UWorld*, WorldContext)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    UWorld* GetCurrentWorld() const;

    //TSharedPtr<FSlateBrush> BackgroundBrush;
    FSlateBrush BackgroundBrush;
    void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
protected:
    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
    //virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
    void StartGame();
    void NewGame();
    void EndGame();
    void GameSetting();

    void HighlightOption(int32 NewIndex);
    void HandleSelection();

    int32 SelectedIndex = 0;
    TArray<TSharedPtr<STextBlock>> MenuOptions;

    UWorld* WorldContext;

    //UWorld* PressedAction();

    //左スティックの入力を管理する変数達
    bool IsLStickUpSignal = false;
    bool IsLStickDownSignal = false;
    int IsStickMoveRecently = 0;//0:Neutral 1:Up　-1:Down
    bool bIsStickMovedUpRecently = false;
    float Threshold = 0.5f;  // この値でスティックの入力強度の閾値を決める
    float LastStickValue = 0.0f;

    void CheckForStickMovement();//倒した瞬間を監視する関数

private:
    UGameInstance* GetGameInstance() const
    {
        // FSlateApplication経由でGameInstanceを取得
        if (GEngine && GEngine->GameViewport)
        {
            UGameInstance* GameInstance = GEngine->GameViewport->GetWorld()->GetGameInstance();
            return GameInstance;
        }
        return nullptr;
    }
};