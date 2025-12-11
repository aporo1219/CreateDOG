// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeTitle.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AGameModeTitle::AGameModeTitle()
{
	//プレイヤーを出さない
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void AGameModeTitle::BeginPlay()
{
	Super::BeginPlay();

	if (!TitleUIClass)
	{
		//TitleのUIをロード
		TitleUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/BP/UI/Title_UI.Title_UI_C"));
	}

	if (TitleUIClass)
	{
		//TitleUIの表示
		TitleUI = CreateWidget<UUserWidget>(GetWorld(), TitleUIClass);
		if (TitleUI)
		{
			TitleUI->AddToViewport();

			//入力設定
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
				//プレイヤーの入力無効
				PC->DisableInput(PC);
			}
		}
	}
}
