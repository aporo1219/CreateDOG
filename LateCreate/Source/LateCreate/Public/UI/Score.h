// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Score.generated.h"

/**
 * 
 */
UCLASS()
class LATECREATE_API UScore : public UUserWidget
{
	GENERATED_BODY()
	
private:
	//残り体力のUI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextHealth;

   //最大体力
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextMaxHealth;

	////時間UI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextTime;

	////スコアUI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextScore;

protected:
	//ネイティブコンストラクト
	virtual void NativeConstruct() override;

	//セリフのテキストボックス
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogueText;

private:
	//残り体力
	UFUNCTION()
	FText SetTextHealth();
	//最大体力
	UFUNCTION()
	FText SetTextMaxHealth();
	////時間
	UFUNCTION()
	FText SetTextTime();
	//スコア
	UFUNCTION()
	FText SetTextScore();
private:
	//関数宣言
	UFUNCTION(BlueprintCallable, category = "UI")
	bool IsTimeLow() const;

	UFUNCTION()
	void UpdateHealthText();

	UFUNCTION()
	void UpdateScoreText();

	UFUNCTION()
	void UpdateTimeText(int32 NewTime);

	FTimerHandle HideTimerHandle;

	//変数宣言
	float RedTimeText = 10.0f;

	float Score = 1000.0f;



public:
	//テキスト表示
	UFUNCTION(BlueprintCallable)
	void ShowDialogue(const FText& Text, float DisplayTime);

	void HideDialogue();

	UFUNCTION()
	void ReceiveDialogue(const FText& InDialogueText);
};
