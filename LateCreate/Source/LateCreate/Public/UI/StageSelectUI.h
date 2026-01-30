// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
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


	//リスト格納用
	TArray<UButton*> StageButtons;

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


	void OpenStageIndex(int32 Index);

	void OpenStageDelay();

	FTimerHandle StageOpenTimer;

	//遷移予定のステージを一時保存
	FName PendingStageName;

	int32 PendingStageIndex;
private:
	//変数宣言
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlayPushButton;
};
