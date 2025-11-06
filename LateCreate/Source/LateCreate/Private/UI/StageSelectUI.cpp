// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UStageSelectUI::NativeConstruct()
{
   Super::NativeConstruct();

	//ボタンをまとめる
	StageButtons = { Stage1Button,Stage2Button,Stage3Button,Stage4Button };


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
	if (SoundToPlayPushButton)
	{
		UGameplayStatics::PlaySound2D(this, SoundToPlayPushButton);
		UE_LOG(LogTemp, Warning, TEXT("SE play"));
	}
	if (StageNames.IsValidIndex(Index))
	{
		
		//一時保存
		PendingStageName = StageNames[Index];

		if (FadeOutAnim)
		{
			UE_LOG(LogTemp, Warning, TEXT("FadeOutAnim play"));
			//フェード再生
			PlayAnimation(FadeOutAnim);

			//アニメ終了時に呼ばれる処理を登録
			FWidgetAnimationDynamicEvent EndEvent;
			EndEvent.BindDynamic(this, &UStageSelectUI::OnFadeOutFinished);
			BindToAnimationFinished(FadeOutAnim, EndEvent);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("FadeOutAnim が NULL"));
			//即ロード
			UGameplayStatics::OpenLevel(GetWorld(), FName(PendingStageName));
		}
		
		UGameplayStatics::OpenLevel(GetWorld(), StageNames[Index]);
	}
}

void UStageSelectUI::OnFadeOutFinished()
{
	if (!PendingStageName.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(PendingStageName));
	}
}