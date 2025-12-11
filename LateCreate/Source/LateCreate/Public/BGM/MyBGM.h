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
	virtual void Tick(float DeltaTime) override;


protected:
	//BGM‚ÌAudioComponet
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = Audio)
	UAudioComponent* BGMComponet;

	//SoundBase‚ð‘z’è
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	TArray<USoundBase*> BGMs;


public:	
	UFUNCTION(BlueprintCallable)
	void PlayClearBGM();

	UFUNCTION(BlueprintCallable)
	void PlayOverBGM();

private:
	const int FORE{ 4 };
	const int FIVE{ 5 };
};
