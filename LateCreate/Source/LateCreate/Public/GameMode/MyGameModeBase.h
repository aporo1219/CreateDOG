// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */

class UScore;

UCLASS()
class LATECREATE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMyGameModeBase();

public: 
	virtual void BeginPlay() override;

protected:
	//変数宣言
	UPROPERTY(EditDefaultsOnly,Category = "Timer")
	float MaxTime = 60.0f;

	

	FTimerHandle TimerHandle;

	UPROPERTY()
	UScore* TimeUI;

	void UpdateTime();

	void AddScore();

	
public:
	//変数宣言
	float GetRemainingTime() const { return RemainingTime; }
	int32  GetScore() const { return Score; }

	float RemainingTime;

	UPROPERTY(BlueprintReadWrite, Category = "Score")
	int32 Score = 1000;

	//クラス宣言
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreChanged);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<class UUserWidget> GameClearClass;

	//クラス宣言
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<UScore> UScoreClass;

	//関数宣言
	UFUNCTION(BlueprintCallable, Category = "UI")
	void GameClear();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, int32, NewTime);

	UPROPERTY(BlueprintAssignable)
	FOnTimeChanged OnTimeChanged;
};
