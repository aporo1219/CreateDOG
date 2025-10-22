// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Score.h"
#include "Components/TextBlock.h"
#include "Character/Dog.h"
#include "Kismet/GameplayStatics.h"

void UScore :: NativeConstruct()
{
	Super::NativeConstruct();
}

bool UScore::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
		return false;

	//ƒoƒCƒ“ƒh
	TextHealth->TextDelegate.BindUFunction(this, "SetTextHealth");
	TextMaxHealth->TextDelegate.BindUFunction(this, "SetTextMaxHealth");
	TextTime->TextDelegate.BindUFunction(this, "SetTextTime");
	TextRemainEnemy->TextDelegate.BindUFunction(this, "SetTextRemainenemy");
	TextScore->TextDelegate.BindUFunction(this, "SetTextScore");

	return true;
}

FText UScore::SetTextHealth()
{
	//dog‚ÌŽæ“¾
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		//‘Ì—Í‚ÌÝ’è
		return FText::FromString(FString::FromInt((int)Dog->GetHealth()));
	}

	return FText();
}

FText UScore::SetTextMaxHealth()
{
	//dog‚ÌŽæ“¾
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		//‘Ì—Í‚ÌÝ’è
		return FText::FromString(FString::FromInt((int)Dog->GetHealthMax()));
	}

	return FText();
}

FText UScore::SetTextTime()
{
	//dog‚ÌŽæ“¾
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		//‘Ì—Í‚ÌÝ’è
		return FText::FromString(FString::FromInt((int)Dog->GetHealth()));
	}

	return FText();
}

FText UScore::SetTextRemainEnemy()
{
	//dog‚ÌŽæ“¾
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		//‘Ì—Í‚ÌÝ’è
		return FText::FromString(FString::FromInt((int)Dog->GetHealthMax()));
	}

	return FText();
}
FText UScore::SetTextScore()
{
	//dog‚ÌŽæ“¾
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		//‘Ì—Í‚ÌÝ’è
		return FText::FromString(FString::FromInt((int)Dog->GetHealth()));
	}

	return FText();
}
