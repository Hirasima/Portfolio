// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager_V2.h"
#include "EnemyBase_V2.h"


//実際のEnemyのDestroy実行処理
void UEnemyManager_V2::ExecuteDestroy() {
    //Destroy対象のEnemy群を順次Destroy

    //要素番号ずれを防ぐため一時配列にコピー
    TArray<AEnemyBase_V2*> DestroyTargets=m_pDestroyTargetEnemies;
    //順次破棄
    for (AEnemyBase_V2* target : DestroyTargets) {
        
        //ヌルチェック
        if (!target) {
            return;
        }

        //自身の管理配列からの解放ブロック/////////////////////////////////////////////////////

        //自身の管理対象から外す
        RemoveManagementEnemy(target);
        //自身のDestroy予定者から外す
        RemoveDestroyTargetEnemy(target);

        //Destroy実行ブロック///////////////////////////////////////////////////////////////
        //Destroy処理+Destroy直前にトリガーする関数
        target->ExecuteDestroy();
    }
}



//自身の更新関数(Enemy配列の更新をまとめて呼ぶ)
void UEnemyManager_V2::GameTick(float _deltaTime) {
    //管理対象のEnemyの更新処理を順に呼んでいく
    for (AEnemyBase_V2* enemy : m_pEnemies) {
        //対象ヌルチェック
        if (!enemy) {
            continue;
        }

        //更新関数を呼ぶ（細かい更新順はEnemy側で決定）
        enemy->GameTick(_deltaTime);
    }
}


//Enemyの次のフレームの頭にDestroyする状態をtrueにするかチェック
void UEnemyManager_V2::CheckDestroy() {
    //管理対象のEnemyの次フレームにDestroyするかの確認を順次する
    for (AEnemyBase_V2* enemy : m_pEnemies) {
        //対象ヌルチェック
        if (!enemy) {
            continue;
        }

        //次フレームにDestroyするかの確認をする
        enemy->CheckDestroy();
    }
}



//管理対象の追加・削除///////////////////////////////////////////////////////////////////////////////////////////////

//自身の管理下にEnemyを追加関数する関数
void UEnemyManager_V2::AddManagementEnemy(class AEnemyBase_V2* _target) {
    //ヌルチェック
    if (!_target) {
        return;
    }
    //すでに管理下においているなら無視
    if (m_pEnemies.Contains(_target)) {
       return;
    }

    //管理対象に追加
    m_pEnemies.Add(_target);

    //Destroyターゲットに追加するコールバックの登録
    FRegisterToDestroyer Delegate;
    Delegate.BindUObject(this, &UEnemyManager_V2::AddDestroyTargetEnemy);
    _target->SetRegisterToDestroyerCallBack(Delegate);
}

//自身の管理下からEnemyを開放する関数
void UEnemyManager_V2::RemoveManagementEnemy(class AEnemyBase_V2* _target) {
    //ヌルチェック
    if (!_target) {
        return;
    }
    //すでに管理下に置いてないなら無視
    if (m_pEnemies.Contains(_target)) {
        //管理対象から外す
        m_pEnemies.Remove(_target);
    }
}

//自身のDestroy予定対象にEnemyを追加する関数
void UEnemyManager_V2::AddDestroyTargetEnemy(class AEnemyBase_V2* _target) {
    //ヌルチェック
    if (!_target) {
        return;
    }
    //すでに管理下においているなら無視
    if (m_pDestroyTargetEnemies.Contains(_target)) {
        return;
    }

    //管理対象に追加
    m_pDestroyTargetEnemies.Add(_target);
}


//自身のDestroy予定対象からEnemyを開放する関数(EnemyにDelegateとして伝達)
void UEnemyManager_V2::RemoveDestroyTargetEnemy(class AEnemyBase_V2* _target) {
    //ヌルチェック
    if (!_target) {
        return;
    }
    //すでに管理下に置いてないなら無視
    if (m_pDestroyTargetEnemies.Contains(_target)) {
        //管理対象から外す
        m_pDestroyTargetEnemies.Remove(_target);
    }
}


