#include "PopupUISubSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UPopupUISubSystem::ShowPopupUI(TSubclassOf<UUserWidget> WidgetClass)
{
    if (!WidgetClass) return;

    // すでに UI が表示されていたら削除
    if (ActiveWidget)
    {
        ActiveWidget->RemoveFromParent();
        ActiveWidget = nullptr;
    }

    // ワールドを取得
    UWorld* World = GetWorld();
    if (!World) return;

    // プレイヤーの HUD を取得
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!PlayerController) return;

    // ウィジェットを作成
    ActiveWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
    if (ActiveWidget)
    {
        ActiveWidget->AddToViewport();
    }
}
void UPopupUISubSystem::ShowPopupUI(const FString& WidgetBlueprintPath)
{
	// すでに表示されている場合は削除
	if (ActiveWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	// ログ出力でデバッグ
	UE_LOG(LogTemp, Log, TEXT("ShowPopupUI called with path: %s"), *WidgetBlueprintPath);

	// 正しい型で TSoftClassPtr を生成
	TSoftClassPtr<UUserWidget> SoftWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(WidgetBlueprintPath));

	// LoadSynchronous() を正しく呼び出す
	TSubclassOf<UUserWidget> WidgetClass = SoftWidgetClass.LoadSynchronous();

	// ロードチェック
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Widget: %s"), *WidgetBlueprintPath);
		return;
	}

	// GetWorld() の安全な取得
	UWorld* World = GetGameInstance()->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get World"));
		return;
	}

	// プレイヤーコントローラー取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController"));
		return;
	}

	// ウィジェット生成
	ActiveWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
	if (ActiveWidget)
	{
		ActiveWidget->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("Widget added to viewport successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create widget"));
	}
}

void UPopupUISubSystem::HidePopupUI()
{
    if (ActiveWidget)
    {
        ActiveWidget->RemoveFromParent();
        ActiveWidget = nullptr;
    }
}

