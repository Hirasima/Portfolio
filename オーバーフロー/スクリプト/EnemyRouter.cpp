// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRouter.h"
#include "Enemy_Base.h"
#include "Components/BoxComponent.h"
#include"Engine/TriggerBox.h"

// Sets default values
AEnemyRouter::AEnemyRouter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyRouter::BeginPlay()
{
	Super::BeginPlay();

	//全てのボックスに誘導関数を伝達
	for (int i = 0;i < m_pRouteCoollisions.Num();i++) {
		m_pRouteCoollisions[i]->OnActorBeginOverlap.AddDynamic(this, &AEnemyRouter::SendNextPos);
		TArray<AActor*> OverlappingActors;
		m_pRouteCoollisions[i]->GetOverlappingActors(OverlappingActors, AEnemy_Base::StaticClass());
		for (AActor* Actor : OverlappingActors) {
			SendNextPos(m_pRouteCoollisions[i], Actor);
		}
	}
	
}

// Called every frame
void AEnemyRouter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyRouter::SendNextPos(AActor* _a, AActor* _b) {
	//接触したのがエネミーじゃないなら無視
	if (!_b->IsA(AEnemy_Base::StaticClass())) {
		return;
	}
	AEnemy_Base* ene = Cast<AEnemy_Base>(_b);

	//自分が何番目かを調べる
	int myIndex = m_pRouteCoollisions.IndexOfByKey(_a);
	//次のインデックスか0番目のインデックスの場所をエネミーに伝達
	if (m_pRouteCoollisions.IsValidIndex(myIndex + 1)) {
		ene->SetWaitPosition(m_pRouteCoollisions[myIndex+1]->GetActorTransform());
	}
	else {
		ene->SetWaitPosition(m_pRouteCoollisions[0]->GetActorTransform());
	}
}

