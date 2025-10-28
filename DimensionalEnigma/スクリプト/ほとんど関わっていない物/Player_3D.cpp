// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_3D.h"
#include "MappingCamera3D.h"
#include "WarpTile.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加

// Sets default values
APlayer_3D::APlayer_3D()
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
void APlayer_3D::BeginPlay()
{
	Super::BeginPlay();
	
	// Tag の設定
	Tags.Add(FName(TEXT("Player")));
}

// Called every frame
void APlayer_3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateOtherActorCameraInfo(m_CameraActor);

	// プレイヤーの更新処理
	UpdateMove(DeltaTime);
}

// Called to bind functionality to input
void APlayer_3D::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// プレイヤーの接触処理
void APlayer_3D::UpdatePlayerCollision()
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
void APlayer_3D::UpdateOtherActorCameraInfo(class AActor* CameraActor)
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

// m_Moving が true の時の移動処理
void APlayer_3D::UpdateMove(float DeltaTime) {

	// 移動していないなら実行しない
	if (m_IsMoving == false) { return; }


	AddActorWorldRotation(m_rValue * DeltaTime, false, 0, ETeleportType::TeleportPhysics);

	AddActorWorldOffset(m_MoveVector * DeltaTime);

	ElapsedTime += DeltaTime;


	// 経過時間が指定した時間を超えたら
	if (ElapsedTime >= m_RotationSpeed) {

		//SetActorRotation(targetRotation, ETeleportType::None);
		//UE_LOG(LogTemp, Warning, TEXT("Moved %s"), *targetRotation.ToString());
		m_IsMoving = false;
		ElapsedTime = 0.0f;
	}

}

// 動きの設定
void APlayer_3D::SetMovement(float angleDeg) {

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

// ここからはEnhanced Inputの関数
//--------------------------------------------------------------------------

// Inputの初期設定
void APlayer_3D::SetupInput()
{
	// PlayerControllerを取得
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 入力を有効にする
	EnableInput(controller);

	if (InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

			// カメラのボタンが押されたときに実行
			EnhancedInputComponent->BindAction(CameraInput, ETriggerEvent::Triggered, this, &APlayer_3D::PressedCamera);

			// 移動のボタンが押されたときに実行
			EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &APlayer_3D::PressedAxis);
			// スタートボタン
			EnhancedInputComponent->BindAction(StartInput, ETriggerEvent::Triggered, this, &APlayer_3D::PressedStart);
			// Aボタン
			EnhancedInputComponent->BindAction(ActionInput, ETriggerEvent::Triggered, this, &APlayer_3D::PressedAction);
			// Bボタン
			EnhancedInputComponent->BindAction(BackInput, ETriggerEvent::Triggered, this, &APlayer_3D::PressedBack);

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
void APlayer_3D::PressedCamera(const FInputActionValue& Value) {

	// UKismetSystemLibrary::PrintString(this, TEXT("Camera Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

	FVector2D v = Value.Get<FVector2D>();


	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Camera: X:%f Y:%f"), v.X, v.Y), true, true, FColor::Cyan, 10.0f, TEXT("None"));



}

// Startボタン
void APlayer_3D::PressedStart() {

	UKismetSystemLibrary::PrintString(this, TEXT("Start Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}

// 左スティックで移動
void APlayer_3D::PressedAxis(const FInputActionValue& Value) {

	// 移動しているなら実行しない
	if (m_IsMoving == true) { return; }

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
void APlayer_3D::PressedAction()
{

	UKismetSystemLibrary::PrintString(this, TEXT("Action Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}

// Backspaceキー、Bボタン（海外でのX）で戻る
void APlayer_3D::PressedBack()
{

	UKismetSystemLibrary::PrintString(this, TEXT("Back Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

}
