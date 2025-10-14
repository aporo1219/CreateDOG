// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BallActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
ABallActor::ABallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	//•¨—AÕ“Ë‚Ì’Ç‰Á
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMesh)
	{
		Mesh->SetStaticMesh(SphereMesh);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Movement->InitialSpeed = I_Speed;
	Movement->MaxSpeed = Max_Speed;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 0.0f;

	// ’e‚ÌŽõ–½i3•b‚ÅÁ–Åj
	InitialLifeSpan = 3.0f;
}


// Called when the game starts or when spawned
void ABallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallActor::InitVelocity(const FVector& ShootDir)
{
	if (Movement)
	{
		Movement->Velocity = ShootDir * Movement->InitialSpeed;
	}
}
