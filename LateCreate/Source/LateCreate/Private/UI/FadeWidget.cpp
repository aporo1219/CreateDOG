// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FadeWidget.h"

void UFadeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsReady = (FadeOut != nullptr);

	if (!FadeOut)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeConstruct: FadeOutAnim NULL"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct: FadeOutAnim OK"));
	}

	UE_LOG(LogTemp, Warning, TEXT("FadeWidget Ready: %d"), bIsReady);
}

void UFadeWidget::PlayFadeOut()
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayFadeOut CALLED"))
	
	if (!FadeOut)
	{
		UE_LOG(LogTemp, Error, TEXT("FadeOutAnim NULL in PlayFadeOut"));
		return;
	}

	UUMGSequencePlayer* Player = PlayAnimation(FadeOut);

	if (Player)
	{
		Player->OnSequenceFinishedPlaying().AddLambda(
			[this](UUMGSequencePlayer& FinishedPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("FadeWidget FadeOut Finished"));
				OnFadeOutFinished.Broadcast();
			}
		);
	}
}

void UFadeWidget::PlayFadeIn()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}