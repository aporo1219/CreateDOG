// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Dog.generated.h"


//�N���X�̐錾
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
	void LockOn(const FInputActionValue& Value);
	//�W�����v
	void Jump(const FInputActionValue& Value);
	//�؂�ւ�
	void Switch(const FInputActionValue& Value);
	//�}�E�X�ړ�
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

private:
	//�e�ۂ̃N���X
	UPROPERTY(EditDefaultsOnly,Category = "Attack")
	TSubclassOf<class ABallActor> BallActorClass;

	//���˃|�C���g
	UPROPERTY(VisibleAnywhere,Category = "Componets")
	USceneComponent* MuzzlePoint;

	//�ړ��ڕW�N���X
   /* UPROPERTY(EditDefaultsOnly,Category = "Move")
	TSubclassOf<class AGoalPoint> GoalPointClass;*/

//�ϐ��錾
private:
	//�X�|�[���ʒu����
	float Spawnlocation = 200.0f;

     //�W�����v���ł��邩����
	bool CanJump = false;

	//�U���A�ړ��؂�ւ�
	bool IsChangeAttack = false;//false�Ȃ�Έړ����[�h

	FVector WorldLocation;
	FVector WorldDirection;
public:
	// �W�����v��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dog|Movement")
	float JumpForce = 1000.0f;

	//���b�N�I���͈̔�
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "LockOn")
	float LockOnRange = 1000.0f;

	//���b�N�I���A�N�^�[
	UPROPERTY(BlueprintReadOnly, Category = "LockOn")
	AActor* LockedTarget = nullptr;
};
