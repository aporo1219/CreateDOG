// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveGoalPoint.generated.h"

UCLASS()
class LATECREATE_API AMoveGoalPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveGoalPoint();

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

private:
	FVector ScaleSize = { 0.3f,0.3f,0.1f };
};
