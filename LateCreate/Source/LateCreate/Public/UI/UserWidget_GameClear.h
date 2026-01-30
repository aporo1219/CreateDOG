// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "UserWidget_GameClear.generated.h"

/**
 * 
 */
UCLASS()
class LATECREATE_API UUserWidget_GameClear : public UUserWidget
{
	GENERATED_BODY()

	////スコアUI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextScore;

	//ボタンUI
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* StageSelectButton;

	//ステージセレクトボタンが押されたとき
	UFUNCTION()
	void OnStageSelectCliked();

public:
	//ネイティブコンストラクト
	virtual void NativeConstruct() override;

	UButton* GetStageSelectButton() { return StageSelectButton; };

	void OpenStageDelaySS();

	void OpenStageDelayR();

private:
	FTimerHandle StageOpenTimer;
	//変数宣言
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayPushButton;

	const float TRANSITION_TIME{ 3.0f };
};
