// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	// WidgetBlueprint‚ÌClass‚ðŽæ“¾‚·‚é
	FString ScorePath = TEXT("/Game/BP/UI/UI.UI_C");
	TSubclassOf<UUserWidget> ScoreClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*ScorePath)).LoadSynchronous();

	// PlayerController‚ðŽæ“¾‚·‚é
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// WidgetClass‚ÆPlayerController‚ªŽæ“¾‚Å‚«‚½‚©”»’è‚·‚é
	if (ScoreClass && PlayerController)
	{
		// Status•\Ž¦—p‚ÌWidget‚ðì¬‚·‚é
		UUserWidget* StatusWidget = UWidgetBlueprintLibrary::Create(GetWorld(), ScoreClass, PlayerController);

		// Viewport‚É’Ç‰Á‚·‚é
		StatusWidget->AddToViewport(0);
	}
}