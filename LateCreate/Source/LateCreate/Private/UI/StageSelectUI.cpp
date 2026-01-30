// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Instance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UStageSelectUI::NativeConstruct()
{
   Super::NativeConstruct();

	//ボタンをまとめる
	StageButtons = { Stage1Button,Stage2Button,Stage3Button };


	//ステージ名の設定
	StageNames = { "stage1","Stage2","Stage3","Stage4" };

	//各ボタンに登録
	if (Stage1Button)
		Stage1Button->OnClicked.AddDynamic(this, &UStageSelectUI::OnStage1Clicked);
	if (Stage2Button)
		Stage2Button->OnClicked.AddDynamic(this, &UStageSelectUI::OnStage2Clicked);
	if (Stage3Button)
		Stage3Button->OnClicked.AddDynamic(this, &UStageSelectUI::OnStage3Clicked);

}

void UStageSelectUI::OnStage1Clicked()
{
	OpenStageIndex(0);
	
}

void UStageSelectUI::OnStage2Clicked()
{
	OpenStageIndex(1);
}

void UStageSelectUI::OnStage3Clicked()
{
	OpenStageIndex(2);
}

void UStageSelectUI::OpenStageIndex(int32 Index)
{
	
	if (!StageNames.IsValidIndex(Index))
	{
		return;
	}

	PendingStageIndex = Index;


	if (SoundToPlayPushButton)
	{
		UGameplayStatics::PlaySound2D(this, SoundToPlayPushButton);
		UE_LOG(LogTemp, Warning, TEXT("SE play"));
	}

	// 少し待ってからステージ遷移
	GetWorld()->GetTimerManager().SetTimer(
		StageOpenTimer,
		this,
		&UStageSelectUI::OpenStageDelay,
		0.5f, 
		false
	);

}

void UStageSelectUI::OpenStageDelay()
{
	if (UMyGameInstance* GI = GetGameInstance<UMyGameInstance>())
	{
		GI->FadeOpenLevel(FName(StageNames[PendingStageIndex]));
	}
}
