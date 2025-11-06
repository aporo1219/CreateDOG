// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeStageSelect.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AGameModeStageSelect::AGameModeStageSelect()
{
	//プレイヤーを出さない
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void AGameModeStageSelect::BeginPlay()
{
	Super::BeginPlay();

	if (!StageSelectUIClass)
	{
		StageSelectUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/BP/UI/StageSelect_UI.StageSelect_UI_C"));
	}

	//StageSelectUIの表示
	if (StageSelectUIClass)
	{
		StageSelectUI = CreateWidget<UUserWidget>(GetWorld(), StageSelectUIClass);
		if (StageSelectUI)
		{
			StageSelectUI->AddToViewport();

			//入力設定
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
				//プレイヤーの入力を無効
				PC->DisableInput(PC);
			}
		}
	}
}