// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/BallActor.h"
#include "Character/Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Gamemode/MyGameModeBase.h"

// Sets default values
ABallActor::ABallActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// 衝突・物理設定
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Mesh->OnComponentHit.AddDynamic(this, &ABallActor::OnHit);

	// メッシュ設定
	UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh)
	{
		Mesh->SetStaticMesh(SphereMesh);
	}

	// ProjectileMovement
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Movement->InitialSpeed = 3000.0f;
	Movement->MaxSpeed = 3000.0f;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 0.0f;
	Movement->UpdatedComponent = RootComponent;
	Movement->bShouldBounce = false;

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABallActor::BeginPlay()
{
	Super::BeginPlay();

	if (Movement)
	{
		if (Mesh)
		{
			Movement->SetUpdatedComponent(Mesh);
		}

		// デバッグログで確認
		UE_LOG(LogTemp, Warning, TEXT("Ball BeginPlay Velocity = %s"), *Movement->Velocity.ToString());
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Movement->Velocity * 0.01f, FColor::Green, false, 2.0f, 0, 2.0f);

		// 念のため明示的に有効化
		Movement->SetComponentTickEnabled(true);
		Movement->Activate(true);
	}
}

void ABallActor::InitVelocity(const FVector& ShootDir)
{
	if (Movement)
	{
		FVector LaunchVel = ShootDir * Movement->InitialSpeed;
		Movement->Velocity = LaunchVel;
		Movement->Activate(true);
		SetActorRotation(ShootDir.Rotation());
		UE_LOG(LogTemp, Warning, TEXT("InitVelocity called: %s"), *Movement->Velocity.ToString());
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + LaunchVel * 0.01f, FColor::Red, false, 2.0f, 0, 2.0f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileMovement is NULL!"));
	}
}

void ABallActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{

	UE_LOG(LogTemp, Warning, TEXT("OnHit called! Actor: %s"), *OtherActor->GetName());

	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		Enemy->StartBlinkAndDie();
		UE_LOG(LogTemp, Warning, TEXT("Hit AEnemy class! Destroying it."));
		AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->Score += HitScore;
			//スコアが0以下になったら0にする
			if (GM->Score < 0)
			{
				GM->Score = 0;
				//UI通知
				GM->OnScoreChanged.Broadcast();
			}
		}
		//UI通知
		GM->OnScoreChanged.Broadcast();
	}
	
	// 弾自身も消す
	Destroy();
}

