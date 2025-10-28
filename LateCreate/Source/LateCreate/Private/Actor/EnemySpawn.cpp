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

}

void AEnemySpawn::SpawnEnemy()
{
	
        int32 NumToSpawn = MaxEnemyCount;

        for (int32 i = 0; i < MaxEnemyCount; i++)
        {
            FVector SpawnLoc = GetActorLocation() + FVector(
                FMath::FRandRange(-2000.f, 2000.f),
                FMath::FRandRange(-2000.f, 2000.f),
                FMath::FRandRange(0.f, 200.f)
            );

            FRotator SpawnRot = GetActorRotation();

            AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemeyClass, SpawnLoc, SpawnRot);
            if (NewEnemy)
            {
                NewEnemy->SetOwner(this); // Spawn通知用
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


