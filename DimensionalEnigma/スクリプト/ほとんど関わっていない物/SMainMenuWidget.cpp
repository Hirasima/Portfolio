#include "SMainMenuWidget.h"
#include "SlateOptMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"
#include "Widgets/SWidget.h"  // 必要なインクルード
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Framework/Application/SlateApplication.h"
#include "InputCoreTypes.h"  // EKeys を使うために必要

#include "SoundManagerSubsystem.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenuWidget::Construct(const FArguments& InArgs)
{
    // フォーカスを設定
    FSlateApplication::Get().SetKeyboardFocus(AsShared());

    // 背景画像の設定
    BackgroundBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/GameTitle.GameTitle")));

    // フォント設定
    FSlateFontInfo FontInfo = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 32 * 1);

    MenuOptions = {
        SNew(STextBlock)
        .Text(FText::FromString("Tutorial"))
        .Font(FontInfo)
        .ShadowOffset(FVector2D(2.0f, 2.0f))
        .ShadowColorAndOpacity(FLinearColor::Black),

        SNew(STextBlock)
        .Text(FText::FromString("Stage Select"))
        .Font(FontInfo)
        .ShadowOffset(FVector2D(2.0f, 2.0f))
        .ShadowColorAndOpacity(FLinearColor::Black),

        SNew(STextBlock)
        .Text(FText::FromString("End Game"))
        .Font(FontInfo)
        .ShadowOffset(FVector2D(2.0f, 2.0f))
        .ShadowColorAndOpacity(FLinearColor::Black)
    };

    ChildSlot
        [
            SNew(SOverlay)
                + SOverlay::Slot()
                [
                    // 背景画像
                    SNew(SImage).Image(&BackgroundBrush)
                ]
                + SOverlay::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Top) // メニューの基準を上寄せに変更
                .Padding(FMargin(0, 650, 0, 0)) // 全体を下に300ピクセルずらす
                [
                    // メニュー
                    SNew(SVerticalBox)
                        + SVerticalBox::Slot()
                        .Padding(10)
                        .AutoHeight()
                        [
                            MenuOptions[0].ToSharedRef()
                        ]
                        + SVerticalBox::Slot()
                        .Padding(10)
                        .AutoHeight()
                        [
                            MenuOptions[1].ToSharedRef()
                        ]
                        + SVerticalBox::Slot()
                        .Padding(10)
                        .AutoHeight()
                        [
                            MenuOptions[2].ToSharedRef()
                        ]
                ]
        ];

    HighlightOption(0);

    // FSlateApplicationを使ってGameInstanceを取得する
    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        UE_LOG(LogTemp, Log, TEXT("GameInstance is valid!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance is not valid."));
    }
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SMainMenuWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    const FKey Key = InKeyEvent.GetKey();

    if (Key == EKeys::W) // 上移動
    {
        HighlightOption((SelectedIndex - 1 + MenuOptions.Num()) % MenuOptions.Num());
        return FReply::Handled();
    }
    else if (Key == EKeys::S) // 下移動
    {
        HighlightOption((SelectedIndex + 1) % MenuOptions.Num());
        return FReply::Handled();
    }
    else if (Key == EKeys::Enter) // 決定
    {
        HandleSelection();
        return FReply::Handled();
    }

    return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

void SMainMenuWidget::HighlightOption(int32 NewIndex)
{
    // 現在の選択項目の色をリセット
    MenuOptions[SelectedIndex]->SetColorAndOpacity(FLinearColor::White);

    // 新しい選択項目を強調表示
    SelectedIndex = NewIndex;
    MenuOptions[SelectedIndex]->SetColorAndOpacity(FLinearColor::Yellow);
}

void SMainMenuWidget::HandleSelection()
{
    switch (SelectedIndex)
    {
    case 0: // Start Game
        UE_LOG(LogTemp, Log, TEXT("New Game Selected"));
        NewGame();
        break;

    case 1: // Game Setting
        UE_LOG(LogTemp, Log, TEXT("Start Game Selected"));
        StartGame();
        break;

    case 2: // End Game
        UE_LOG(LogTemp, Log, TEXT("End Game Selected"));
        EndGame();
        break;

    default:
        break;
    }
}

