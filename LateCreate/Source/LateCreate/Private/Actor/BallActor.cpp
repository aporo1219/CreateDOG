// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/BallActor.h"
#include "Character/Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"

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
		const FVector LaunchVel = ShootDir.GetSafeNormal() * Movement->InitialSpeed;
		Movement->Velocity = LaunchVel;
		Movement->SetVelocityInLocalSpace(LaunchVel);
		Movement->Activate(true);

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
	//UE_LOG(LogTemp, Warning, TEXT("Hit"));
	/*if (OtherActor->ActorHasTag("Enemy"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Enemy! Destroying it."));
		OtherActor->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor has NO Enemy tag! Tag count: %d"), OtherActor->Tags.Num());
		for (auto Tag : OtherActor->Tags)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
		}
	}*/

	UE_LOG(LogTemp, Warning, TEXT("OnHit called! Actor: %s"), *OtherActor->GetName());

	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit AEnemy class! Destroying it."));
		Enemy->Destroy();
	}
	
}

