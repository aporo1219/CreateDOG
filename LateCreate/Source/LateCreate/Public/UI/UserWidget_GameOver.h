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

	//リスタートボタンが押されたとき
	UFUNCTION()
	void OnRestartCliked();
};
