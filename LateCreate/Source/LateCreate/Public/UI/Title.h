// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundMix.h"
#include "Title.generated.h"

/**
 * 
 */
UCLASS()
class LATECREATE_API UTitle : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	//ボタンのバインド
	UPROPERTY(meta = (BindWidget))

	class UButton* StageSelectButton;

	//クリックイベント
	UFUNCTION()
	void OnStageSelectCliced();

	void OpenStageDelay();

	//サウンドミックスのバインド
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sound")
	USoundMix* SEDuckSoundMix;

private:
	FTimerHandle StageOpenTimer;
	//変数宣言
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayPushButton;

	const float TRANSITION_TIME{ 3.0f };
};
