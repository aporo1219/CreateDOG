// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
};
