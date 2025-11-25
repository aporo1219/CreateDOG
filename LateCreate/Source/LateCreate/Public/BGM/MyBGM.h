// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "MyBGM.generated.h"

UCLASS()
class LATECREATE_API AMyBGM : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBGM();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	//BGMÇÃAudioComponet
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = Audio)
	UAudioComponent* BGMComponet;

	//SoundCueÇëzíË
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundCue* stage1BGM;


public:	
	
	//BGMÇÃê›íËä÷êî
    UFUNCTION(BlueprintCallable,Category = Audio)
	void SetBGM();

	//BGMÇí‚é~Ç∑ÇÈä÷êî
	UFUNCTION(BlueprintCallable,Category = Audio)
	void StopBGM();
};
