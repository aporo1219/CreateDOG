// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_GameOver.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UUserWidget_GameOver::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (RestartButton)
	{
		//バインド
		RestartButton->OnClicked.AddDynamic(this, &UUserWidget_GameOver::OnRestartCliked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RestartButton is NULL!"));
	}
	return true;
}


//クリックされたときの処理
void UUserWidget_GameOver::OnRestartCliked()
{
	UE_LOG(LogTemp, Warning, TEXT("Restart Button Clicked!"));

	// ポーズ解除
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}
