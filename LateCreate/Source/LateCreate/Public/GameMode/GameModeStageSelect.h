// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeStageSelect.generated.h"

/**
 * 
 */
UCLASS()
class LATECREATE_API AGameModeStageSelect : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameModeStageSelect();

protected:
	virtual void BeginPlay() override;

public:
	// ステージセレクトUIのBlueprintクラスを設定するための変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> StageSelectUIClass;

private:
	//UIの参照
	UPROPERTY()
	class UUserWidget* StageSelectUI;

	//変数宣言
	int ViewPort = 100;
};
