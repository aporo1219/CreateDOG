// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class LATECREATE_API UStageSelectUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	//Blueprintのボタンとバインドクラス
	UPROPERTY(meta = (BindWidget))
	class UButton* Stage1Button;
	UPROPERTY(meta = (BindWidget))
	class UButton* Stage2Button;
	UPROPERTY(meta = (BindWidget))
	class UButton* Stage3Button;
	UPROPERTY(meta = (BindWidget))
	class UButton* Stage4Button;

	//画像とバインドクラス
	UPROPERTY(meta = (BindWidget))
	class UImage* Stage1Image;
	UPROPERTY(meta = (BindWidget))
	class UImage* Stage2Image;
	UPROPERTY(meta = (BindWidget))
	class UImage* Stage3Image;
	UPROPERTY(meta = (BindWidget))
	class UImage* Stage4Image;

	//テキストとバインドクラス
	UPROPERTY(meta = (BindWidget))
	class UText* Stage1Text;
	UPROPERTY(meta = (BindWidget))
	class UText* Stage2Text;
	UPROPERTY(meta = (BindWidget))
	class UText* Stage3Text; 
	UPROPERTY(meta = (BindWidget))
	class UText* Stage4Text;

	//リスト格納用
	TArray<UButton*> StageButtons;
	TArray<UImage*> StageImages;
	TArray<UText*> StageTexts;

	//ステージ名の配列
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "stage")
	TArray<FName> StageNames;

private:
	//クリックイベント
	UFUNCTION()
	void OnStage1Clicked();

	UFUNCTION()
	void OnStage2Clicked();

	UFUNCTION()
	void OnStage3Clicked();

	UFUNCTION()
	void OnStage4Clicked();

	void OpenStageIndex(int32 Index);
};
