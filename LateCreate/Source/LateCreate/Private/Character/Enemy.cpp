// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include  "Character/EnemyBullet.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    RootComponent = EnemyMesh;

    MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
    MuzzlePoint->SetupAttachment(EnemyMesh);
    MuzzlePoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f)); // ‘O•û

    FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input

//‹Ê”­ŽË
void AEnemy::Fire()
{
    if (EnemyBulletClass)
    {
        FVector SpawnLoc = MuzzlePoint->GetComponentLocation();
        FRotator SpawnRot = MuzzlePoint->GetComponentRotation();
        GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulletClass, SpawnLoc, SpawnRot);
    }
  
}
