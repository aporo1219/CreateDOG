// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MyGameModeBase.h"
#include "Controller/MainPlayerController.h"
#include "Character/Dog.h"
#include "Actor/BallActor.h"
#include "Framework/InGameHUD.h"
#include "UI/Score.h"
#include "UI/StageSelectUI.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include <UI/UserWidget_GameClear.h>

AMyGameModeBase::AMyGameModeBase()
{

    //最初のPawnの設定
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

	//1秒ごとにUpdateTimeを呼ぶ
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyGameModeBase::UpdateTime, 1.0f, true);

}

//制限時間処理関数
void AMyGameModeBase::UpdateTime()
{
	RemainingTime -= 1.0f;

	if (RemainingTime <= 0)
	{
		RemainingTime = 0;
	}

    OnTimeChanged.Broadcast((int32)RemainingTime);

	//ゲームクリア
	if (RemainingTime <= 0)
	{
        GetWorldTimerManager().ClearTimer(TimerHandle);
        FTimerHandle TempHandle;
        GetWorldTimerManager().SetTimer(TempHandle, this, &AMyGameModeBase::GameClear, 0.05f, false);
	}
}

//スコア処理関数
void AMyGameModeBase::AddScore()
{
    bool ChangeScore = false;

	if (const ADog* Dog = Cast<ADog>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
        //弾の発射でスコアを減らす
		Score -= Dog->ShootScore;

        ChangeScore = true;
        
		UE_LOG(LogTemp, Warning, TEXT("Score Updated: %d"), (int)Score);
	}

     if (const ABallActor* BulletP = Cast<ABallActor>(UGameplayStatics::GetActorOfClass(GetWorld(),ABallActor::StaticClass())))
	{
        //弾が敵に当たるとスコアを増やす
		Score -= BulletP->HitScore;

        ChangeScore = true;
	}

    if (ChangeScore)
    {
        //スコアの変動をUIに送る
        OnScoreChanged.Broadcast();
    }
}

//ゲームクリア処理
void AMyGameModeBase::GameClear()
{
    if (!GameClearClass)
    {
        GameClearClass = LoadClass<UUserWidget_GameClear>(nullptr, TEXT("/Game/BP/UI/GameClear_UI.GameClear_UI_C"));
    }

    if (!GameClearClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load GameClear_UI!"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    UUserWidget_GameClear* GameClearUI = CreateWidget<UUserWidget_GameClear>(World, GameClearClass.Get());
    if (!GameClearUI)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create GameClearUI!"));
        return;
    }

    GameClearUI->AddToViewport(999);
    GameClearUI->SetVisibility(ESlateVisibility::Visible);

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;
        PC->bEnableClickEvents = true;
        PC->bEnableMouseOverEvents = true;

        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
    }

    UGameplayStatics::SetGamePaused(World, true);
}

void AMyGameModeBase::AddEnemyKillScore(int32 killscore)
{
    Score += killscore;
    OnScoreChanged.Broadcast();
}
