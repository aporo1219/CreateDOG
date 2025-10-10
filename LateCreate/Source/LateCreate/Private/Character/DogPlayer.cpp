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

	//StaticMeshComponent��ǉ��ARootComponent�ɐݒ�
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaiticMeshComponent"));
	RootComponent = Cube;

	// StaticMesh��Laod����StaticMeshComponent��StaticMesh�ɐݒ肷��
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));

	// StaticMesh��StaticMeshComponent�ɐݒ肷��
	Cube->SetStaticMesh(Mesh);

	// Material��StaticMesh�ɐݒ肷��
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	// Material��StaticMeshComponent�ɐݒ肷��
	Cube->SetMaterial(0, Material);

	// Simulate Physics��L���ɂ���(�����V�~�����[�V�����j
	Cube->SetSimulatePhysics(true);

	// SpringArm��ǉ�����
	SpringArm = CreateDefaultSubobject < USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);

	// �p�x��ύX���� FRotator(Pitch(Y), Yaw(Z), Roll(X))
	SpringArm->SetRelativeRotation(FRotator(PitchArm, YawArm, RollArm));

	// Spring Arm�̒����𒲐�����
	SpringArm->TargetArmLength = ArmLength;

	// SpringArm����̊p�x���p�����Ȃ�
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	// Camera��Lag��L���ɂ���
	SpringArm->bEnableCameraLag = true;

	//Camera�̒ǉ�
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

