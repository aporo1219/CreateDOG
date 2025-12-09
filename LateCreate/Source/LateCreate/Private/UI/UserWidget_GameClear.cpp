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

		// ボタンにフォーカスを当てる
		StageSelectButton->SetKeyboardFocus();

		//バインド
		StageSelectButton->OnClicked.AddDynamic(this, &UUserWidget_GameClear::OnStageSelectCliked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StageSelectButton is NULL!"));
	}
}

//クリックされたときの処理
void UUserWidget_GameClear::OnStageSelectCliked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStageSelectCliked CALLED!!!"));

	// ポーズ解除
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(GetWorld(),TEXT( "StageSelect"));
}
