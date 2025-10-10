// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// LocalPlayer ���擾�ł��邩�`�F�b�N
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		//UE_LOG(LogTemp, Error, TEXT("LocalPlayer ���܂����݂��܂���"));
		return;
	}

	// Enhanced Input �T�u�V�X�e�����擾
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (Subsystem)
	{
		if (MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
			//UE_LOG(LogTemp, Log, TEXT("�}�b�s���O�ǉ�����"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("MappingContext �����ݒ�ł�"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("EnhancedInput �T�u�V�X�e�����擾�ł��܂���"));
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (SwitchAction)
		{
			EIC->BindAction(SwitchAction, ETriggerEvent::Started, this, &AMainPlayerController::SwitchPawn);
		}
	}
}

//�؂�ւ������̊֐�
void AMainPlayerController::SwitchPawn(const FInputActionValue& Value)
{
	if (!DogPawn || !PetOwerPawn)
	{
		//UE_LOG(LogTemp, Warning, TEXT("DogPawn �܂��� AlienPawn ���ݒ肳��Ă��܂���"));
		return;
	}

	if (IsDog)
	{
		Possess(PetOwerPawn);
		IsDog = false;
		UE_LOG(LogTemp, Log, TEXT("�F���l�|�[���𑀍쒆"));
	}
	else
	{
		Possess(DogPawn);
		IsDog = true;
		UE_LOG(LogTemp, Log, TEXT("���|�[���𑀍쒆"));
	}
}