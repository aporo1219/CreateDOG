// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_GameClear.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/MyGameModeBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUserWidget_GameClear::NativeConstruct()
{
	Super::NativeConstruct();

	// ゲームモードを取得してスコアを取得
	if (const AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (TextScore)
		{
			FString ScoreString = FString::Printf(TEXT("SCORE : %d"), GameMode->GetScore());
			TextScore->SetText(FText::FromString(ScoreString));
		}
	}

	if (StageSelectButton)
	{
		//バインド
		StageSelectButton->OnClicked.AddDynamic(this, &UUserWidget_GameClear::OnStageSelectCliked);
	}
}

void UUserWidget_GameClear::OnStageSelectCliked()
{
	// ポーズ解除
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(GetWorld(),TEXT( "StageSelect"));
}
