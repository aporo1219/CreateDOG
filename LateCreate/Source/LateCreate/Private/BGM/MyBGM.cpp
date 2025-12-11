// Fill out your copyright notice in the Description page of Project Settings.


#include "BGM/MyBGM.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AMyBGM::AMyBGM()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BGMComponet = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	RootComponent = BGMComponet;

	BGMComponet->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AMyBGM::BeginPlay()
{
	Super::BeginPlay();
	
	FString StageName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	UE_LOG(LogTemp, Warning, TEXT("BGM play"));
	int BGMIndex = -1;

	if (StageName == "stageselect")
	{
		BGMIndex = 0;
	}
	else if (StageName == "stage1" || StageName == "stage2" )
	{
		BGMIndex = 1;
	}
	else if (StageName == "stage3" || StageName == "stage4")
	{
		BGMIndex = 2;
	}
	else if (StageName == "Title")
	{
		BGMIndex = 3;
	}
	//BGM再生
	if (BGMs.IsValidIndex(BGMIndex) && BGMComponet)
	{
		BGMComponet->SetSound(BGMs[BGMIndex]);
		BGMComponet->Play();
	}
}

void AMyBGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

//ゲームクリア時にBGMを流す
void AMyBGM::PlayClearBGM()
{
	if (BGMs.IsValidIndex(FORE))
	{
		BGMComponet->SetSound(BGMs[FORE]);
		BGMComponet->Play();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameOver BGM index is invalid!"));
	}
}

//ゲームオーバー時にBGMを流す
void AMyBGM::PlayOverBGM()
{
	if (BGMs.IsValidIndex(FIVE))
	{
		BGMComponet->SetSound(BGMs[FIVE]);
		BGMComponet->Play();
	}
}

