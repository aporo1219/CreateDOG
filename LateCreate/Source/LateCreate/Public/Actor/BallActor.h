// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

UCLASS()
class LATECREATE_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallActor();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* Movement;
public:	
	//‹Ê‚Ì“®‚«
	void InitVelocity(const FVector& ShootDir);



private:
	float I_Speed = 2000.0f;
	float Max_Speed = 2000.0f;

};
