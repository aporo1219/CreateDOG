// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBullet.h"
#include "Character/Dog.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{

	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// 衝突・物理設定
// 衝突・物理設定
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetGenerateOverlapEvents(true);

	// オーバーラップイベントバインド
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnHit);
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
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	FVector ForwardDir = GetActorForwardVector();
	float Speed = 1000.f;

	UProjectileMovementComponent* Projectile = FindComponentByClass<UProjectileMovementComponent>();
	if (Projectile)
	{
		Projectile->Velocity = ForwardDir * Speed;
	}
	else
	{
		// もしProjectileMovementを使ってない場合
		SetActorLocation(GetActorLocation() + ForwardDir * Speed * GetWorld()->GetDeltaSeconds());
	}

}

// Called every frame
void AEnemyBullet::InitVelocity(const FVector& ShootDir)
{
	if (Movement)
	{
		Movement->Velocity = ShootDir * Movement->InitialSpeed;

		Movement->Activate(true);

		SetActorRotation(ShootDir.Rotation());

		UE_LOG(LogTemp, Warning, TEXT("InitVelocity called: %s"), *Movement->Velocity.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileMovement is NULL!"));
	}
}


void AEnemyBullet::OnHit(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBullet::OnHit called with %s"), *OtherActor->GetName());
	
	//nullかの確認
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
		return;

	// ここにログを入れる
	UE_LOG(LogTemp, Warning, TEXT("Overlap with %s"), *OtherActor->GetName());
	//当たったかの確認
	ADog* Dog = Cast<ADog>(OtherActor);
	if (Dog)
	{
		Dog->TakeDamege(Damege);
	}

	//玉の削除
	Destroy();
}
