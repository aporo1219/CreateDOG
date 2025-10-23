// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MyGameModeBase.h"
#include "Controller/MainPlayerController.h"
#include "Character/Dog.h"
#include "Character/DogPlayer.h"
#include "Framework/InGameHUD.h"
#include "UI/Score.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase()
{

    //Å‰‚ÌPawn‚Ìİ’è
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP/BPDog"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = AInGameHUD::StaticClass();
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	RemainingTime = MaxTime;

	//1•b‚²‚Æ‚ÉUpdateTime‚ğŒÄ‚Ô
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyGameModeBase::UpdateTime, 1.0f, true);
}

//§ŒÀŠÔˆ—ŠÖ”
void AMyGameModeBase::UpdateTime()
{
	RemainingTime -= 1.0f;

	if (RemainingTime <= 0)
	{
		RemainingTime = 0;
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}
