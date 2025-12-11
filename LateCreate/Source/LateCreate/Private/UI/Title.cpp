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
	UGameplayStatics::OpenLevel(GetWorld(), FName("StageSelect"));
}
