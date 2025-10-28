// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalTile.h"
#include "WorldSelect.h"
#include "StageSubSystem.h"
#include "Kismet/GameplayStatics.h"

#include "Player3D.h"//大竹追加　プレイヤーにゴールしたことを伝えるため

// Sets default values
AGoalTile::AGoalTile()
	: m_bGoal(false)
	, m_TransitionMode(ETransitionMode::NextLevel)
	, m_LevelName()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoalTile::BeginPlay()
{
	Super::BeginPlay();
	
	// Tag を設定
	Tags.Add(FName(TEXT("GoalTile")));
}

// Called every frame
void AGoalTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetIsGoal(DeltaTime);

	DebugNextLevel();
}

void AGoalTile::SetIsGoal(float DeltaTime)
{
	if (!m_bGoal)
	{
		FVector Start = GetActorLocation();
		FVector End = GetActorLocation() + FVector(0.f, 0.f, 10.f);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		FHitResult OutHit;
		bool isHit = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, FCollisionObjectQueryParams::AllObjects, CollisionParams);

		if (isHit)
		{
			AActor* HitActor = OutHit.GetActor();

			if (HitActor->ActorHasTag("Player"))
			{
				m_bGoal = true;

				UStageSubSystem* stage = GetWorld()->GetSubsystem<UStageSubSystem>();

				if (stage)
				{
					stage->OnGoal(m_LevelName);
				}
				
				//--------------------------------------大竹追加
				//プレイヤーにゴールしたことを伝える
				//型変換して取得
				if (HitActor->IsA(APlayer3D::StaticClass()))//ヌルチェック
				{
					APlayer3D* pPlayer3D = Cast<APlayer3D>(HitActor);
					pPlayer3D->SetGoalFlag(true);
				}

				//--------------------------------------

				/*switch (m_TransitionMode)
				{
				case ETransitionMode::NextLevel:

					LoadNextLevel();

					UE_LOG(LogTemp, Warning, TEXT("Next Level"));
					break;
				case ETransitionMode::ReturnSelect:
					

					LoadNextLevel();
					UE_LOG(LogTemp, Warning, TEXT("Return Select"));
					break;
				}*/
			}
		}
	}
}

void AGoalTile::LoadNextLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), m_LevelName);
}

// デバッグ用ネクストレベル
void AGoalTile::DebugNextLevel()
{
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
	{
		UGameplayStatics::OpenLevel(GetWorld(), m_LevelName);
	}
}