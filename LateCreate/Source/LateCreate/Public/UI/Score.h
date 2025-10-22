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

	//時間UI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextTime;

	//残りの敵数UI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextRemainEnemy;

	//スコアUI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextScore;

protected:
	//ネイティブコンストラクト
	virtual void NativeConstruct() override;
	//初期化
	bool Initialize() override;

private:
	//残り体力
	UFUNCTION()
	FText SetTextHealth();
	//最大体力
	UFUNCTION()
	FText SetTextMaxHealth();
	//時間
	UFUNCTION()
	FText SetTextTime();
	//残りの敵数
	UFUNCTION()
	FText SetTextRemainEnemy();
	//スコア
	UFUNCTION()
	FText SetTextScore();
};
