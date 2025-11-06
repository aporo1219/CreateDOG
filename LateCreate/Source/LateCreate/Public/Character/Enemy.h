// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Character/EnemyBullet.h"
#include "Sound/SoundBase.h"
#include "Enemy.generated.h"

UCLASS()
class LATECREATE_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//モデル
	UPROPERTY(VisibleAnywhere,Category = "Components")
	UStaticMeshComponent* EnemyMesh;
	//移動
	UPROPERTY(VisibleAnywhere,Category = "Movement")
	class UFloatingPawnMovement* FloatingMovement;
	//玉の発射位置
	UPROPERTY(VisibleAnywhere,Category = "Attack")
	USceneComponent* MuzzlePoint;

	//発射するクラス
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<class AEnemyBullet> EnemyBulletClass;

	//玉の発射関数
	void Fire();

	UFUNCTION()
	void InitializeEnemy();

public:
	FTimerHandle FireTimerHandle;

	//変数宣言
	float FirstBullet1 = 2.0f;
	float FirstBullet2 = 4.0f;
	float BulletCoolTime1 = 5.0f;
	float BulletCoolTime2 = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayHit;

	//関数宣言
	void TakeDamegeAndDie();

	void Respawn();
};
