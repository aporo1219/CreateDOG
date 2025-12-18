// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MoveGoalPoint.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMoveGoalPoint::AMoveGoalPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	//コリジョンなし
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//モデルの呼び出し
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Engine/BasicShapes/Cylinder"));

	//モデルの設定
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetWorldScale3D(ScaleSize);
	}
}



