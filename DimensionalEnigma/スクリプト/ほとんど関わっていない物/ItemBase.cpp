#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

#include "ItemGameInstance.h"
#include "ItemSubsystem.h"
#include "ItemManager.h"
#include "SoundManagerSubsystem.h"

AItemBase::AItemBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // 球状コリジョンの設定
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->InitSphereRadius(50.0f);  // 半径を50に設定
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnActorBeginOverlap);

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

void AItemBase::BeginPlay()
{
    Super::BeginPlay();

    //回転量をセット
    RotateAmount = RotateAmount_Idol;

    if (!StaticMeshComponent->GetStaticMesh())
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
        if (DefaultMesh.Succeeded())
        {
            StaticMeshComponent->SetStaticMesh(DefaultMesh.Object);
        }
    }

    UWorld* world = GetWorld();
    if (world)
    {
        UItemManager* itemMng =  world->GetSubsystem<UItemManager>();
        if (itemMng)
        {
            itemMng->RegisterItem(this);
        }
    }

    
}

void AItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 現在の回転を取得
    FRotator CurrentRotation = GetActorRotation();

    // 回転を定義
    FRotator NewRotation = CurrentRotation + FRotator(0.0f, RotateAmount * DeltaTime, 0.0f);

    // アクターの回転を更新
    SetActorRotation(NewRotation);


    //取得後の処理
    if (IsGetting)
    {
        //現在の座標を取得
        FVector Pos = GetActorLocation();
        FVector NewPos = Pos + FVector(0.0f, 0.0f, MovementY);

        //アクターの座標を更新
        SetActorLocation(NewPos);

        //重力を与える
        MovementY -= GravityScale * DeltaTime;


        Counter += DeltaTime;
    }
    else
    {
        //削除する高さをセット(高さが変わることがあるため常に監視)
        m_DeleteHeight = GetActorLocation().Z;
    }
    //一定に達したら削除
    if ( m_DeleteHeight - 5.0f > GetActorLocation().Z)
    {
        Destroy();
    }
}

void AItemBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AItemBase::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(TargetActorClass))
    {
        if (!IsGetting)
        {
            UE_LOG(LogTemp, Warning, TEXT("OnCollision"));
            //ここで情報を送る
            UItemSubsystem* ItemSubsystem = GetGameInstance()->GetSubsystem<UItemSubsystem>();
            if (ItemSubsystem)
            {
                ItemSubsystem->SetItemFlag(ItemID, true); // 取得状態に
                ItemSubsystem->GetCount += 1;//取得数を加算
                GetWorld()->GetSubsystem<UItemManager>()->OnGetItem(this);

                UE_LOG(LogTemp, Warning, TEXT("GetCount: %d"), ItemSubsystem->GetCount)
            }

            //SEを再生
            USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
            if (SoundManager)
            {
                // パスで音源を再生
                SoundManager->PlaySoundFromPath(SE_Path);

                UE_LOG(LogTemp, Warning, TEXT("PlaySE"))
            }

            IsGetting = true;
            //回転量をセット
            RotateAmount = RotateAmount_Out;
        }
    }
}

int AItemBase::GetItemID()
{
    return ItemID;
}