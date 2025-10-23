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

	//変数宣言
	//スポーンタイマー
	FTimerHandle SpawnTimeHandle;
	UPROPERTY(EditAnywhere,Category = "Spawn")
	float SpawnTime = 5.0f;
};
