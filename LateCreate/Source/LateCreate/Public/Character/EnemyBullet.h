// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class LATECREATE_API AEnemyBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyBullet();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionSphere;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* Movement;
public:
	//‹Ê‚Ì“®‚«
	void InitVelocity(const FVector& ShootDir);

	//ŠÖ”éŒ¾
    //Œ¢‚É“–‚½‚Á‚½‚Ìˆ—
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	//•Ï”éŒ¾
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damege = 10.0f;

	//•Ï”
private:
	float I_Speed = 5000.0f;
	float Max_Speed = 5000.0f;
};
