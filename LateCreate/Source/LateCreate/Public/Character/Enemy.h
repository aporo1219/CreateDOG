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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* EnemyMesh;

	//移動
	UPROPERTY(VisibleAnywhere,Category = "Movement")
	class UFloatingPawnMovement* FloatingMovement;
	//玉の発射位置
	UPROPERTY(VisibleAnywhere,Category = "Attack")
	USceneComponent* MuzzlePoint;
	// ルートコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	//発射するクラス
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<class AEnemyBullet> EnemyBulletClass;

	//玉の発射関数
	void Fire();

	//敵の移動関数
	void EnemyMove();

	UFUNCTION()
	void InitializeEnemy();

public:
	FTimerHandle FireTimerHandle;

	//変数宣言
	float FirstBullet1 = 1.0f;
	float FirstBullet2 = 3.0f;
	float BulletCoolTime1 = 5.0f;
	float BulletCoolTime2 = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayHit;

	//関数宣言
	void TakeDamegeAndDie();

	void Respawn();

	void StartBlinkAndDie();//点滅開始関数

	void StartBlinkAndDamege();

	void Blink();//点滅処理関数

	void DamageBlink();

	void TakeDamege(float damege);//ダメージ処理

	void EndInvinible();
private:
	//変数宣言
	float EnemyTurnAngle = 270.0f;
	//ダメージ時の点滅変数
	FTimerHandle BlinkTimerHandle;
	FTimerHandle DieTimerHandle;

	bool bIsVisible = true;

	int BlinkCount = 0;
	const int MaxBlinkCount = 6;//３回点滅

	float BlinkInterval = 0.1f;

	bool bBlinkMidWay;//点滅中か

	bool bDie;//死亡中か

	bool bScoreChenge;

	float  enemy_HP{ 20 };
	int32 killscore = 100;

	//無敵中か
	bool bIsInvincible{ false };

	//無敵時間
	UPROPERTY(EditAnywhere,Category = "Enemy")
	float InvincibleTime{ 1.0f };

	//無敵タイマー
	FTimerHandle InvincibleTimerHandle;

	bool bDamageBlinking = false;
	int32 DamageBlinkCount = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxDamageBlinkCount = 6;

	UPROPERTY(EditAnywhere)
	float DamageBlinkInterval = 0.1f;

	FTimerHandle DamageBlinkTimerHandle;
};
