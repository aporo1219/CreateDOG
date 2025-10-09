// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainPlayerController.generated.h"

//�N���X�錾
class UInputMappingContext;
class UInputAction;
class APawn;

/**
 * 
 */
UCLASS()
class LATECREATE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
public:
	// Enhanced Input �ݒ�
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	UInputAction* SwitchAction;

	//�؂�ւ��Ώ�
	UPROPERTY(EditAnywhere,Category = "Pawn")
	APawn* DogPawn;

	UPROPERTY(EditAnywhere,Category = "Pawn")
	APawn* PetOwerPawn;

private:
	bool IsDog = true;
	void SwitchPawn(const FInputActionValue& V);
};
