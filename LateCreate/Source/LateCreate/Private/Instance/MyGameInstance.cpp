// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/MyGameInstance.h"
#include "UI/FadeWidget.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance Init"));

	if (FadeWidgetClass)
	{
		//フェードクラスを追加
		FadeWidget = CreateWidget<UFadeWidget>(this, FadeWidgetClass);
		if (FadeWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("FadeWidget Created"));
			FadeWidget->AddToViewport(FADE_VIEW);
		}
	}
}

void UMyGameInstance::FadeOpenLevel(FName LevelName)
{
	UE_LOG(LogTemp, Warning, TEXT("FadeOpenLevel CALLED"));

//UE_LOG(LogTemp, Error, TEXT("FadeWidget not ready"));
		UGameplayStatics::OpenLevel(this, LevelName);
		//return;
	

	PendingLevelName = LevelName;

	FadeWidget->OnFadeOutFinished.Clear();
	FadeWidget->OnFadeOutFinished.AddDynamic(
		this,
		&UMyGameInstance::OnFadeOutFinished
	);

	FadeWidget->PlayFadeOut();
}

void UMyGameInstance::OnFadeOutFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("OnFadeOutFinished CALLED"));

	if (!PendingLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, PendingLevelName);
	}
}



