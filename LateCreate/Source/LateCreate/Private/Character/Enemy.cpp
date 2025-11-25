// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include  "Character/EnemyBullet.h"
#include "Character/Dog.h"
#include "Actor/EnemySpawn.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
   

    EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
    RootComponent = EnemyMesh;

    EnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EnemyMesh->SetCollisionProfileName(TEXT("BlockAllDynamic")); 
    EnemyMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    EnemyMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    EnemyMesh->SetSimulatePhysics(false);
    EnemyMesh->SetEnableGravity(false);

    // Skeletal MeshをC++でセット
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/model/Alien/Alien"));
    if (MeshAsset.Succeeded())
    {
       EnemyMesh->SetSkeletalMesh(MeshAsset.Object);
       UE_LOG(LogTemp, Warning, TEXT("Mesh successfully loaded!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load mesh! Check path."));
    
    }


    MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
    MuzzlePoint->SetupAttachment(EnemyMesh);
    MuzzlePoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f)); // 前方

    FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
    FloatingMovement->UpdatedComponent = RootComponent;

    static ConstructorHelpers::FClassFinder<AEnemyBullet> BulletBP(TEXT("/Game/BP/Enemy/BP_EnemyBullet"));
    if (BulletBP.Succeeded())
    {
        EnemyBulletClass = BulletBP.Class;
    }

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

    // 0.5秒後に初期化処理を実行
    GetWorldTimerManager().SetTimerForNextTick(this, &AEnemy::InitializeEnemy);
    SetActorTickEnabled(true);
  
}


void AEnemy::InitializeEnemy()
{
    ACharacter* DogChara = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    // プレイヤーを取得

    if (!DogChara)
    {
        UE_LOG(LogTemp, Error, TEXT("DogChara is still NULL even after delay!"));
        return;
    }

    //敵の玉の発射間隔をおランダムにさせる
    float FirstFire = FMath::FRandRange(FirstBullet1, FirstBullet2);

    // ここで発射タイマーをセット
    GetWorldTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AEnemy::Fire,
        FirstFire,   // 間隔
        false   // ループ
    );
}
// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

   

    //プレイヤーの取得
    ACharacter* DogChara = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!DogChara)
    {
        return;
    }

    //敵自身とプレイヤーの位置取得
    FVector PlayerLoc = DogChara->GetActorLocation();
    FVector EnemyLoc = GetActorLocation();

    //向きの計算
    FVector ToPlayer = PlayerLoc - EnemyLoc;

    //角度の変換
    FRotator LookRot = ToPlayer.Rotation();
    LookRot.Yaw += EnemyTurnAngle;

    //敵の回転
    SetActorRotation(LookRot);

    //再生されているレベルの取得
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

    //レベルによっての敵の動きの処理
    if (LevelName == "Stage2" || LevelName == "Stage3")
    {
        EnemyMove();
    }
}

// Called to bind functionality to input

//玉発射
void AEnemy::Fire()
{

    ACharacter* DogChara = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!DogChara)
    {
        UE_LOG(LogTemp, Error, TEXT("DogChara is NULL!"));
    }

    if (!EnemyBulletClass)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyBulletClass is NULL!"));
    }

    if (!DogChara || !EnemyBulletClass)
        return;
    //発射位置
    FVector SpawnLoc = MuzzlePoint->GetComponentLocation() + GetActorForwardVector() * 100.f;

    // プレイヤーの位置を補正（腰 or 胸のあたりに狙う）
    FVector TargetLoc = DogChara->GetActorLocation() + FVector(0.f, 0.f, 50.f); // 高さ補正
    // 発射方向を計算
    FVector Direction = (TargetLoc - SpawnLoc).GetSafeNormal();
    FRotator SpawnRot = Direction.Rotation();

    //玉生成
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AEnemyBullet* Bullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulletClass, SpawnLoc, SpawnRot);

    //玉の初速を設定
    if (Bullet)
    {
        FVector ShootDir = (DogChara->GetActorLocation() - SpawnLoc).GetSafeNormal();
        Bullet->InitVelocity(ShootDir);

        //発射元の無視
        Bullet->Mesh->IgnoreActorWhenMoving(this, true);
        Bullet->SetOwner(this);
    }

    //攻撃のクールタイム
    float NextFire = FMath::FRandRange(BulletCoolTime1, BulletCoolTime2);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemy::Fire, NextFire, false);
}

void AEnemy::TakeDamegeAndDie()
{
    //被弾SE
    if (SoundToPlayHit)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayHit, GetActorLocation());
    }

    // Fireタイマーを停止
    GetWorldTimerManager().ClearAllTimersForObject(this);

    // 敵を非表示＆衝突無効にする
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);

    // EnemySpawn に通知
    if (AEnemySpawn* Spawner = Cast<AEnemySpawn>(GetOwner()))
    {
        Spawner->OnEnemyKilled(this);
    }
}

//リスポーン処理
void AEnemy::Respawn()
{
    // 非表示を解除
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    SetActorTickEnabled(true);

    // Fireタイマーを再セット
    float FirstFire = FMath::FRandRange(FirstBullet1, FirstBullet2);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemy::Fire, FirstFire, false);
}

//敵の動き処理
void AEnemy::EnemyMove()
{
    if (!FloatingMovement)
        return;

    float Speed = 200.f;
    FVector CurrentLoc = GetActorLocation();
    FVector MoveDelta = GetActorForwardVector() * Speed * GetWorld()->GetDeltaSeconds();

    // 衝突判定用
    FHitResult Hit;
    SetActorLocation(CurrentLoc + MoveDelta, true, &Hit);

    // 壁にぶつかったらスライド
    if (Hit.bBlockingHit)
    {
        FVector SlideDelta = FVector::VectorPlaneProject(MoveDelta, Hit.Normal);
        SetActorLocation(CurrentLoc + SlideDelta, true);
    }
}
