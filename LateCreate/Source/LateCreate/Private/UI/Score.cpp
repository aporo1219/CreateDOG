// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Score.h"
#include "Components/TextBlock.h"
#include "Character/Dog.h"
#include "GameMode/MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

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

		Dog->OnSayDialogue.AddDynamic(this, &UScore::ReceiveDialogue);

		Dog->OnModeChanged.AddDynamic(this, &UScore::OnModeChange);

		// 初期状態を反映
		OnModeChange(Dog->GetIsChangeAttck());
		// 初期表示
		UpdateHealthText();

		
	}
	if (AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// イベントにバインド
		GM->OnTimeChanged.AddDynamic(this, &UScore::UpdateTimeText);

		GM->OnScoreChanged.AddDynamic(this, &UScore::UpdateScoreText);

		// 初期表示
		UpdateTimeText(GM->GetRemainingTime());

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
		//スコアの取得（変数に直接アクセス or Getter関数）
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
	if (const AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		TextScore->SetText(FText::AsNumber(GM->GetScore()));
	}
}

void UScore::UpdateTimeText(int32 NewTime)
{
	if (TextTime)
	{
		TextTime->SetText(FText::AsNumber(NewTime));

		// 10秒以下なら赤く点滅
		if (NewTime <= 10)
		{
			TextTime->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		}
	}
}

//セリフを受け取る
void UScore::ReceiveDialogue(const FText& InDialogueText)
{
	UE_LOG(LogTemp, Warning, TEXT("ReceiveDialogue: %s"), *InDialogueText.ToString());

	ShowDialogue(InDialogueText, 2.0f);
}

//セリフを表示させる関数
void UScore::ShowDialogue(const FText& Text, float DisplayTime)
{
	if (!DialogueText)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueText is NULL"));
		return;
	}


	DialogueText->SetText(Text);
	DialogueText->SetVisibility(ESlateVisibility::Visible);

	if (DisplayTime > 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			HideTimerHandle,
			this,
			&UScore::HideDialogue,
			DisplayTime,
			false
		);
	}
}

//セリフを隠す
void UScore::HideDialogue()
{
	if (DialogueText)
	{
		DialogueText->SetVisibility(ESlateVisibility::Hidden);
	}
}

//画像の切り換え
void UScore::ChangeImage(bool battack)
{
	if (!IconImage)
		return;
	
	if (battack && Attack)
	{
		IconImage->SetBrushFromTexture(Attack);
	}
	else if (!battack && Move)
	{
		IconImage->SetBrushFromTexture(Move);
	}
}

//画像の切り替え関数を呼ぶ出す
void UScore::OnModeChange(bool bIsAttack)
{
	UE_LOG(LogTemp, Warning, TEXT("OnModeChange: %s"), bIsAttack ? TEXT("Attack") : TEXT("Move"));
	ChangeImage(bIsAttack);
}