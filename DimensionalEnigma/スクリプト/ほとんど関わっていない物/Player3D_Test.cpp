// Player3D_Test.cpp
// 23cu0228 hanawa


#include "Player3D_Test.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MappingCamera3D.h"
#include "WarpTile.h"
#include "DrawDebugHelpers.h"

#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加


// Sets default values
APlayer3D_Test::APlayer3D_Test()
	: m_IsMove(false)
	, m_bWarp(true)
	, m_bCameraMove(false)
	, m_ForwardDirection(FVector::ZeroVector)
	, m_RightDirection(FVector::ZeroVector)
	, m_RayVec(FVector::ZeroVector)
	, m_MoveInput(FVector2D::ZeroVector)
	, m_MoveVector(FVector::ZeroVector)
	, m_MoveSpeed(0.f)
	, m_RotationSpeed(0.f)
	, m_rValue(FRotator::ZeroRotator)
	, RotationAngle(0.f)
	, m_CameraActor(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤー0として設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayer3D_Test::BeginPlay()
{
	Super::BeginPlay();

	// Tag の設定
	Tags.Add(FName(TEXT("Player")));
}

// Called every frame
void APlayer3D_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateOtherActorCameraInfo(m_CameraActor);

	// プレイヤーの更新処理
	UpdateMove();
	//Move(DeltaTime);
}

// Called to bind functionality to input
void APlayer3D_Test::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// プレイヤーの接触処理
void APlayer3D_Test::UpdatePlayerCollision()
{
	// レイ始点
	FVector Start = GetActorLocation();
	// レイ終点
	FVector End = GetActorLocation() + m_RayVec;

	// デバッグライン描画
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f);

	// 自信をレイキャストの判定に含めない
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult OutHit;

	// レイキャスト
	bool isHit = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, FCollisionObjectQueryParams::AllObjects, CollisionParams);
	if (isHit)
	{
		m_IsMove = true;
		m_bWarp = true;

		AActor* HitActor = OutHit.GetActor();

		// ヒットしたオブジェクトが"WarpTile"なら指定した座標に移動
		if (HitActor->ActorHasTag(FName("WarpTile")))
		{
			// ワープタイルのワープ処理とワープ不可時のbool値の取得
			AWarpTile* pWarpTile = Cast<AWarpTile>(HitActor);

			pWarpTile->WarpPlayer(this);
			m_bWarp = pWarpTile->GetIsWarp();

			if (!m_bWarp) { UE_LOG(LogTemp, Warning, TEXT("Not Hit WarpTile")); }
		}

		// UE_LOG(LogTemp, Log, TEXT("Hit"));
	}
	else
	{
		m_IsMove = false;
		// UE_LOG(LogTemp, Log, TEXT("No Hit"));
	}
}

// 移動用カメラの方向を取得(更新処理)
void APlayer3D_Test::UpdateOtherActorCameraInfo(class AActor* CameraActor)
{
	// 移動用カメラがセットされていたら処理を行う
	if (CameraActor)
	{
		// カメラの移動中のフラグ
		m_bCameraMove = Cast<AMappingCamera3D>(CameraActor)->GetCameraMoving();

		// 移動用カメラのコンポーネントの情報を取得する
		UCameraComponent* CameraComp = CameraActor->FindComponentByClass<UCameraComponent>();

		if (CameraComp)
		{
			m_ForwardDirection = CameraComp->GetForwardVector();		// カメラの前ベクター
			m_RightDirection = CameraComp->GetRightVector();			// カメラの左ベクター
		}
	}
}

// プレイヤーの更新処理
void APlayer3D_Test::UpdateMove()
{
	if (!m_bCameraMove)
	{
		// "W"キーが一度押されたらX軸プラス方向に進む処理
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::W))
		{
			// レイを飛ばして判定を行う
			m_RayVec = FVector(m_ForwardDirection.X * 13.f, m_ForwardDirection.Y * 13.f, -10.f);
			UpdatePlayerCollision();

			// 移動可能かつワープ可能な際 X軸プラス方向に進む
			if (m_IsMove)
			{
				m_IsMoving = true;
				if (!m_bWarp) { return; }

				AddActorLocalOffset(m_ForwardDirection * 10.f);
			}
		}
		// "S"キーが一度押されたらX軸マイナス方向に進む処理
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::S))
		{
			// レイを飛ばして判定を行う
			m_RayVec = FVector(m_ForwardDirection.X * -13.f, m_ForwardDirection.Y * -13.f, -10.f);
			UpdatePlayerCollision();

			// 移動可能かつワープ可能な際 X軸マイナス方向に進む
			if (m_IsMove)
			{
				if (!m_bWarp) { return; }

				AddActorLocalOffset(m_ForwardDirection * -10.f);
			}
		}
		// "D"キーが一度押されたらY軸プラス方向に進む処理
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::D))
		{
			// レイを飛ばして判定を行う
			m_RayVec = FVector(m_RightDirection.X * 13.f, m_RightDirection.Y * 13.f, -10.f);
			UpdatePlayerCollision();

			// 移動可能かつワープ可能な際 Y軸プラス方向に進む
			if (m_IsMove)
			{
				if (!m_bWarp) { return; }

				AddActorLocalOffset(m_RightDirection * 10);
			}
		}
		// "A"キーが一度押されたらY軸マイナス方向に進む処理
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::A))
		{
			// レイを飛ばして判定を行う
			m_RayVec = FVector(m_RightDirection.X * -13.f, m_RightDirection.Y * -13.f, -10.f);
			UpdatePlayerCollision();

			// 移動可能かつワープ可能な際 Y軸マイナス方向に進む
			if (m_IsMove)
			{
				if (!m_bWarp) { return; }

				AddActorLocalOffset(m_RightDirection * -10);
			}
		}
	}
}


