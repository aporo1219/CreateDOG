// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Score.h"
#include "Components/TextBlock.h"
#include "Character/Dog.h"
#include "GameMode/MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UScore :: NativeConstruct()
{
	Super::NativeConstruct();

	if (TextHealth)
	{
		//バインド
		TextHealth->TextDelegate.BindUFunction(this, "SetTextHealth");
	}
	if (TextMaxHealth)
	{
		//バインド
		TextMaxHealth->TextDelegate.BindUFunction(this, "SetTextMaxHealth");
	}
	if (TextTime)
	{
		//バインド
		TextTime->TextDelegate.BindUFunction(this, "SetTextTime");
	}
	if (TextScore)
	{
		//バインド
		TextScore->TextDelegate.BindUFunction(this, "SetTextScore");
	}
	// Dogを取得
	if (ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		// イベントにバインド
		Dog->OnHealthChanged.AddDynamic(this, &UScore::UpdateHealthText);

		// 初期表示
		UpdateHealthText();

		
	}
	if (AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// イベントにバインド
		GM->OnScoreChanged.AddDynamic(this, &UScore::UpdateScoreText);

		
		UpdateScoreText();

	}
	
}



FText UScore::SetTextHealth()
{
	//dogの取得
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		//体力の設定
		return FText::FromString(FString::FromInt((int)Dog->GetHealth()));
	}

	return FText();
}

FText UScore::SetTextMaxHealth()
{
	//dogの取得
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		//体力の設定
		return FText::FromString(FString::FromInt((int)Dog->GetHealthMax()));
	}


	return FText();
}


FText UScore::SetTextTime()
{
	//GameModeBaseの取得
	if (const AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// 制限時間の取得（変数に直接アクセス or Getter関数）
		return FText::FromString(FString::FromInt((float)GameMode->GetRemainingTime()));
	}

	return FText();
}

//１０秒以内になったら点滅させる処理の関数
bool UScore::IsTimeLow() const
{
	if (const AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{

		return GameMode->GetRemainingTime() <= RedTimeText;
	}
	return false;
}

//スコア
FText UScore::SetTextScore()
{
	if (const AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// 制限時間の取得（変数に直接アクセス or Getter関数）
		return FText::FromString(FString::FromInt((float)GameMode->GetScore()));
	}


	return FText();
}

//体力UIの更新
void UScore::UpdateHealthText()
{
	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (TextHealth)
		{
			TextHealth->SetText(FText::FromString(FString::FromInt((int)Dog->GetHealth())));
		}
		if (TextMaxHealth)
		{
			TextMaxHealth->SetText(FText::FromString(FString::FromInt((int)Dog->GetHealthMax())));
		}
	}
}

//スコアUIの更新
void UScore::UpdateScoreText()
{
	AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM && TextScore)
	{
		TextScore->SetText(FText::AsNumber(GM->Score));
	}
}
