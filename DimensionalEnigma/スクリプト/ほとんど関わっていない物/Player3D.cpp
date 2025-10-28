// 制作者：土谷祐太
// 三次元状態でのキューブを管理する

#include "Player3D.h"
#include "Components/StaticMeshComponent.h"
#include "WarpTile.h"
#include "MappingCamera3D.h"
#include "CameraPawn0332.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加

#include "SoundManagerSubsystem.h"
#include "Math/UnrealMathUtility.h"

#include "EnigmaGameInstance.h"
#include "GameManager.h"
#include "StageSubSystem.h"

// Sets default values
APlayer3D::APlayer3D()
	: m_IsMoving(false)
	, m_MoveSpeed(20.f)
	, m_MoveVector(.0f, .0f, .0f)
	, m_rValue(FRotator::ZeroRotator)
	, RotationAngle(180.f)
	, m_RotationSpeed(2.0f)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// アクターのStaticMeshComponentを取得
	MeshComponent = this->FindComponentByClass<UStaticMeshComponent>();

	// プレイヤー0として設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	if (StickRInput_X == nullptr)
	{
		StickRInput_X = LoadObject<UInputAction>(nullptr, TEXT("/Game/EnhancedInput/Input/IA_StickR_X.IA_StickR_X"));
	}
	if (StickRInput_Y == nullptr)
	{
		StickRInput_Y = LoadObject<UInputAction>(nullptr, TEXT("/Game/EnhancedInput/Input/IA_StickR_Y.IA_StickR_Y"));
	}

}

// Called when the game starts or when spawned
void APlayer3D::BeginPlay()
{
	Super::BeginPlay();

	SetupInput();

	// GameManagerを取得
	world = GetWorld();
	if (world) {
		gameinstance = Cast<UEnigmaGameInstance>(world->GetGameInstance());
		if (gameinstance) {
			gamemanager = world->GetSubsystem<UStageSubSystem>();
		}
	}


	Tags.Add(FName("Player"));

	m_StartLocation = GetActorLocation();

	//--------大竹追加
	//基本座標を初期化
	m_BasePlayerPos = GetActorLocation();
	//--------

	//SEを再生(無音)空撃ちさせる
	USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	if (SoundManager)
	{
		// パスで音源を再生
		SoundManager->PlaySoundFromPath(MoveSE_Path, 0.001f);

		UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
	}
}

// Called every frame
void APlayer3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateOtherActorCameraInfo(m_CameraActor);

	UpdateMove(DeltaTime);

	UpdateGoalAction(DeltaTime);//大竹追加

	FreeCameraControl(DeltaTime);//大竹追加
}

// Called to bind functionality to input
void APlayer3D::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Gamepad_LeftX", this, &APlayer3D::MoveRight);
}

