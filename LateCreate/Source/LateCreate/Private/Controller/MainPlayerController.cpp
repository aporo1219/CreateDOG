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

	// LocalPlayer が取得できるかチェック
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		//UE_LOG(LogTemp, Error, TEXT("LocalPlayer がまだ存在しません"));
		return;
	}

	// Enhanced Input サブシステムを取得
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (Subsystem)
	{
		if (MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
			//UE_LOG(LogTemp, Log, TEXT("マッピング追加成功"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("MappingContext が未設定です"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("EnhancedInput サブシステムが取得できません"));
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

//切り替え処理の関数
void AMainPlayerController::SwitchPawn(const FInputActionValue& Value)
{
	if (!DogPawn || !PetOwerPawn)
	{
		//UE_LOG(LogTemp, Warning, TEXT("DogPawn または AlienPawn が設定されていません"));
		return;
	}

	if (IsDog)
	{
		Possess(PetOwerPawn);
		IsDog = false;
		UE_LOG(LogTemp, Log, TEXT("宇宙人ポーンを操作中"));
	}
	else
	{
		Possess(DogPawn);
		IsDog = true;
		UE_LOG(LogTemp, Log, TEXT("犬ポーンを操作中"));
	}
}