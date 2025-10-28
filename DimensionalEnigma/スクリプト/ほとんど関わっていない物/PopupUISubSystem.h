

#pragma once

#include "CoreMinimal.h"
//#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "PopupUISubSystem.generated.h"

UCLASS()
class MYPROJECT_API UPopupUISubSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // ポップアップUIを表示
    UFUNCTION(BlueprintCallable, Category = "PopupUI")
    void ShowPopupUI(TSubclassOf<UUserWidget> WidgetClass);
public:
    // ポップアップUIを表示（ファイルパス指定）
    //UFUNCTION(BlueprintCallable, Category = "PopupUI")
    void ShowPopupUI(const FString& WidgetBlueprintPath);
    // ポップアップUIを非表示
    UFUNCTION(BlueprintCallable, Category = "PopupUI")
    void HidePopupUI();

private:
    // 表示中の UI ウィジェット
    UPROPERTY()
    UUserWidget* ActiveWidget = nullptr;

    TSubclassOf<UUserWidget> m_PopupWidget;
};