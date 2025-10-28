// ItemSubsystem.cpp
#include "ItemSubsystem.h"

void UItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // 初期化コード
    // 
    
    // ItemFlagの初期化（サイズや初期値の設定など）
    ItemFlag.SetNum(10); // 例えば、10個のアイテムフラグを持つ
    for (bool& Flag : ItemFlag)
    {
        Flag = false; // 初期状態ではすべてfalse
    }
}
bool UItemSubsystem::GetItemFlag(int _Index)
{
    bool result = false;
    //要素番号が配列内かチェック
    if (_Index >= 0 && _Index < 10)
    {
        result = ItemFlag[_Index];
    }
    return result;
}
void UItemSubsystem::SetItemFlag(int _Index, bool _Flag)
{
    //要素番号が配列内かチェック
    if (_Index >= 0 && _Index < 10)
    {
        ItemFlag[_Index]= _Flag;
    }
}