// Fill out your copyright notice in the Description page of Project Settings.


#include "BGM/MyBGM.h"
#include "Kismet/GameplayStatics.h"

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
	
	UE_LOG(LogTemp, Warning, TEXT("BGM play"));
	if (stage1BGM)
	{
		BGMComponet->SetSound(stage1BGM);
		SetBGM();
		
	}
	
}

void AMyBGM::SetBGM()
{
	FString StageName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	
	if (StageName == "stage1")
	{
		if (BGMComponet && !BGMComponet->IsPlaying())
		{
			BGMComponet->Play();
			
		}
	}
}

void AMyBGM::StopBGM()
{

}

