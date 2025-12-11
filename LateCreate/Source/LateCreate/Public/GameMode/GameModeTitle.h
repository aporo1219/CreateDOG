// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeTitle.generated.h"

/**
 * 
 */
UCLASS()
class LATECREATE_API AGameModeTitle : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeTitle();

protected:
	virtual void BeginPlay() override;
public:
	//タイトルUIのBlueprintクラスを設定するための変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget>TitleUIClass;

private:
	//UIの参照
	UPROPERTY()
	class UUserWidget* TitleUI;

	//変数宣言
	int ViewPort = 100;
	
};
