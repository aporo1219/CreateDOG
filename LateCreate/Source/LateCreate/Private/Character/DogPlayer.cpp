// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DogPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ADogPlayer::ADogPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMeshComponentを追加、RootComponentに設定
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaiticMeshComponent"));
	RootComponent = Cube;

	// StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));

	// StaticMeshをStaticMeshComponentに設定する
	Cube->SetStaticMesh(Mesh);

	// MaterialをStaticMeshに設定する
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	// MaterialをStaticMeshComponentに設定する
	Cube->SetMaterial(0, Material);

	// Simulate Physicsを有効にする(物理シミュレーション）
	Cube->SetSimulatePhysics(true);

	// SpringArmを追加する
	SpringArm = CreateDefaultSubobject < USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);

	// 角度を変更する FRotator(Pitch(Y), Yaw(Z), Roll(X))
	SpringArm->SetRelativeRotation(FRotator(PitchArm, YawArm, RollArm));

	// Spring Armの長さを調整する
	SpringArm->TargetArmLength = ArmLength;

	// SpringArmからの角度を継承しない
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	// CameraのLagを有効にする
	SpringArm->bEnableCameraLag = true;

	//Cameraの追加
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ADogPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADogPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADogPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

