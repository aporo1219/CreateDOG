// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnemySpawn.h"
#include "Character/Enemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
	// BeginPlayでタイマーを開始し、繰り返しSpawnEnemyを呼び出す
    SpawnEnemy();
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //再生されていlevelの取得
    StageName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

    if (StageName == "stage1")
    {
        MaxEnemyCount = 3;
    }
    else if (StageName == "stage2"||StageName == "stage3")
    {
        MaxEnemyCount = 4;
    }
    else if (StageName == "stage4")
    {
        MaxEnemyCount = 5;
    } 
}

void AEnemySpawn::SpawnEnemy()
{
	
    for (int32 i = 0; i < MaxEnemyCount; i++)
    {
        FVector SpawnLoc;
        if (!FindSafeSpawnLoc(SpawnLoc))
        {
            UE_LOG(LogTemp, Warning, TEXT("Safe spawn location not found"));
            continue;
        }

        FRotator SpawnRot = GetActorRotation();

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(
            EnemeyClass,
            SpawnLoc,
            SpawnRot,
            Params
        );

        if (NewEnemy)
        {
            NewEnemy->SetOwner(this);
            SpawnedEnemies.Add(NewEnemy);
        }
    }
}

// 敵が倒されたら呼ばれる
void AEnemySpawn::OnEnemyKilled(AActor* DeadEnemy)
{
    // 現在の敵をチェック（非表示も含める場合はタグなどで管理）
    bool bAnyVisible = false;

    for (AActor* EnemyActor : SpawnedEnemies)
    {
        if (EnemyActor && !EnemyActor->IsHidden())
        {
            bAnyVisible = true;
            break;
        }
    }

    if (!bAnyVisible)
    {
        // 全滅したら一定時間後に復活
        GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemySpawn::RespawnEnemies, RespawnDelay, false);
    }
}

void AEnemySpawn::RespawnEnemies()
{
   for (AEnemy* Enemy : SpawnedEnemies)
   {
      if (Enemy)
      {
          // ランダムに位置をずらす場合
          FVector BaseLoc = GetActorLocation();
          float RandX = FMath::FRandRange(-2000.f, 2000.f);
          float RandY = FMath::FRandRange(-2000.f, 2000.f);
          float RandZ = FMath::FRandRange(0.f, 200.f);
          Enemy->SetActorLocation(BaseLoc + FVector(RandX, RandY, RandZ));

          // 復活処理
          Enemy->Respawn();
      }
   }
}

//安全な場所指定
bool AEnemySpawn::FindSafeSpawnLoc(FVector& OutLoc)
{
    const float Radius = 50.f;   // 敵の半径
    const float HalfHeight = 80.f; // 敵の高さ

    FCollisionShape Capsule = FCollisionShape::MakeCapsule(Radius, HalfHeight);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    for (int32 i = 0; i < 10; i++) // 最大10回トライ
    {
        FVector TestLoc = GetActorLocation() + FVector(
            FMath::FRandRange(-2000.f, 2000.f),
            FMath::FRandRange(-2000.f, 2000.f),
            FMath::FRandRange(50.f, 200.f)
        );

        bool bHit = GetWorld()->OverlapBlockingTestByChannel(
            TestLoc,
            FQuat::Identity,
            ECC_WorldStatic,
            Capsule,
            Params
        );

        if (!bHit)
        {
            OutLoc = TestLoc;
            return true;
        }
    }

    return false; // 見つからなかった
}


