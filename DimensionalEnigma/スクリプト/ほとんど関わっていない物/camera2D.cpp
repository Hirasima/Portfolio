#include "camera2D.h"

// コンストラクタ
Acamera2D::Acamera2D() :m_pCamera(NULL), TestNum(0.0f), CameraRot_Pitch(0.0f), CameraRot_Yaw(0.0f)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//トランスフォームのコンポーネントを追加？
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//カメラ初期設定
	InitializeCamera();


}
//デストラクタ
Acamera2D::~Acamera2D()
{

}
void Acamera2D::InitializeCamera()
{
	//カメラオブジェクトを生成し、ルートコンポーネントにくっつける
	UCameraComponent* pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("pCamera"));

	pCamera->SetupAttachment(RootComponent);

	//カメラマネージャー(CCameraManager)クラスを通してカメラを管理
	//マネージャー側に、使用するカメラをセットする
	SetCamera(pCamera);

	//カメラが存在していれば
	if (pCamera != NULL)
	{
		//カメラのそうい位置を設定する
	}

}

//カメラの座標を設定
void Acamera2D::SetCameraPos(float _x, float _y, float _z)
{
	if (m_pCamera != NULL)
	{
		m_pCamera->SetRelativeLocation(FVector(_x, _y, _z));
	}
}
//カメラの角度を設定
void Acamera2D::SetCameraRot(float _pitch, float _yaw, float _roll)
{
	if (m_pCamera != NULL)
	{
		m_pCamera->SetRelativeRotation(FRotator(_pitch, _yaw, _roll));
	}
}

//カメラの角度を取得
FVector3d Acamera2D::GetCameraPos()
{
	FVector3d Pos = FVector3d(0.0f, 0.0f, 0.0f);
	if (m_pCamera != NULL)
	{
		Pos = m_pCamera->GetRelativeLocation();
	}
	return Pos;
}

//カメラの角度を取得
FRotator Acamera2D::GetCameraRot()
{
	FRotator Rot = FRotator(0.0f, 0.0f, 0.0f);
	if (m_pCamera != NULL)
	{
		Rot = m_pCamera->GetRelativeRotation();
	}
	return Rot;
}

//プレイヤー関係
//カメラ更新
void Acamera2D::UpdateCamera()
{
	//親であるアクター自体が回転するため、カメラとキャラが一緒に回転する
	FRotator NewRotation = GetActorRotation();

	//Z軸
	NewRotation.Yaw += m_cameraRotateInput.X;

	//Y軸
	float newPitch = NewRotation.Pitch + m_cameraRotateInput.Y;

	//アクターの回転量を反映
	SetActorRotation(NewRotation);

	//回転できるか確認
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Y)) {
		TestNum += 1.0f;
		SetCameraRot(TestNum, TestNum, 0.0f);
	}

	float Rad_Target = 0.0f;
	float Yaw_Target = 0.0f;

	//目標値を入力
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Z))
	{
		Rad_Target -= 3.1415926f / 18.0f;
		Yaw_Target += 10.0f;
	}
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::C))
	{
		Rad_Target += 3.1415926f / 18.0f;
		Yaw_Target -= 10.0f;
	}
	//目標値に近づける(値の差を一定量割りそれを加算する)
	Rad += (Rad_Target - Rad) / 10.0f;
	CameraRot_Yaw += (Yaw_Target - CameraRot_Yaw) / 10.0f;


	SetCameraPos(sin(Rad - (3.1415926f / 2.0f)) * 100.0f, cos(Rad - (3.1415926f / 2.0f)) * 100.0f, 0.0f);
	SetCameraRot(-30.0f, CameraRot_Yaw, 0.0f);
}

//カメラを初期状態にする
void Acamera2D::SetCameraDefaultCondition()
{
	SetCameraRot(-30.0f, 0.0f, 0.0f);
}

void Acamera2D::UpdatePawn()
{
	if (!m_pawnMoveInput.IsZero())
	{
		m_pawnMoveInput = m_pawnMoveInput.GetSafeNormal() * 10;

		FVector NewLocation = GetActorLocation();

		NewLocation += GetActorForwardVector() * m_pawnMoveInput.Y;
		NewLocation += GetActorRightVector() * m_pawnMoveInput.X;
		SetActorLocation(NewLocation);
	}

}



// Called when the game starts or when spawned
void Acamera2D::BeginPlay()
{
	Super::BeginPlay();

	SetCameraDefaultCondition();
}

// Called every frame
void Acamera2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCamera();
	UpdatePawn();


}

// Called to bind functionality to input
void Acamera2D::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &Acamera2D::Pawn_MoveX);
	InputComponent->BindAxis("MoveRight", this, &Acamera2D::Pawn_MoveZ);

	InputComponent->BindAxis("CameraPitch", this, &Acamera2D::Cam_RotatePitch);
	InputComponent->BindAxis("CameraYaw", this, &Acamera2D::Cam_RotateYaw);

}

//入力バインド
//カメラ回転
void Acamera2D::Cam_RotatePitch(float _axisValue)
{
	m_cameraRotateInput.Y = _axisValue;
}
void Acamera2D::Cam_RotateYaw(float _axisValue)
{
	m_cameraRotateInput.X = _axisValue;
}
void Acamera2D::Pawn_MoveX(float _axisValue)
{
	m_pawnMoveInput.Y = FMath::Clamp(_axisValue, -1.0f, 1.0f);
}
void Acamera2D::Pawn_MoveZ(float _axisValue)
{
	m_pawnMoveInput.X = FMath::Clamp(_axisValue, -1.0f, 1.0f);
}

