// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Dog.generated.h"


//クラスの宣言
class UStaticMeshCOmponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;


UCLASS()
class LATECREATE_API ADog : public APawn
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
	void Standerd(const FInputActionValue& Value);

private:
	/** BallをControlする */
	void ControlBall(const FInputActionValue& Value);

	//LookInputAction
	UPROPERTY(EditAnywhere,Category = Input,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	//AttackInputAction
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction>AttackAction;

	//StanderdInputAction
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction>StanderdAction;

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

private:
	//弾丸のクラス
	UPROPERTY(EditDefaultsOnly,Category = "Projectile")
	TSubclassOf<class ABallActor> BallActorClass;

	//発射ポイント
	UPROPERTY(VisibleAnywhere,Category = "Componets")
	USceneComponent* MuzzlePoint;


};
