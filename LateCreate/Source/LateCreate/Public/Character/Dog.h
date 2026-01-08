// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Sound/SoundBase.h"
#include <Components/SpotLightComponent.h>
#include "Dog.generated.h"

//クラスの宣言
class UStaticMeshCOmponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;


UCLASS()
class LATECREATE_API ADog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADog();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//主人公のメッシュ カプセル
	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivaetAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Sphere;

	//カメラ配置のスプリングアーム
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	//カメラ
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	

	//スプリングアームの角度変数
	float RollArm = 0.0f, PitchArm = -30.0f, YawArm = 0.0f;

	//スプリングアームの長さ調整変数
	float ArmLength = 450.0f;

protected:
	//視点操作
	void Look(const FInputActionValue& Value);
	//攻撃
	void Attack(const FInputActionValue& Value);
	//標準
	void LockOn(const FInputActionValue& Value);
	//ジャンプ
	void Jump(const FInputActionValue& Value);
	//切り替え
	void Switch(const FInputActionValue& Value);
	//マウス移動
	void MoveToMousePoint(const FInputActionValue& Value);
private:

	//LookInputAction
	UPROPERTY(EditAnywhere,Category = Input,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	//AttackInputAction
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction>AttackAction;

	//StanderdInputAction
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction>StanderdAction;

	//JumpInputAction
	UPROPERTY(EditAnywhere,Category = Input,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	//SwitchInputAction
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwitchAction;

	//ClickInputAction
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ClickAction;

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	//スポットライトをコンポーネント
	UPROPERTY(VisibleAnywhere, Category = "Light")
	USpotLightComponent* SpotLight;
private:
	//弾丸のクラス
	UPROPERTY(EditDefaultsOnly,Category = "Attack")
	TSubclassOf<class ABallActor> BallActorClass;

	//発射ポイント
	UPROPERTY(VisibleAnywhere,Category = "Componets")
	USceneComponent* MuzzlePoint;

	//移動目標クラス
    UPROPERTY(EditDefaultsOnly,Category = "Move")
	TSubclassOf<AActor> MoveGoalPointClass;

	UPROPERTY()
	AActor* CurrentMoveMarker;


//変数宣言
private:
	//スポーン位置調整
	float Spawnlocation = 200.0f;

     //ジャンプができるか判定
	bool CanJump = false;

	//攻撃、移動切り替え
	bool IsChangeAttack = false;//falseならば移動モード

	FVector WorldLocation;
	FVector WorldDirection;

	//meshの大きさ補正
	float MeshScale = 0.3f;

	//角度補正
	float AngleCon = 90.0f;

	//速度
	float Speed = 300.0f;

	//体力
	float Health = 100.0f;

	//最大体力
	float MaxHealth = 100.0f;

	//ジャンプSE制御変数
	float JumpSECheck = false;

	//ステージの取得
	FString StageName;
	//ライトの位置補正
	const FVector SpotLightPos = { 0.f,0.f,200.0f };
	const FRotator SpotLightRot = { -75.0f,0.f,0.f };

	//移動目標地点
	FVector MoveTargetLoc;

	//到着目標判定の距離
	UPROPERTY(EditAnywhere,Category = "Move")
	float GoalLocRange = 250.0f;

	//関数宣言
	//ゲームオーバー関数
	void GameOver();

	UPROPERTY(EditAnywhere,Category = "Sound")
	USoundBase* SoundToPlayJump;

	UPROPERTY(EditAnywhere,Category = "Sound")
	USoundBase* SoundToPlayShot;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayChenge;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayHitByEnemy;

public:
	// ジャンプ力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dog|Movement")
	float JumpForce = 1000.0f;

	//ロックオンの範囲
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "LockOn")
	float LockOnRange = 1000.0f;

	// 打った時のスコア	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 ShootScore = 10;

	//ロックオンアクター
	UPROPERTY(BlueprintReadOnly, Category = "LockOn")
	AActor* LockedTarget = nullptr;
	
	//関数宣言
    float GetHealth() const { return Health; }
    float GetHealthMax() const { return MaxHealth; }


	//ダメージ処理関数
	UFUNCTION()
	void TakeDamege(float DamegeAmount);

	// 体力変更時のイベント
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHealthChanged OnHealthChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreChanged);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnScoreChanged OnScoreChanged;
	//クラス宣言
// 補助コリジョン用の Capsule
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* HeadCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* FrontLeftLegCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* FrontRightLegCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* BackLeftLegCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* BackRightLegCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> GameOverClass; 


};
