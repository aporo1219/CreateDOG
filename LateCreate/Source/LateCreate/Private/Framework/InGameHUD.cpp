// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	// WidgetBlueprint‚ÌClass‚ğæ“¾‚·‚é
	FString ScorePath = TEXT("/Game/BP/UI/UI.UI_C"); // © Copy Reference‚ÅŠm”F
	TSubclassOf<UUserWidget> ScoreClass =
		TSoftClassPtr<UUserWidget>(FSoftObjectPath(*ScorePath)).LoadSynchronous();

	// PlayerController‚ğæ“¾‚·‚é
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// WidgetClass‚ÆPlayerController‚ªæ“¾‚Å‚«‚½‚©”»’è‚·‚é
	if (!ScoreClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ScoreClass not found at %s"), *ScorePath);
		return;
	}

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
		return;
	}

	// Widget‚ğì¬
	UUserWidget* StatusWidget = CreateWidget<UUserWidget>(PlayerController, ScoreClass);

	if (StatusWidget)
	{
		StatusWidget->AddToViewport(0);
		UE_LOG(LogTemp, Log, TEXT("StatusWidget created successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create StatusWidget!"));
	}
}