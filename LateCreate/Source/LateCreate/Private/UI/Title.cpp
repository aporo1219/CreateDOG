// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTitle::NativeConstruct()
{
	//ボタンの登録
	if (StageSelectButton)
		StageSelectButton->OnClicked.AddDynamic(this, &UTitle::OnStageSelectCliced);
}

//クリックされたときの関数
void UTitle::OnStageSelectCliced()
{
	//BGMを一瞬下げる
	if (SEDuckSoundMix)
	{
		UGameplayStatics::PushSoundMixModifier(this, SEDuckSoundMix);
	}

	if (SoundToPlayPushButton)
	{
		UGameplayStatics::PlaySound2D(this, SoundToPlayPushButton);
		UE_LOG(LogTemp, Warning, TEXT("SE play"));
	}

	// 少し待ってからステージ遷移
	GetWorld()->GetTimerManager().SetTimer(
		StageOpenTimer,
		this,
		&UTitle::OpenStageDelay,
		TRANSITION_TIME,
		false
	);
}

//ステージを開く
void UTitle::OpenStageDelay()
{
	//音量を下げる
	if (SEDuckSoundMix)
	{
		UGameplayStatics::PopSoundMixModifier(this, SEDuckSoundMix);
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("StageSelect"));
}