// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Enemy.h"
#include "EnemySpawn.generated.h"

UCLASS()
class LATECREATE_API AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//敵のクラス指定
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Spawn")
	TSubclassOf<AEnemy> EnemeyClass;

	//関数宣言
	//敵スポーン関数
	UFUNCTION(BlueprintCallable,Category = "Spawn")
	void SpawnEnemy();

	//敵死亡チェック関数
	UFUNCTION()
	void OnEnemyKilled(AActor* DeadEnemy);

	/** 全滅後に復活 */
	void RespawnEnemies();

	//スポーンする時の場所制限
	bool FindSafeSpawnLoc(FVector& OutLoc);
	
	//変数宣言
	// スポーン上限
	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 MaxEnemyCount = 5;


	/** 全滅後の復活までの遅延時間（秒） */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float RespawnDelay = 2.f;

	/** 生成した敵の配列 */
	UPROPERTY()
	TArray<AEnemy*> SpawnedEnemies;

	/** 復活用タイマー */
	FTimerHandle RespawnTimerHandle;

private:
	FString StageName;
};
