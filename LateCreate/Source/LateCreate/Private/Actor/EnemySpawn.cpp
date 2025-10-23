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
	GetWorldTimerManager().SetTimer(SpawnTimeHandle, this, &AEnemySpawn::SpawnEnemy, SpawnTime, true);
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawn::SpawnEnemy()
{
	if (EnemeyClass)
	{
		//スポーン位置
		FVector SpawnLoc = GetActorLocation();
		FRotator SpawnRot = GetActorRotation();


		// 敵をワールドにスポーンさせる
		GetWorld()->SpawnActor<AEnemy>(EnemeyClass, SpawnLoc, SpawnRot);
	}
}

