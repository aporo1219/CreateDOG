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
	//UE_LOG(LogTemp, Warning, TEXT("PlayFadeOut CALLED"));
	//
	//BindToAnimationFinished(
	//	FadeOut,
	//	FWidgetAnimationDynamicEvent::CreateUFunction(
	//		this,
	//		FName("HandleFadeOutFinished")
	//	)
	//);

	if (FadeOut)
	{
		PlayAnimation(FadeOut);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FadeOutAnim NULL in PlayFadeOut"));
	}
}

void UFadeWidget::PlayFadeIn()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}

void UFadeWidget::HandleFadeOutFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("FadeWidget FadeOut Finished"));
	OnFadeOutFinished.Broadcast();
}