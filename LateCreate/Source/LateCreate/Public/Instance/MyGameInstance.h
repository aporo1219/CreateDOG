// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

class UFadeWidget;

UCLASS()
class LATECREATE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//初期化関数
	virtual void Init() override;

	// フェード付きレベル遷移
	void FadeOpenLevel(FName LevelName);

	// フェードイン
	void PlayFadeIn();

private:
	//ビューポートのオーダー変数
	const int FADE_VIEW{ 999 };

	UPROPERTY()
	UFadeWidget* FadeWidget;

	FName PendingLevelName;

	UFUNCTION()
	void OnFadeOutFinished();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFadeWidget> FadeWidgetClass;
};


