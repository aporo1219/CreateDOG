// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/GoalPoint.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGoalPoint::AGoalPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// è’ìÀÅEï®óùê›íË
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// ÉÅÉbÉVÉÖê›íË
	UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMesh)
	{
		Mesh->SetStaticMesh(CylinderMesh);
	}


}

// Called when the game starts or when spawned
void AGoalPoint::BeginPlay()
{
	Super::BeginPlay();
}


