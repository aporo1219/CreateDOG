// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include  "Character/EnemyBullet.h"
#include "Character/Dog.h"
#include "Actor/EnemySpawn.h"
#include "GameMode/MyGameModeBase.h"
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

    //変数の初期化--------------------
    bBlinkMidWay = false;
    bDie = false;
    bScoreChenge = false;
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
    if (LevelName == "Stage2" || LevelName == "Stage3"|| LevelName == "Stage4")
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

//ダメージ処理
void AEnemy::TakeDamege(float damege)
{
    // 無敵中ならダメージ無効
    if (bIsInvincible)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Enemy Hit"));

    // HP減少
    enemy_HP -= damege;
    if (enemy_HP < 0)
    {
        enemy_HP = 0;
    }

    // HP0なら死亡処理へ
    if (enemy_HP <= 0)
    {
        bDie = true;
        StartBlinkAndDie();
        return;
    }

    // 無敵開始
    bIsInvincible = true;

    StartBlinkAndDamege();

    // 無敵解除タイマー
    GetWorldTimerManager().SetTimer(
        InvincibleTimerHandle,
        this,
        &AEnemy::EndInvinible,
        InvincibleTime,
        false
    );
}

void AEnemy::TakeDamegeAndDie()
{
    if (bScoreChenge)
        return;

    bScoreChenge = true;

    // スコア加算
    if (AMyGameModeBase* GM =
        Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this)))
    {
        GM->AddEnemyKillScore(killscore);
    }
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
    //状態リセット
    bDie = false;
    bBlinkMidWay = false;
    bScoreChenge = false;
    BlinkCount = 0;
    bIsVisible = true;
    bIsInvincible = false;
    bDamageBlinking = false;
    DamageBlinkCount = 0;
    enemy_HP = maxHP;

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

//点滅開始関数
void AEnemy::StartBlinkAndDie()
{
    if (bBlinkMidWay)
        return;

    bBlinkMidWay = true;

    //倒れてる処理中なら無視
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

    if (EnemyMesh)
    {
        EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    BlinkCount = 0;
    bIsVisible = true;

    //0.1秒ごとに点滅
    GetWorldTimerManager().SetTimer(
        BlinkTimerHandle,
        this,
        &AEnemy::Blink,
        BlinkInterval,
        true
    );
}

//点滅処理
void AEnemy::Blink()
{
    bIsVisible = !bIsVisible;
    SetActorHiddenInGame(!bIsVisible);

    BlinkCount++;

    if (BlinkCount >= MaxBlinkCount)
    {
        GetWorldTimerManager().ClearTimer(BlinkTimerHandle);
        bBlinkMidWay = false;
        TakeDamegeAndDie();
    }
}

void AEnemy::EndInvinible()
{
    bIsInvincible = false;

    if (!bDie && EnemyMesh)
    {
        EnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    // 念のため表示を戻す
    SetActorHiddenInGame(false);
}

void AEnemy::StartBlinkAndDamege()
{
    if (bDamageBlinking)
        return;

    bDamageBlinking = true;
    DamageBlinkCount = 0;
    bIsVisible = true;

    GetWorldTimerManager().SetTimer(
        DamageBlinkTimerHandle,
        this,
        &AEnemy::DamageBlink,
        DamageBlinkInterval,
        true
    );
}

void AEnemy::DamageBlink()
{
    bIsVisible = !bIsVisible;
    SetActorHiddenInGame(!bIsVisible);

    DamageBlinkCount++;

    if (DamageBlinkCount >= MaxDamageBlinkCount)
    {
        GetWorldTimerManager().ClearTimer(DamageBlinkTimerHandle);
        bDamageBlinking = false;
        SetActorHiddenInGame(false);
    }
}