// 動きの設定
void APlayer3D_Test::SetMovement(float angleDeg) {

	// 右上
	if (angleDeg >= 0 && angleDeg < 90) {

		m_rValue = (FRotator(-RotationAngle, .0f, .0f));
		m_MoveVector = FVector(m_MoveSpeed, 0.0f, 0.0f);

		return;
	}

	// 右下
	if (angleDeg >= 90 && angleDeg < 180) {

		m_rValue = (FRotator(.0f, .0f, RotationAngle));
		m_MoveVector = FVector(0.0f, m_MoveSpeed, 0.0f);

	}

	// 左下
	if (angleDeg >= 180 && angleDeg < 270) {

		m_rValue = (FRotator(RotationAngle, .0f, .0f));
		m_MoveVector = FVector(-m_MoveSpeed, 0.0f, 0.0f);
	}

	// 左上
	if (angleDeg >= 270 && angleDeg <= 360) {

		m_rValue = (FRotator(.0f, .0f, -RotationAngle));
		m_MoveVector = FVector(.0f, -m_MoveSpeed, .0f);
	}

}

void APlayer3D_Test::Move(float _DeltaTime) 
{
	// 移動していないなら実行しない
	if (m_IsMoving == false) { return; }
	UE_LOG(LogTemp, Log, TEXT("Move"));

	AddActorWorldRotation(m_rValue * _DeltaTime, false, 0, ETeleportType::TeleportPhysics);

	AddActorWorldOffset(m_MoveVector * _DeltaTime);

	ElapsedTime += _DeltaTime;


	// 経過時間が指定した時間を超えたら
	if (ElapsedTime >= m_RotationSpeed) {

		//SetActorRotation(targetRotation, ETeleportType::None);
		//UE_LOG(LogTemp, Warning, TEXT("Moved %s"), *targetRotation.ToString());
		m_IsMoving = false;
		ElapsedTime = 0.0f;
	}
}

// ここからはEnhanced Inputの関数
//--------------------------------------------------------------------------


void APlayer3D_Test::SetupInput()
{
	// PlayerControllerを取得する
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 入力を有効にする
	EnableInput(controller);

	if (InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

			// カメラのボタンが押されたときに実行
			EnhancedInputComponent->BindAction(CameraInput, ETriggerEvent::Triggered, this, &APlayer3D_Test::PressedCamera);

			// 移動のボタンが押されたときに実行
			EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &APlayer3D_Test::PressedAxis);
			// スタートボタン
			EnhancedInputComponent->BindAction(StartInput, ETriggerEvent::Triggered, this, &APlayer3D_Test::PressedStart);
			// Aボタン
			EnhancedInputComponent->BindAction(ActionInput, ETriggerEvent::Triggered, this, &APlayer3D_Test::PressedAction);
			// Bボタン
			EnhancedInputComponent->BindAction(BackInput, ETriggerEvent::Triggered, this, &APlayer3D_Test::PressedBack);

		}

		// マッピングコンテキストを適用
		if (APlayerController* PlayerController = Cast<APlayerController>(controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {

				subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}

	}

}

// L、Rボタンでカメラの回転
void APlayer3D_Test::PressedCamera(const FInputActionValue& Value) {

	// UKismetSystemLibrary::PrintString(this, TEXT("Camera Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));



	FVector2D v = Value.Get<FVector2D>();

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Camera: X:%f Y:%f"), v.X, v.Y), true, true, FColor::Cyan, 10.0f, TEXT("None"));



}

// Startボタン
void APlayer3D_Test::PressedStart() {

	UKismetSystemLibrary::PrintString(this, TEXT("Start Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}

// 左スティックで移動
void APlayer3D_Test::PressedAxis(const FInputActionValue& Value) {

	// 移動しているなら実行しない
	if (m_IsMoving) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Pressed"))

	m_MoveInput = Value.Get<FVector2D>();


	// 入力された方向を360°表記(Radian)に変換
	float angle = atan2(m_MoveInput.X, m_MoveInput.Y);
	float angleDeg = FMath::RadiansToDegrees(angle);

	// マイナスの場合帳消しにする
	if (angleDeg < 0) {
		angleDeg += 360;
	}

	SetMovement(angleDeg);
	m_IsMoving = true;


	// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Moved: X:%f Y:%f"), m_MoveInput.X, m_MoveInput.Y), true, true, FColor::Cyan, 10.0f, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Degree: %f"), angleDeg), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}

// Aボタン(海外での〇)でアクションを起こす
void APlayer3D_Test::PressedAction()
{

	UKismetSystemLibrary::PrintString(this, TEXT("Action Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}

// Backspaceキー、Bボタン（海外でのX）で戻る
void APlayer3D_Test::PressedBack()
{

	UKismetSystemLibrary::PrintString(this, TEXT("Back Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}




//void APlayer3D_Test::MoveForward(float Value)
//{
//}
//
//void APlayer3D_Test::MoveRight(float Value)
//{
//}