// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void AInGameHUD::BeginPlay()
{
    Super::BeginPlay();

    // è≠ÇµíxâÑÇ≥ÇπÇƒê∂ê¨
    FTimerHandle TempHandle;
    GetWorldTimerManager().SetTimer(TempHandle, [this]()
        {
            // WidgetBlueprintÇÃClassÇéÊìæ
            FString ScorePath = TEXT("/Game/BP/UI/UI.UI_C");
            TSubclassOf<UUserWidget> ScoreClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*ScorePath)).LoadSynchronous();

            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (!ScoreClass || !PlayerController)
            {
                UE_LOG(LogTemp, Error, TEXT("ScoreClass or PlayerController is null!"));
                return;
            }

            UUserWidget* StatusWidget = CreateWidget<UUserWidget>(PlayerController, ScoreClass);
            if (StatusWidget)
            {
                StatusWidget->AddToPlayerScreen(9999); // ëOñ Ç…ï\é¶
                StatusWidget->SetVisibility(ESlateVisibility::Visible);
                UE_LOG(LogTemp, Log, TEXT("StatusWidget created successfully."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create StatusWidget!"));
            }

        }, 0.05f, false); // 0.05ïbíxâÑ
}
