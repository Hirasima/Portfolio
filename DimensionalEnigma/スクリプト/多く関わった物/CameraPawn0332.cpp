// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPawn0332.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h" // �ǉ�
#include "Kismet/GameplayStatics.h" // �ǉ�
#include "Components/InputComponent.h" // �ǉ�
#include "EnhancedInputComponent.h" // �ǉ�
#include "EnhancedInputSubsystems.h" // �ǉ�
#include "SoundManagerSubsystem.h"
#include "StageSubSystem.h"

// Sets default values
ACameraPawn0332::ACameraPawn0332()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�J�����̏�����
	InitCamera();

	// �����ݒ�
	bool bIsOrthographic = false;
	GetCamera()->SetProjectionMode(ECameraProjectionMode::Perspective);
}

// Called when the game starts or when spawned
void ACameraPawn0332::BeginPlay()
{
	Super::BeginPlay();

	//�J�����̏����p�x�ݒ�
	SetCameraAngle(-35.26f, 315.0f, 0.0f);
	//�ʒu��������
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));

	DefaultCameraPitch = GetActorRotation().Pitch;//�������@��|�ǉ�

	m_lerpRatio = 0.7f;
	//�X�v�����O�A�[���̒����ݒ�
	if (SpringArmLength <= 0.0f)
	{
		SpringArmLength = 1000.0f;
	}
	m_pSpringArm->TargetArmLength = SpringArmLength;

	// GameManager���擾
	world = GetWorld();
	if (world) {
		gameinstance = Cast<UEnigmaGameInstance>(world->GetGameInstance());
		if (gameinstance) {
			gamemanagaer = gameinstance->GetSubsystem<UGameManager>();
		}
		stage = world->GetSubsystem<UStageSubSystem>();
	}

	//�����e�ɂ���
	GetCamera()-> SetProjectionMode(ECameraProjectionMode::Orthographic);

	//-------------------------��|�ǉ�
	//SE���Đ�(����)�󌂂�������
	USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	if (SoundManager)
	{
		// �p�X�ŉ������Đ�
		SoundManager->PlaySoundFromPath(m_CameraRotSE_Path, 0.001f);

		UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
	}
	//-------------------------
}

// Called every frame
void ACameraPawn0332::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetCameraRotateInputX();

	UpdateCamera(DeltaTime);
	
}

// Called to bind functionality to input
void ACameraPawn0332::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//�Ǘ����Ă���J�������擾
UCameraComponent* ACameraPawn0332::GetCamera() const
{
	return m_pCamera;
}

//�Ǘ����Ă���X�v�����O�A�[�����擾
USpringArmComponent* ACameraPawn0332::GetSpringArm() const
{
	return m_pSpringArm;
}

//�Ǘ�����J������ݒ�
void ACameraPawn0332::SetCamera(UCameraComponent* _pCamera)
{
	m_pCamera = _pCamera;
}

//�Ǘ�����X�v�����O�A�[����ݒ�
void ACameraPawn0332::SetSpringArm(USpringArmComponent* _pSpringArm)
{
	m_pSpringArm = _pSpringArm;
}

//�J�����̏�����
void ACameraPawn0332::InitCamera()
{
	if (!m_pSpringArm)
	{
		//�X�v�����O�A�[������
		m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>("m_pSpringArm");
		m_pSpringArm->SetupAttachment(RootComponent);

		//�X�v�����O�A�[���̒����ݒ�
		m_pSpringArm->TargetArmLength = SpringArmLength;
		m_pSpringArm->bDoCollisionTest = false;
	}
	
	if (!m_pCamera)
	{
		//�J��������
		m_pCamera = CreateDefaultSubobject<UCameraComponent>("m_pCamera");
		m_pCamera->SetupAttachment(m_pSpringArm);
		m_pCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	}
}

