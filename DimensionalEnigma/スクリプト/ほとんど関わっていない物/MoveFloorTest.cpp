// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveFloorTest.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SoundManagerSubsystem.h"

namespace MoveFloorTest::Private
{
	constexpr float BOX_LENGTH = 10.0f;
}

// Sets default values
AMoveFloorTest::AMoveFloorTest()
	: m_MoveBlock(nullptr)
	, m_MovingType(EMovingType::Movement)
	, m_SwitchType(ESwitchType::ManyTimes)
	, m_MoveDistances()
	, m_RotationDistance(FRotator::ZeroRotator)
	, m_EasingType(EEasingType::Linear)
	, m_MoveEndTime(0.f)
	, m_MoveDistance(FVector::ZeroVector)
	, m_StartOtherObjLocation(FVector::ZeroVector)
	, m_StartOtherObjRotation(FRotator::ZeroRotator)
	, m_Default3DRotation(FRotator(-35.26f, -45.0f, 0.0f))
	, m_Default2DRotation(FRotator(-90.f, 0.f, 0.f))
	, m_MoveCurrentTime(0.f)
	, m_SwitchCoolTime(0.f)
	, m_Alpha(0.f)
	, m_MoveDistanceIdx(0)
	, m_isSwitch(false)
	, m_isMoving(false)
	, m_isMoveReturn(false)
	, m_isOnce(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 足場のスタティックメッシュの設定
	BoxComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	BoxComponent->SetMobility(EComponentMobility::Movable);
	RootComponent = BoxComponent;
	RootComponent->SetMobility(EComponentMobility::Movable);

	// スイッチのスタティックメッシュの設定
	SwitchComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	SwitchComponent->SetMobility(EComponentMobility::Movable);
	SwitchComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMoveFloorTest::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName(TEXT("MoveFloorTest")));

	if (m_MoveDistances.IsValidIndex(m_MoveDistanceIdx))
	{
		m_MoveDistance = m_MoveDistances[m_MoveDistanceIdx];
	}

	// GameManagerを取得
	world = GetWorld();
	if (world) {
		gameinstance = Cast<UEnigmaGameInstance>(world->GetGameInstance());
		if (gameinstance) {
			gamemanager = world->GetSubsystem<UStageSubSystem>();
		}
	}

	//SEを再生(無音)空撃ちさせる
	USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	if (SoundManager)
	{
		// パスで音源を再生
		SoundManager->PlaySoundFromPath(SE_Path, 0.001f);

		UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
	}
}

// Called every frame
void AMoveFloorTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCheckPlayer();

	UpdateMoveBlock(DeltaTime);
}

void AMoveFloorTest::SetMaterial()
{
	UMaterialInterface* Material = SwitchComponent->GetMaterial(0);
	if (Material)
	{
		UMaterialInstanceDynamic* PressedColor = UMaterialInstanceDynamic::Create(Material, this);
		if (PressedColor)
		{
			PressedColor->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0.6f, 0.6f, 0.6f));
			SwitchComponent->SetMaterial(0, PressedColor);
		}
	}
}

void AMoveFloorTest::UpdateCheckPlayer()
{
	// レイを飛ばす
	const FVector Start = GetActorLocation();
	const FVector End = Start + FVector(0.f, 0.f, 6.f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult OutHit;
	bool isHit = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, FCollisionObjectQueryParams::AllObjects, CollisionParams);
	if (isHit)
	{
		AActor* HitActor = OutHit.GetActor();

		// プレイヤーが接触した時、指定ブロックを動かす
		if (HitActor->ActorHasTag("Player"))
		{
			if (!m_isSwitch)
			{
				m_isSwitch = true;
				m_isMoving = true;

				m_MoveCurrentTime = 0.f;

				// 初期座標の設定
				if (m_MoveBlock != nullptr)
				{
					m_StartOtherObjLocation = m_MoveBlock->GetActorLocation();

					m_StartOtherObjRotation = m_MoveBlock->GetActorRotation();

					m_StartOtherObjScale = m_MoveBlock->GetActorScale();
				}

				if (m_MainCamera != nullptr)
				{
					switch (m_MovingType)
					{
					case EMovingType::TwoD:
						m_Default3DRotation = m_MainCamera->GetActorRotation();
						break;

					case EMovingType::ThreeD:
						m_Default2DRotation = m_MainCamera->GetActorRotation();
						break;

					default:
						break;
					}
				}

				USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
				if (SoundManager)
				{
					// パスで音源を再生
					SoundManager->PlaySoundFromPath(SE_Path);
					UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
				}
			}
			else
			{
				// UE_LOG(LogTemp, Log, TEXT("Not Pressed"));
			}
		}
	}
}

