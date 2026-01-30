// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_GameOver.generated.h"

/**
 * 
 */
UCLASS()
class LATECREATE_API UUserWidget_GameOver : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* StageSelectButton;

	//リスタートボタンが押されたとき
	UFUNCTION()
	void OnRestartCliked();

	//ステージセレクトボタンが押されたとき
	UFUNCTION()
	void OnStageSelectCliked();

	void OpenStageDelaySS();

	void OpenStageDelayR();
private:
	FTimerHandle StageOpenTimer;
	//変数宣言
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayPushButton;

	const float TRANSITION_TIME{ 3.0f };
};
