// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UStageSelectUI::NativeConstruct()
{
   Super::NativeConstruct();

	//ボタンをまとめる
	StageButtons = { Stage1Button,Stage2Button,Stage3Button,Stage4Button };

	// 画像リストに追加
	StageImages = { Stage1Image, Stage2Image, Stage3Image, Stage4Image };

	// テキストリストに追加
	StageTexts = { Stage1Text, Stage2Text, Stage3Text, Stage4Text };

	//ステージ名の設定
	StageNames = { "stage1","Stage2","Stage3","Stage4" };

	//各ボタンに登録
	if (Stage1Button)
		Stage1Button->OnClicked.AddDynamic(this, &UStageSelectUI::OnStage1Clicked);
	if (Stage2Button)
		Stage2Button->OnClicked.AddDynamic(this, &UStageSelectUI::OnStage2Clicked);
	if (Stage3Button)
		Stage3Button->OnClicked.AddDynamic(this, &UStageSelectUI::OnStage3Clicked);
	if (Stage4Button)
		Stage4Button->OnClicked.AddDynamic(this, &UStageSelectUI::OnStage4Clicked);

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

void UStageSelectUI::OnStage4Clicked()
{
	OpenStageIndex(3);
}

void UStageSelectUI::OpenStageIndex(int32 Index)
{
	if (StageNames.IsValidIndex(Index))
	{
		UGameplayStatics::OpenLevel(GetWorld(), StageNames[Index]);
	}
}