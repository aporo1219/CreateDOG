// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "FadeWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeFinished);

UCLASS()
class LATECREATE_API UFadeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FFadeFinished OnFadeOutFinished;

	//フェードアウト関数
	UFUNCTION(BlueprintCallable)
	void PlayFadeOut();

	//フェードイン関数
	UFUNCTION(BlueprintCallable)
	void PlayFadeIn();
	
	UWidgetAnimation* GetFadeOutAnim() const { return FadeOut; }

	bool IsPlayFade() const { return bIsReady; };
protected:
	virtual void NativeConstruct() override;
	//UMGのアニメーションのバインド
	UPROPERTY(meta = (BindWidgetAnim),Transient)
	UWidgetAnimation* FadeOut;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;

private:
	bool bIsReady = false;
};
