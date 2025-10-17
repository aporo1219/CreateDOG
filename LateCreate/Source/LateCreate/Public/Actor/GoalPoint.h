// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalPoint.generated.h"

UCLASS()
class LATECREATE_API AGoalPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoalPoint();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