//�J�����̍X�V
void ACameraPawn0332::UpdateCamera(float _DeltaTime)
{

	//�ړ����o�Ȃ��Ȃ�
	if (!m_cameraMoving)
	{
		//���͂���Ă��鎞
		if (m_inputRotationDireX != 0 && !stage->GetGoaling())
		{
			//��]��������͂���Ă�������ɂ���
			m_rotationDireX = m_inputRotationDireX;
			//�ړI�n�̐ݒ�
			m_targetAngle = GetActorRotation() + FRotator(0.0f, 90.0f * m_rotationDireX, 0.0f);
			//Lerp�Ɏg���������ُ�l�Ȃ�␳
			if (m_lerpRatio <= 0 || m_lerpRatio > 1)
				m_lerpRatio = 0.06f;

			//UE_LOG(LogTemp, Log, TEXT("Start Move Target Yaw : %f"), m_targetAngle.Yaw);

			//�ړ����ɕύX
			m_cameraMoving = true;

			if (stage)
			{
				stage->SetCameraMoving(true);
			}
			//UE_LOG(LogTemp, Log, TEXT("m_cameraMoving Is %d"), (int)m_cameraMoving);
		    
			//-------------------------��|�ǉ�
			USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
			if (SoundManager)
			{
				// �p�X�ŉ������Đ�
				SoundManager->PlaySoundFromPath(m_CameraRotSE_Path);

				UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
			}
			//
		}
		else
		{
			//�t���[�J����

			SetActorRotation(GetActorRotation() - m_FreeCameraRot_temp);//���Z�O�̏�Ԃɖ߂�

			m_BaseCameraRot = GetActorRotation();//��{���W�Ƃ��ċL�^���Ă���

			SetActorRotation(m_BaseCameraRot + m_FreeCameraRot);

			m_FreeCameraRot_temp = m_FreeCameraRot;
		}
	}
	else
	{
		//���݂̉�]��
		FRotator r1 = GetActorRotation();
		//�ړ���̗\�肵�Ă����]��
		FRotator r2 = FMath::Lerp(r1, m_targetAngle, m_lerpRatio * (_DeltaTime * 20.0f));
																		//����|�@�قȂ�FPS�Ђł�����̑��x���ێ�����悤��
		//��]������
		SetActorRotation(r2);

		//��]���Ԍv��
		m_RotTimer += _DeltaTime;

		//�ړI�n�ɂƂĂ��߂��Ȃ������͈ړ��ʂ��������Ȃ肷������  
		if (FMath::Abs(m_targetAngle.Yaw - r1.Yaw) <= 0.01f || FMath::Abs(r2.Yaw - r1.Yaw) < 0.01f ||//��|�@�I�[�Ɉ�a���������邽�ߍX�ɔ͈͂��ׂ���
			m_RotTimer > 0.5f || r1 == GetActorRotation())											 //��|�ǉ����X�Ɉ�莞�Ԍo�߂����� �X�ɉ�]�ʂ�0�ł���Ȃ��
		{
			//�ړI�n�Ɉړ�
			SetActorRotation(m_targetAngle);

			//�l��������
			m_rotationDireX = 0;
			m_cameraMoving = false;
			m_CameraInput.X = 0.f;
			m_RotTimer = 0.0f;//��|�ǉ�

			// �J�����̉�]�񐔂�Manager�ɃJ�E���g������
			if (stage)
			{
				stage->AddCameraMoveCount();
			}

			//UE_LOG(LogTemp, Log, TEXT("m_cameraMoving Is %d"), (int)m_cameraMoving);
		}
	}
}

//�X�v�����O�A�[���̒�����ݒ�
void ACameraPawn0332::SetArmLength(float _length)
{
	if (m_pSpringArm != nullptr)
	{
		m_pSpringArm->TargetArmLength = _length;
	}
}

//�J�����̊p�x��ݒ�
void ACameraPawn0332::SetCameraAngle(float _pitch, float _yaw, float _roll)
{
	SetActorRotation(FRotator(_pitch, _yaw, _roll));
	//UE_LOG(LogTemp, Log, TEXT("Set Camera Rotation"));
	/*FTransform t = GetActorTransform();
	FRotator r = t.Rotator();*/
	/*UE_LOG(LogTemp, Log, TEXT("Set Camera Rotation : p %d y %d r %d"), r.Pitch, r.Yaw, r.Roll);*/
}

//�J�����̊p�x�ɉ��Z
void ACameraPawn0332::AddCameraAngle(float _pitch, float _yaw, float _roll)
{
	AddActorWorldRotation(FRotator(_pitch, _yaw, _roll));
}

//���͒l����
void ACameraPawn0332::SetCameraRotateInputX()
{
	if (m_CameraInput.X == 1.f)
	{
		m_inputRotationDireX = -1;
		//UE_LOG(LogTemp, Log, TEXT("Input Right"));
	}
	else if (m_CameraInput.X == -1.f)
	{
		m_inputRotationDireX =  1;
		//UE_LOG(LogTemp, Log, TEXT("Input Left"));
	}
	else
	{
		m_inputRotationDireX = 0;
		//UE_LOG(LogTemp, Log, TEXT("Input None"));
	}
}

//String�^�̃Z�[�u�f�[�^���擾
FString ACameraPawn0332::GetSaveDataToString()
{
	return FString("");
}
//String�^�̃Z�[�u�f�[�^��ݒ�
void ACameraPawn0332::SetSaveDataToString(const FString _saveData)
{

}

void ACameraPawn0332::SetupInput()
{

}

void ACameraPawn0332::PressedCamera(const FInputActionValue& Value)
{

}

void ACameraPawn0332::SetCameraInput(FVector2D _Input)
{
	m_CameraInput = _Input;
}

void ACameraPawn0332::SetIsPlayerMoving(bool _IsPlayerMoving)
{
	m_IsPlayerMoving = _IsPlayerMoving;
}

//-------------------------------��|���ǉ������֐�
void ACameraPawn0332::SetCameraAngle(float _Roll)
{
	FRotator CameraRot = GetActorRotation();
	CameraRot.Pitch = -_Roll+ DefaultCameraPitch;
	SetActorRotation(CameraRot);
}
void ACameraPawn0332::SetTargetCameraRoll()
{

}

//������
void ACameraPawn0332::SetCameraMode(bool mode)
{
	if (mode)
	{
		GetCamera()->SetProjectionMode(ECameraProjectionMode::Perspective);
		m_CameraMode = true;

		//�X�v�����O�A�[���̒����ݒ�
		SpringArmLength = 300.0f;
		m_pSpringArm->TargetArmLength = SpringArmLength;
	}
	else
	{
		GetCamera()->SetProjectionMode(ECameraProjectionMode::Orthographic);
		m_CameraMode = false;

		//�X�v�����O�A�[���̒����ݒ�
		SpringArmLength = 1000.0f;
		m_pSpringArm->TargetArmLength = SpringArmLength;
	}


}


void ACameraPawn0332::SetFreeCameraRot(FRotator _Rot)
{
	m_FreeCameraRot = _Rot;
}
FRotator ACameraPawn0332::GetFreeCameraRot()
{
	return m_FreeCameraRot;
}

//-------------------------------