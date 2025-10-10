// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Dog.generated.h"


//�N���X�̐錾
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
	//��l���̃��b�V�� �J�v�Z��
	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivaetAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Sphere;

	//�J�����z�u�̃X�v�����O�A�[��
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	//�J����
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	//�X�v�����O�A�[���̊p�x�ϐ�
	float RollArm = 0.0f, PitchArm = -30.0f, YawArm = 0.0f;

	//�X�v�����O�A�[���̒��������ϐ�
	float ArmLength = 450.0f;

protected:
	//���_����
	void Look(const FInputActionValue& Value);
	//�U��
	void Attack(const FInputActionValue& Value);
	//�W��
	void Standerd(const FInputActionValue& Value);

private:
	/** Ball��Control���� */
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
	//�e�ۂ̃N���X
	UPROPERTY(EditDefaultsOnly,Category = "Projectile")
	TSubclassOf<class ABallActor> BallActorClass;

	//���˃|�C���g
	UPROPERTY(VisibleAnywhere,Category = "Componets")
	USceneComponent* MuzzlePoint;


};
