// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupArea.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

#include "PopupUISubSystem.h"

APopupArea::APopupArea()
{
    PrimaryActorTick.bCanEverTick = true;

    // 球状コリジョンの設定
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->InitSphereRadius(50.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APopupArea::OnActorBeginOverlap);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APopupArea::OnActorEndOverlap);

    // メッシュコンポーネントの設定
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    // デフォルトメッシュの適用
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (DefaultMesh.Succeeded())
    {
        StaticMeshComponent->SetStaticMesh(DefaultMesh.Object);
    }
}

// Called when the game starts or when spawned
void APopupArea::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APopupArea::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsOverlapping)
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlapping..."));
    }
}

// Called to bind functionality to input
void APopupArea::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 触れた瞬間の処理
void APopupArea::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(TargetActorClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnCollision (Enter)"));
        bIsOverlapping = true;

        //UIを表示
        UPopupUISubSystem* PopupUI = GetGameInstance()->GetSubsystem<UPopupUISubSystem>();
        if (PopupUI)
        {
            // パスでUIを表示
            PopupUI->ShowPopupUI(WidgetClass);

            UE_LOG(LogTemp, Warning, TEXT("ShowPopup!"))
        }
    }
}

// 離れた瞬間の処理
void APopupArea::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(TargetActorClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnCollision (Exit)"));
        bIsOverlapping = false;

        //UIを表示
        UPopupUISubSystem* PopupUI = GetGameInstance()->GetSubsystem<UPopupUISubSystem>();
        if (PopupUI)
        {
            // パスでUIを表示
            PopupUI->HidePopupUI();

            UE_LOG(LogTemp, Warning, TEXT("ClosePopup!"))
        }
    }
}