void AMoveFloorTest::UpdateMoveBlock(float DeltaTime)
{
	if (m_isOnce) { return; }

	if (m_isMoving)
	{
		// スイッチを押されている見た目に変更
		if (SwitchComponent)
		{
			SwitchComponent->SetRelativeLocation(FVector(0.f, 0.f, 3.5f));

			// スイッチが"Once"(一度のみ使用)ならマテリアルを灰色に変更
			if (m_SwitchType == ESwitchType::Once)
			{
				SetMaterial();
			}
		}

		if (m_MoveBlock != nullptr)
		{
			// 始点と終点の座標を設定
			FVector TargetLocation = m_StartOtherObjLocation;
			if (!m_isMoveReturn)
			{
				TargetLocation = TargetLocation + (m_MoveDistance * MoveFloorTest::Private::BOX_LENGTH);
			}
			else
			{
				TargetLocation = TargetLocation + (m_MoveDistance * -MoveFloorTest::Private::BOX_LENGTH);
			}

			FRotator TargetRotation = m_StartOtherObjRotation + (m_RotationDistance);
			
			FVector TargetScale = m_StartOtherObjScale + (m_ScaleSize);
			// UE_LOG(LogTemp, Log, TEXT("StartRotation : %s"), *StartRotation.ToString());

			float Alpha = FMath::Clamp(m_MoveCurrentTime / m_MoveEndTime, 0.0f, 1.0f);

			switch (m_MovingType)
			{
				// 移動処理
			case EMovingType::Movement:
				m_ObjLocation = SetMoveSpeed(m_EasingType, m_StartOtherObjLocation, TargetLocation, Alpha, m_MoveEndTime);
				
				m_MoveBlock->SetActorLocation(m_ObjLocation);

				break;

				// 回転処理
			case EMovingType::Rotation:
				m_ObjRotation = SetRotationSpeed(m_EasingType, m_StartOtherObjRotation, TargetRotation, Alpha, m_MoveEndTime);

				m_MoveBlock->SetActorRotation(m_ObjRotation);
					
				break;

				// 拡縮処理
			case EMovingType::Scale:
				m_ObjScale = SetScaleSpeed(m_EasingType, m_StartOtherObjScale, TargetScale, Alpha, m_MoveEndTime);
				
				m_MoveBlock->SetActorScale3D(m_ObjScale);

				break;

			case EMovingType::ThreeD:
				gamemanager->SetIsDimensionChange(true);
				gamemanager->SetDimensionType(EDimensionType::ThreeD);
				m_CamRotationSpeed = SetRotationSpeed(m_EasingType, m_Default2DRotation, m_Default3DRotation, Alpha, m_MoveEndTime);

				if (m_MainCamera)
				{
					m_MainCamera->SetActorRotation(m_CamRotationSpeed);
				}
				break;

			case EMovingType::TwoD:
				gamemanager->SetIsDimensionChange(true);
				gamemanager->SetDimensionType(EDimensionType::TwoD);
				m_CamRotationSpeed = SetRotationSpeed(m_EasingType, m_Default3DRotation, m_Default2DRotation, Alpha, m_MoveEndTime);

				if (m_MainCamera)
				{
					m_MainCamera->SetActorRotation(m_CamRotationSpeed);
				}
				break;

			default:
				UE_LOG(LogTemp, Warning, TEXT("Move Type Not Found"));

				break;
			}

			m_MoveCurrentTime += DeltaTime;

			// 規定時間を超えたらターゲット位置にセットする
			if (m_MoveCurrentTime >= m_MoveEndTime)
			{
				m_isMoving = false;

				switch (m_MovingType)
				{
					// Movement の場合
				case EMovingType::Movement:

					m_MoveBlock->SetActorLocation(TargetLocation);

					if (!m_isMoveReturn)
					{
						if (m_MoveDistances.IsValidIndex(m_MoveDistanceIdx + 1))
						{
							++m_MoveDistanceIdx;
							m_MoveDistance = m_MoveDistances[m_MoveDistanceIdx];

							//UE_LOG(LogTemp, Warning, TEXT("IsValidIndex %d"), m_MoveDistanceIdx);
						}
						else
						{
							m_isMoveReturn = true;

							//UE_LOG(LogTemp, Warning, TEXT("Return"), m_MoveDistanceIdx);
						}
					}
					else
					{
						--m_MoveDistanceIdx;

						if (m_MoveDistances.IsValidIndex(m_MoveDistanceIdx))
						{
							m_MoveDistance = m_MoveDistances[m_MoveDistanceIdx];

							//UE_LOG(LogTemp, Log, TEXT("MoveReturn %d"), m_MoveDistanceIdx);
						}
						else
						{
							m_isMoveReturn = false;
							m_MoveDistanceIdx = 0;
							//UE_LOG(LogTemp, Log, TEXT("Not Return"), m_MoveDistanceIdx);
						}
					}

					break;

					// Rotation の場合
				case EMovingType::Rotation:
					m_MoveBlock->SetActorRotation(TargetRotation);
					// UE_LOG(LogTemp, Warning, TEXT("TargetRotation : %s"), *TargetRotation.ToString());
					break;

					// Scale の場合
				case EMovingType::Scale:
					m_MoveBlock->SetActorScale3D(TargetScale);

					m_ScaleSize = m_ScaleSize * -1.f;

					break;

				case EMovingType::ThreeD:
					if (m_PlayerControlCamera != nullptr)
					{
						gamemanager->SetIsDimensionChange(false);
						gamemanager->DirectionCamNumber = 0;
						m_MainCamera->SetActorRotation(m_Default3DRotation);
						m_PlayerControlCamera->SetActorRotation(FRotator(0.f, 0.f, 0.f));
					}
					break;

				case EMovingType::TwoD:
					if (m_PlayerControlCamera != nullptr)
					{
						gamemanager->SetIsDimensionChange(false);
						gamemanager->DirectionCamNumber = 4;
						m_MainCamera->SetActorRotation(m_Default2DRotation);
						m_PlayerControlCamera->SetActorRotation(FRotator(0.f, 0.f, 0.f));
					}
					break;
				}

				// スイッチのタイプが"Once"なら一度だけの状態にする
				if (m_SwitchType == ESwitchType::Once)
				{
					m_isOnce = true;
				}
				// スイッチのタイプが"ManyTimes"なら見た目を元に戻す
				else if (m_SwitchType == ESwitchType::ManyTimes)
				{
					if (SwitchComponent)
					{
						SwitchComponent->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
					
						//if (PressedColor)
						//{
						//	PressedColor->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0.6f, 0.6f, 0.6f));
						//	SwitchComponent->SetMaterial(0, PressedColor);
						//	UE_LOG(LogTemp, Log, TEXT("Not found PressedColor"));
						//}
					}
				}
			}
		}
		// UE_LOG(LogTemp, Log, TEXT("Move Blocks"))
	}
	else
	{
		// 移動後のクールタイムを設定
		m_MoveCurrentTime += DeltaTime;
		if (m_MoveCurrentTime >= m_MoveEndTime + 0.3f)
		{
			m_isSwitch = false;
		}
		return;
	}
}

