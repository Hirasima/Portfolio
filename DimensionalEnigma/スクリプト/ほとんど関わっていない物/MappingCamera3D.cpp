// MappingCamera3D.cpp
// 23cu0228 hanawa

#include "MappingCamera3D.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMappingCamera3D::AMappingCamera3D()
	: m_CameraRotation(FRotator::ZeroRotator)
	, m_DirectionCamNumber(0)
	, m_lerpRatio(0.06f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMappingCamera3D::BeginPlay()
{
	Super::BeginPlay();
	
	// GameManagerを取得
	world = GetWorld();
	if (world) {
		gameinstance = Cast<UEnigmaGameInstance>(world->GetGameInstance());
		if (gameinstance) {
			gamemanager = world->GetSubsystem<UStageSubSystem>();
		}
	}

}

// Called every frame
void AMappingCamera3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_CameraRotation = this->GetActorRotation();

	//UpdateMappingCamera(DeltaTime);

	SetCameraRotateInputX();
	UpdateCamera();
}

// Called to bind functionality to input
void AMappingCamera3D::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// カメラの画角番号を返す処理
int AMappingCamera3D::GetDirectionNumber()
{
	return m_DirectionCamNumber;
}

// カメラの更新処理
void AMappingCamera3D::UpdateMappingCamera(float DeltaTime)
{
	//// 矢印「右」キーで反時計回りに回転
	//if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Right))
	//{
	//	// SetActorRotation(m_CameraRotation + FRotator(0.0f, -m_RotationValue, 0.0f));

	//	AddActorLocalRotation(FRotator(0.f, -90.f, 0.f));

	//	// カメラの画角番号を変更する
	//	++m_DirectionCamNumber;
	//	if (m_DirectionCamNumber > 3)
	//	{
	//		m_DirectionCamNumber = 0;
	//	}
	//}

	//// 矢印「左」キーで時計回りに回転
	//if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Left))
	//{
	//	// SetActorRotation(m_CameraRotation + FRotator(0.0f, m_RotationValue, 0.0f));

	//	AddActorLocalRotation(FRotator(0.f, 90.f, 0.f));

	//	// 画角番号を変更する
	//	--m_DirectionCamNumber;
	//	if (m_DirectionCamNumber < 0)
	//	{
	//		m_DirectionCamNumber = 3;
	//	}
	//}
}

void AMappingCamera3D::UpdateCamera()
{
	if (m_IsPlayerMoving) { return; }

	if (!m_cameraMoving)
	{
		if (m_inputRotationDireX != 0)
		{
			m_rotationDireX = m_inputRotationDireX;

			m_targetAngle = GetActorRotation() + FRotator(0.0f, 90.0f * m_rotationDireX, 0.0f);

			if (m_lerpRatio <= 0 || m_lerpRatio > 1)
				m_lerpRatio = 0.06;

			//UE_LOG(LogTemp, Log, TEXT("Start Move Target Yaw : %f"), m_targetAngle.Yaw);

			//移動中に変更
			m_cameraMoving = true;
			//UE_LOG(LogTemp, Log, TEXT("m_cameraMoving Is %d"), (int)m_cameraMoving);
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("No Camera Input!"));
		}
	}
	else
	{
		FRotator r1 = GetActorRotation();
		FRotator r2 = FMath::Lerp(r1, m_targetAngle, m_lerpRatio);

		SetActorRotation(r2);
		//UE_LOG(LogTemp, Log, TEXT("Move Yaw : %f"), r2.Yaw);

		if (FMath::Abs(m_targetAngle.Yaw - r1.Yaw) <= 1.0f || FMath::Abs(r2.Yaw - r1.Yaw) < 1.0f)
		{
			SetActorRotation(m_targetAngle);

			if (gamemanager->GetDimensionType() == EDimensionType::ThreeD)
			{
				//m_DirectionCamNumber -= m_rotationDireX;

				//if (m_DirectionCamNumber > 3)
				//{
				//	m_DirectionCamNumber = 0;
				//}
				//if (m_DirectionCamNumber < 0)
				//{
				//	m_DirectionCamNumber = 3;
				//}

				if (gamemanager->GetDimensionType() == EDimensionType::ThreeD)
				{
					gamemanager->DirectionCamNumber -= m_rotationDireX;

					if (gamemanager->DirectionCamNumber > 3)
					{
						gamemanager->DirectionCamNumber = 0;
					}
					if (gamemanager->DirectionCamNumber < 0)
					{
						gamemanager->DirectionCamNumber = 3;
					}
				}
			}

			UE_LOG(LogTemp, Log, TEXT("DirectionCamNumber %d"), gamemanager->DirectionCamNumber);

			m_rotationDireX = 0;
			m_cameraMoving = false;
			m_CameraInput.X = 0.f;
			//UE_LOG(LogTemp, Log, TEXT("m_cameraMoving Is %d"), (int)m_cameraMoving);
		}
	}
}

void AMappingCamera3D::SetCameraRotateInputX()
{
	if (m_CameraInput.X == 1.f)
	{
		m_inputRotationDireX = -1;
		//UE_LOG(LogTemp, Log, TEXT("Input Right"));
	}
	else if (m_CameraInput.X == -1.f)
	{
		m_inputRotationDireX = 1;
		//UE_LOG(LogTemp, Log, TEXT("Input Left"));
	}
	else
	{
		m_inputRotationDireX = 0;
		//UE_LOG(LogTemp, Log, TEXT("Input None"));
	}
}

//void AMappingCamera3D::SetIsPlayerMoving(bool _IsPlayerMoving)
//{
//	m_IsPlayerMoving = _IsPlayerMoving;
//}

bool AMappingCamera3D::GetCameraMoving()
{
	return m_cameraMoving;
}

void AMappingCamera3D::SetCameraInput(FVector2D _Input)
{
	m_CameraInput = _Input;
}

void AMappingCamera3D::SetIsPlayerMoving(bool _IsPlayerMoving)
{
	m_IsPlayerMoving = _IsPlayerMoving;
}