// プレイヤーの接触処理
void APlayer3D::UpdatePlayerCollision()
{
	// レイ始点
	FVector Start = GetActorLocation();
	// レイ終点
	FVector End = GetActorLocation() + m_RayVec;

	//// デバッグライン描画
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f);

	// 自信をレイキャストの判定に含めない
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult OutHit;

	// レイキャスト
	bool isHit = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, FCollisionObjectQueryParams::AllObjects, CollisionParams);
	if (isHit && !gamemanager->GetCameraMoving() && !gamemanager->GetGoaling())
	{
		m_IsMove = true;
		m_bWarp = true;

		AActor* HitActor = OutHit.GetActor();
		m_DestinationActor = HitActor;
		m_DestinationPos = m_DestinationActor->GetActorLocation();

		// ヒットしたオブジェクトが"WarpTile"なら指定した座標に移動
		if (HitActor->ActorHasTag(FName("WarpTile")))
		{
			// ワープタイルのワープ処理とワープ不可時のbool値の取得
			AWarpTile* pWarpTile = Cast<AWarpTile>(HitActor);

			pWarpTile->WarpPlayer(this);
			m_bWarp = pWarpTile->GetIsWarp();
			m_bDownWarp = pWarpTile->GetIsWarped();
			m_DownWarpLocation = pWarpTile->GetDownWarpLocation();

			if (!m_bWarp) { UE_LOG(LogTemp, Warning, TEXT("Not Hit WarpTile")); }
		}
		else
		{
			m_TargetLocation = HitActor->GetActorLocation();
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
void APlayer3D::UpdateOtherActorCameraInfo(class AActor* CameraActor)
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
void APlayer3D::UpdateMove(float DeltaTime)
{
	if (!m_IsMoving) { return; }

	if (!m_IsFriezeChecked)
	{
		if (m_DestinationPos != m_DestinationActor->GetActorLocation())
		{
			gamemanager->SetPlayerMoving(false);

			SetActorLocation(m_StartLocation);

			m_IsMoving = false;
			Cast<ACameraPawn0332>(m_MainCamera)->SetIsPlayerMoving(false);
			Cast<AMappingCamera3D>(m_CameraActor)->SetIsPlayerMoving(false);
			UE_LOG(LogTemp, Warning, TEXT("false"));
			ElapsedTime = 0.0f;

			return;
		}
	}
	//問題なかった為trueに
	m_IsFriezeChecked = true;
	

	
	if (gamemanager->GetIsSelect()) { gamemanager->SetIsSelect(false); }

	if (gamemanager)
	{
		gamemanager->SetPlayerMoving(true);
	}

	Cast<ACameraPawn0332>(m_MainCamera)->SetIsPlayerMoving(true);
	Cast<AMappingCamera3D>(m_CameraActor)->SetIsPlayerMoving(false);

	AddActorWorldRotation(m_rValue * DeltaTime * m_RotationSpeed);

	//-----------------------------------------------------大竹追加
	//角度に合わせてプレイヤーの高さを少し上げる
	FVector PlayerPos = GetActorLocation();//取得
	FRotator PlayerRot = GetActorRotation();

	//1F前の状況に戻す
	PlayerPos.Z -= m_AddjustRotHeight;

	//基本座標として記録
	m_BasePlayerPos = PlayerPos;

	m_AddjustRotHeight = CalculateHeight(PlayerRot.Pitch + PlayerRot.Yaw + PlayerRot.Roll) * 10.0f;//ずらす高さを計算
	PlayerPos.Z += m_AddjustRotHeight;//補正
	SetActorLocation(PlayerPos);//代入
	//-----------------------------------------------------

	AddActorWorldOffset(m_MoveVector * DeltaTime * m_RotationSpeed);

	ElapsedTime += DeltaTime * m_RotationSpeed;

	// 経過時間が指定した時間を超えたら
	if (ElapsedTime >= 0.47f)//大竹少々変更 0.5fから0.47f
	{
		gamemanager->SetPlayerMoving(false);

		UE_LOG(LogTemp, Log, TEXT("TargetLocation %s"), *m_TargetLocation.ToString());

		SetActorRotation(m_rValue * 0.5f * m_RotationSpeed);
		//SetActorLocation(FVector(FMath::Floor(TargetLocation.X), FMath::Floor(TargetLocation.Y), FMath::Floor(TargetLocation.Z)));


		if (m_bDownWarp)
		{
			m_StartLocation = m_DownWarpLocation + FVector(m_MoveVector * 0.25f * m_RotationSpeed);

			m_bDownWarp = false;
			UE_LOG(LogTemp, Warning, TEXT("DownWarp"))
		}
		else
		{
			m_StartLocation += FVector(m_MoveVector * 0.25f * m_RotationSpeed);
			UE_LOG(LogTemp, Warning, TEXT("location %s"), *FVector(m_MoveVector * 0.25f * m_RotationSpeed).ToString());
		}

		SetActorLocation(m_StartLocation);

		if (gamemanager)
		{
			gamemanager->AddPlayerMoveCount();
		}

		//-------------------------------------大竹追加
		//SEを再生
		USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
		if (SoundManager)
		{
			// パスで音源を再生
			SoundManager->PlaySoundFromPath(MoveSE_Path);

			UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
		}


		//静止チェックをもとに戻す
		m_IsFriezeChecked = false;

		//高さの補正量を戻しておく
		m_AddjustRotHeight = 0.0f;

		//基本座標として記録
		m_BasePlayerPos = GetActorLocation();
		//-------------------------------------

		m_IsMoving = false;
		Cast<ACameraPawn0332>(m_MainCamera)->SetIsPlayerMoving(false);
		Cast<AMappingCamera3D>(m_CameraActor)->SetIsPlayerMoving(false);
		UE_LOG(LogTemp, Warning, TEXT("false"));
		ElapsedTime = 0.0f;
	}
}

// 動きの設定
void APlayer3D::SetMovement(float angleDeg)
{
	if (m_MoveInput.X == 0.f)
	{
		m_rValue = (FRotator(m_ForwardDirection.X * m_MoveInput.Y * -RotationAngle, 0.f, m_ForwardDirection.Y * m_MoveInput.Y * RotationAngle));
		m_MoveVector = FVector(m_ForwardDirection.X * m_MoveInput.Y * m_MoveSpeed, m_ForwardDirection.Y * m_MoveInput.Y * m_MoveSpeed, 0.f);
	}

	if (m_MoveInput.Y == 0.f)
	{
		m_rValue = (FRotator(m_RightDirection.X * m_MoveInput.X * -RotationAngle, 0.f, m_RightDirection.Y * m_MoveInput.X * RotationAngle));
		m_MoveVector = FVector(m_RightDirection.X * m_MoveInput.X * m_MoveSpeed, m_RightDirection.Y * m_MoveInput.X * m_MoveSpeed, 0.f);
	}
}

bool APlayer3D::GetIsWarpable()
{
	return m_IsMoving;
}

// ここからはEnhanced Inputの関数
//--------------------------------------------------------------------------

// Inputの初期設定
void APlayer3D::SetupInput()
{
	// PlayerControllerを取得
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 入力を有効にする
	EnableInput(controller);

	if (InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

			// カメラのボタンが押されたときに実行
			EnhancedInputComponent->BindAction(CameraInput, ETriggerEvent::Triggered, this, &APlayer3D::PressedCamera);

			// 移動のボタンが押されたときに実行
			EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &APlayer3D::PressedAxis);
			// スタートボタン
			EnhancedInputComponent->BindAction(StartInput, ETriggerEvent::Triggered, this, &APlayer3D::PressedStart);
			// Aボタン
			EnhancedInputComponent->BindAction(ActionInput, ETriggerEvent::Triggered, this, &APlayer3D::PressedAction);
			// Bボタン
			EnhancedInputComponent->BindAction(BackInput, ETriggerEvent::Triggered, this, &APlayer3D::PressedBack);

			// 右スティック
			EnhancedInputComponent->BindAction(StickRInput_X, ETriggerEvent::Triggered, this, &APlayer3D::PressedStickR_X);
			EnhancedInputComponent->BindAction(StickRInput_Y, ETriggerEvent::Triggered, this, &APlayer3D::PressedStickR_Y);
		}

		// マッピングコンテキストを適用
		if (APlayerController* PlayerController = Cast<APlayerController>(controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// L、Rボタンでカメラの回転
void APlayer3D::PressedCamera(const FInputActionValue& Value)
{
	if (gamemanager->GetDimensionType() == EDimensionType::TwoD || gamemanager->GetIsDimensionChange()) { return; }

	//カメラとプライヤーどちらも動いていないことを確認する
	if (!gamemanager->GetPlayerMoving() && !gamemanager->GetCameraMoving())
	{
		FVector2D v = Value.Get<FVector2D>();

		if (m_MainCamera)
		{
			AMappingCamera3D* pMappingCamera = Cast<AMappingCamera3D>(m_CameraActor);
			ACameraPawn0332* pMainCamera = Cast<ACameraPawn0332>(m_MainCamera);
			pMainCamera->SetCameraInput(v);
			pMainCamera->SetCameraRotateInputX();

			pMappingCamera->SetCameraInput(v);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Cam Moving"));
	}
}

// Startボタン
void APlayer3D::PressedStart()
{
	//UKismetSystemLibrary::PrintString(this, TEXT("Start Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));
}

// 左スティックで移動
void APlayer3D::PressedAxis(const FInputActionValue& Value)
{
	// 移動しているとき、又は次元変化中なら実行しない
	if (m_IsMoving || gamemanager->GetIsDimensionChange()) { return; }

	//スティックカメラ中なら実行しない
	ACameraPawn0332* pMainCamera = Cast<ACameraPawn0332>(m_MainCamera);
	if (pMainCamera->GetFreeCameraRot() != FRotator(0.0f, 0.0f, 0.0f)) { return; }

	if (!m_bCameraMove)
	{

		m_MoveInput = Value.Get<FVector2D>();

		// MoveInput の XY が逆
		if (m_MoveInput.X == 0.f)
		{
			m_RayVec = FVector(m_ForwardDirection.X * m_MoveInput.Y * 13.f, m_ForwardDirection.Y * m_MoveInput.Y * 13.f, -10.f);
			UpdatePlayerCollision();

			if (m_IsMove)
			{
				if (!m_bWarp) { return; }

				// 入力された方向を360°表記(Radian)に変換
				float angle = atan2(m_ForwardDirection.Y * m_MoveInput.Y, m_ForwardDirection.X * m_MoveInput.Y);
				float angleDeg = FMath::RadiansToDegrees(angle);

				UE_LOG(LogTemp, Warning, TEXT("X ForwardD %s RightD %s"), *m_ForwardDirection.ToString(), *m_RightDirection.ToString());

				// マイナスの場合帳消しにする
				if (angleDeg < 0)
				{
					angleDeg += 360;
				}

				SetMovement(angleDeg);

				m_IsMoving = true;

				// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Moved: X:%f Y:%f"), m_MoveInput.X, m_MoveInput.Y), true, true, FColor::Cyan, 10.0f, TEXT("None"));
				//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Degree: %f"), angleDeg), true, true, FColor::Cyan, 10.0f, TEXT("None"));
			}
		}
		else if (m_MoveInput.Y == 0.f)
		{
			m_RayVec = FVector(m_RightDirection.X * m_MoveInput.X * 13.f, m_RightDirection.Y * m_MoveInput.X * 13.f, -10.f);
			UpdatePlayerCollision();

			if (m_IsMove)
			{
				if (!m_bWarp) { return; }

				// 入力された方向を360°表記(Radian)に変換
				float angle = atan2(m_RightDirection.Y * m_MoveInput.X, m_RightDirection.X * m_MoveInput.X);
				float angleDeg = FMath::RadiansToDegrees(angle);

				UE_LOG(LogTemp, Warning, TEXT("X ForwardD %s RightD %s"), *m_ForwardDirection.ToString(), *m_RightDirection.ToString());

				// マイナスの場合帳消しにする
				if (angleDeg < 0)
				{
					angleDeg += 360;
				}

				SetMovement(angleDeg);

				m_IsMoving = true;

				// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Moved: X:%f Y:%f"), m_MoveInput.X, m_MoveInput.Y), true, true, FColor::Cyan, 10.0f, TEXT("None"));
				//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Degree: %f"), angleDeg), true, true, FColor::Cyan, 10.0f, TEXT("None"));
			}
		}
	}
}

// Aボタン(海外での〇)でアクションを起こす
void APlayer3D::PressedAction()
{
	gamemanager->SetIsSelect(true);
	//UKismetSystemLibrary::PrintString(this, TEXT("Action Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));
}

// Backspaceキー、Bボタン（海外でのX）で戻る
void APlayer3D::PressedBack()
{
	//UKismetSystemLibrary::PrintString(this, TEXT("Back Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));
}

//右Stick
void APlayer3D::PressedStickR_X(const FInputActionValue& Value)
{
	//真上視点の場合は処理しない
	if (EDimensionType::TwoD == gamemanager->GetDimensionType() || gamemanager->GetIsDimensionChange()) { return; }

	FVector2D _StickR_Input = Value.Get<FVector2D>();
	m_StickR_Value.X = _StickR_Input.X;

	ACameraPawn0332* pMainCamera = Cast<ACameraPawn0332>(m_MainCamera);

	//プレイヤーが動いてたらフリーカメラを元に戻して即終了
	if (m_IsMoving)
	{

		pMainCamera->SetFreeCameraRot(FRotator(0.0f, 0.0f, 0.0f));
		return;
	}

	//pMainCamera->SetFreeCameraRot(FRotator(0.0f, _StickR_Input.X, 0.0f) + pMainCamera->GetFreeCameraRot());
}
//右Stick
void APlayer3D::PressedStickR_Y(const FInputActionValue& Value)
{
	//真上視点の場合は処理しない
	if (EDimensionType::TwoD == gamemanager->GetDimensionType() || gamemanager->GetIsDimensionChange()) { return; }

	FVector2D _StickR_Input = Value.Get<FVector2D>();
	m_StickR_Value.Y = _StickR_Input.X;

	ACameraPawn0332* pMainCamera = Cast<ACameraPawn0332>(m_MainCamera);

	//プレイヤーが動いてたらフリーカメラを元に戻して即終了
	if (m_IsMoving)
	{

		pMainCamera->SetFreeCameraRot(FRotator(0.0f, 0.0f, 0.0f));
		return;
	}
	
	if (_StickR_Input.X > 0.0f)//プラスの時
	{
		if (pMainCamera->GetFreeCameraRot().Pitch > 30.0f)//30を超えそう？
		{
			_StickR_Input.X = 0.0f;
		}
	}
	else
		if (_StickR_Input.X < 0.0f)//マイナスの時
		{
			if (pMainCamera->GetFreeCameraRot().Pitch < -30.0f)//-30を超えそう？
			{
				_StickR_Input.X = 0.0f;
			}
		}

	//pMainCamera->SetFreeCameraRot(FRotator( _StickR_Input.X,0.0f, 0.0f) + pMainCamera->GetFreeCameraRot());
}


//--------大竹が追加した関数

float APlayer3D::CalculateHeight(float Degrees)//移動時角度に応じてプレイヤーの高さを少し変える
{
	// 角度を0～90度の範囲に正規化
	while (Degrees < 0.0f)
	{
		Degrees += 90.0f;
	}
	while (Degrees > 90.0f)
	{
		Degrees -= 90.0f;
	}

	Degrees += 45.0f;//45度ずらす

	// 角度をラジアンに変換
	float AngleRadians = FMath::DegreesToRadians(Degrees);

	// 高さ（sin値）を計算
	float Height = FMath::Sin(AngleRadians) - FMath::DegreesToRadians(45.0f);


	if (Height < 0)
	{
		Height = 0;
	}
	return Height;
}
void APlayer3D::UpdateGoalAction(float DeltaTime)
{
	if (!m_GoalFlag || m_IsMoving)
	{
		return;
	}

	FVector PlayerPos = GetActorLocation();
	if (PlayerPos.Z <= m_BasePlayerPos.Z - m_SinkHeight)//沈み込む高さを超えたら終了
	{
		PlayerPos.Z = m_BasePlayerPos.Z - m_SinkHeight;//高さを揃える
		SetActorLocation(PlayerPos);
		return;
	}

	//高さを下げる
	m_SinkVel -= m_SinkAcc * DeltaTime * 120.0f;//速度を加速させる
	PlayerPos.Z += m_SinkVel * DeltaTime;
	SetActorLocation(PlayerPos);
}
void APlayer3D::SetGoalFlag(bool _Flag)
{
	m_GoalFlag = _Flag;
}
float APlayer3D::GetFinHeight()//終点の高さを取得
{
	return m_StartLocation.Z;
}
bool APlayer3D::GetIsMoving()
{
	return m_IsMoving;
}
FVector APlayer3D::GetBasePlayerPos()
{
	return m_BasePlayerPos;
}

void APlayer3D::FreeCameraControl(float _DeltaTime)
{
	ACameraPawn0332* pMainCamera = Cast<ACameraPawn0332>(m_MainCamera);

	//プレイヤーが動いてたらフリーカメラを元に戻して即終了
	if (m_IsMoving)
	{

		pMainCamera->SetFreeCameraRot(FRotator(0.0f, 0.0f, 0.0f));
		return;
	}

	//徐々に戻す
	FRotator FreeRot = pMainCamera->GetFreeCameraRot();
	FRotator AddRot = -1.0f * (FreeRot * 0.05f) * (_DeltaTime * 120.0f);//減衰量の計算
	pMainCamera->SetFreeCameraRot(FreeRot + AddRot);
	if (FreeRot.Pitch < 0.05f && FreeRot.Pitch > -0.05f &&
		FreeRot.Roll < 0.05f && FreeRot.Roll > -0.05f &&
		FreeRot.Yaw < 0.05f && FreeRot.Yaw > -0.05f)
	{
		pMainCamera->SetFreeCameraRot(FRotator(0.0f, 0.0f, 0.0f));
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
	{
		pMainCamera->SetFreeCameraRot(FRotator(0.0f, 0.0f, 0.0f));
	}

	if (m_StickR_Value.Y > 0.0f)//プラスの時
	{
		if (pMainCamera->GetFreeCameraRot().Pitch > 30.0f)//30を超えそう？
		{
			m_StickR_Value.Y = 0.0f;
		}
	}
	else
		if (m_StickR_Value.Y < 0.0f)//マイナスの時
		{
			if (pMainCamera->GetFreeCameraRot().Pitch < -30.0f)//-30を超えそう？
			{
				m_StickR_Value.Y = 0.0f;
			}
		}
	//カメラを傾ける
	pMainCamera->SetFreeCameraRot(FRotator(m_StickR_Value.Y * _DeltaTime * 120.0f, m_StickR_Value.X * _DeltaTime * 120.0f, 0.0f) + pMainCamera->GetFreeCameraRot());


	// ゲームパッドの左スティックY軸の入力値を取得
	//float CurrentStickValueX = GetWorld()->GetFirstPlayerController()->GetInputAxisValue("Gamepad_LeftX");
	// ゲームパッドの左スティックY軸の入力値を取得
	//float CurrentStickValueY = GetWorld()->GetFirstPlayerController()->GetInputAxisValue("Gamepad_LeftY");
	//if (CurrentStickValueX != 0.0f || CurrentStickValueY != 0)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("InputStick!!"));
	}
	//if (GetWorld()->GetFirstPlayerController()->GetInputAxisValue("MoveRight"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("InputStick!!"));
	}

	//pMainCamera->SetFreeCameraRot(FRotator(0.0f, CurrentStickValueY*10.0f, CurrentStickValueX*10.0f));

	//入力値を元に戻しておく
	m_StickR_Value = FVector(0.0f, 0.0f, 0.0f);
}

void APlayer3D::MoveRight(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("LeftX: %f"), Value);
}