void SMainMenuWidget::StartGame()
{
    if (UWorld* World = GetCurrentWorld())
    {

        // レベルを遷移
        FString LevelName = TEXT("WorldSelect"); // 適切なレベル名を指定
        UGameplayStatics::OpenLevel(World, FName(*LevelName));

        // レベル移行後にタイトル画面を削除
        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->RemoveViewportWidgetContent(AsShared());
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Enter key is pressed"));
}
void SMainMenuWidget::NewGame()
{
    if (UWorld* World = GetCurrentWorld())
    {
        // レベルを遷移
        FString LevelName = TEXT("stage_1_0"); // 適切なレベル名を指定
        UGameplayStatics::OpenLevel(World, FName(*LevelName));

        // レベル移行後にタイトル画面を削除
        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->RemoveViewportWidgetContent(AsShared());
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Enter key is pressed"));
}
void SMainMenuWidget::EndGame()
{
    if (GIsEditor)
    {
        UE_LOG(LogTemp, Warning, TEXT("EndGame called in Editor mode. Skipping request to exit."));

        UWorld* World = GetCurrentWorld();
        if (!World)
        {
            UE_LOG(LogTemp, Error, TEXT("World is null!"));
            return;
        }

        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
        {
            // ゲームを終了（PIE モードの場合はセッション終了）
            UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        }

        return;
    }
    if (!GEngine)
    {
        UE_LOG(LogTemp, Warning, TEXT("GEngine is null!"));
        return;
    }
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->RemoveAllViewportWidgets();
    }

    FGenericPlatformMisc::RequestExit(false);
}

void SMainMenuWidget::GameSetting()
{
    // 設定画面のロジック
}
void SMainMenuWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    CheckForStickMovement();//左スティックの入力を監視

    //更新する前の入力値を記録する
    bool IsLStickUpSignal_old = IsLStickUpSignal;
    bool IsLStickDownSignal_old = IsLStickDownSignal;
    // プレイヤーコントローラーを取得
    APlayerController* PlayerController = Cast<APlayerController>(GetCurrentWorld()->GetFirstPlayerController());
    if (PlayerController)
    {
        float StickX, StickY;
        // 左スティックの状態を取得
        PlayerController->GetInputAnalogStickState(EControllerAnalogStick::CAS_LeftStick, StickX, StickY);

        // Y軸（上下）の値をログに出力
        if (StickY != 0.0f)
        {
            UE_LOG(LogTemp, Log, TEXT("Left Stick Y: %f"), StickY);
        }
        if (StickY >= 0.5f)
        {
            IsLStickUpSignal = true;
        }
        else
        {
            IsLStickUpSignal = false;
        }
        if (StickY <= -0.5f)
        {
            IsLStickDownSignal = true;
        }
        else
        {
            IsLStickDownSignal = false;
        }
    }
    else
    {
        IsLStickUpSignal = false;
        IsLStickDownSignal = false;
    }

    if (GetCurrentWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W) ||
        IsStickMoveRecently == 1 ||
        bIsStickMovedUpRecently == true ||
        (IsLStickUpSignal == true && IsLStickUpSignal_old == false)||
        PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Up))
    {
        HighlightOption((SelectedIndex - 1 + MenuOptions.Num()) % MenuOptions.Num());
    }
    if (GetCurrentWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S) ||
        IsStickMoveRecently == -1 ||
        (IsLStickDownSignal == true && IsLStickDownSignal_old == false)||
        PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Down))
    {
        HighlightOption((SelectedIndex + 1 + MenuOptions.Num()) % MenuOptions.Num());
    }
    if (GetCurrentWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter) ||
        GetCurrentWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
    {
        HandleSelection();
    }

    if (GetCurrentWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Q))
    {
        //BGMを再生
        USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
        if (SoundManager)
        {
            //仮でBGM再生
            //SoundManager->PlayBGM(TEXT("/Game/Sound/TestBGM.TestBGM"));

            UE_LOG(LogTemp, Warning, TEXT("PlayBGM"))
        }
    }
    if (GetCurrentWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
    {
        //BGMを再生
        USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
        if (SoundManager)
        {
            //仮でBGM再生
            //SoundManager->StopBGM();

            UE_LOG(LogTemp, Warning, TEXT("StopBGM"))
        }
    }
}
UWorld* SMainMenuWidget::GetCurrentWorld() const
{
    if (GEngine && GEngine->GameViewport)
    {
        return GEngine->GameViewport->GetWorld();
    }
    return nullptr;
}

/*+// Aボタン(海外での〇)でアクションを起こす
UWorld* SMainMenuWidget::PressedAction()
{

    //UKismetSystemLibrary::PrintString(this, TEXT("Action Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}
*/


void  SMainMenuWidget::CheckForStickMovement()
{
    // ゲームパッドの左スティックY軸の入力値を取得
    float CurrentStickValue = GetCurrentWorld()->GetFirstPlayerController()->GetInputAxisValue("Gamepad_LeftY");

    // 左スティックが上方向（正の方向）に倒された瞬間を検出
    if (CurrentStickValue > Threshold && LastStickValue <= Threshold)
    {
        bIsStickMovedUpRecently = true;
        IsStickMoveRecently = 1;
    }
    else
        if (CurrentStickValue < -Threshold && LastStickValue >= -Threshold)
        {
            bIsStickMovedUpRecently = false;
            IsStickMoveRecently = -1;
        }
    else
    {
        bIsStickMovedUpRecently = false;
        IsStickMoveRecently = 0;
    }

    // 前回のスティックの状態を保存
    LastStickValue = CurrentStickValue;
}