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
	//•Ï”éŒ¾
	UPROPERTY(EditDefaultsOnly,Category = "Timer")
	float MaxTime = 60.0f;

	

	FTimerHandle TimerHandle;

	UPROPERTY()
	UScore* TimeUI;

	//ƒNƒ‰ƒXéŒ¾
	UPROPERTY(EditDefaultsOnly,Category = "UI")
	TSubclassOf<UScore> UScoreClass;

	void UpdateTime();

	
public:
	//•Ï”éŒ¾
	float GetRemainingTime() const { return RemainingTime; }

	float RemainingTime;
};