// 移動の速度を設定
FVector AMoveFloorTest::SetMoveSpeed(EEasingType _EasingType, FVector& _Start, FVector& _End, float _Alpha, float _MoveEndTime)
{
	_Alpha = EasingType(_EasingType, _Alpha, _MoveEndTime);
	return FMath::Lerp(_Start, _End, _Alpha);
}

// 回転の速度を設定
FRotator AMoveFloorTest::SetRotationSpeed(EEasingType _EasingType, FRotator& _Start, FRotator& _End, float _Alpha, float _MoveEndTime)
{
	_Alpha = EasingType(_EasingType, _Alpha, _MoveEndTime);
	return FMath::Lerp(_Start, _End, _Alpha);
}

// 拡縮の速度を設定
FVector AMoveFloorTest::SetScaleSpeed(EEasingType _EasingType, FVector& _Start, FVector& _End, float _Alpha, float _MoveEndTime)
{
	_Alpha = EasingType(_EasingType, _Alpha, _MoveEndTime);
	return FMath::Lerp(_Start, _End, _Alpha);
}

// イージングタイプを設定
float AMoveFloorTest::EasingType(EEasingType _EasingNum, float _Alpha, float _MoveEndTime)
{
	// 範囲チェック
	_Alpha = FMath::Clamp(_Alpha, 0.f, 1.f);

	switch (_EasingNum)
	{
		// イーズアウト
	case EEasingType::EaseOut:
		_Alpha = 1.f - FMath::Pow(1.f - _Alpha, 3);
		//UE_LOG(LogTemp, Log, TEXT("EaseOut Alpha %f"), _Alpha);
		break;

		// イーズイン
	case EEasingType::EaseIn:

		_Alpha = FMath::Pow(_Alpha, 3);
		break;

		// イーズインアウト
	case EEasingType::EaseInOut:

		if (_Alpha <= 0.5f)
		{
			_Alpha = FMath::Pow(_Alpha * 1.5f, 2);
			//UE_LOG(LogTemp, Warning, TEXT("Alpha %f"), _Alpha);
		}
		else if (_Alpha <= 1.0f)
		{
			_Alpha = 1.0f - 0.45f * FMath::Pow(2.f * (1.0f - _Alpha), 2);
			//UE_LOG(LogTemp, Log, TEXT("Alpha %f"), _Alpha);
		}
		break;

		// 等速
	case EEasingType::Linear:
		break;
	default:
		break;
	}

	return _Alpha;
}