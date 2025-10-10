// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DogPlayer.generated.h"

//クラスの宣言
class UStaticMeshCOmponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class LATECREATE_API ADogPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADogPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//主人のメッシュ カプセル
	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivaetAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Cube;

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
};
