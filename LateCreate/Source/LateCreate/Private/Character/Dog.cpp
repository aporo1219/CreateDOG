// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dog.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/BallActor.h"

// Sets default values
ADog::ADog()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//StaticMeshComponent��ǉ��ARootComponent�ɐݒ�
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaiticMeshComponent"));
	RootComponent = Sphere;

	// StaticMesh��Laod����StaticMeshComponent��StaticMesh�ɐݒ肷��
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));

	// StaticMesh��StaticMeshComponent�ɐݒ肷��
	Sphere->SetStaticMesh(Mesh);

	// Material��StaticMesh�ɐݒ肷��
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	// Material��StaticMeshComponent�ɐݒ肷��
	Sphere->SetMaterial(0, Material);

	// Simulate Physics��L���ɂ���(�����V�~�����[�V�����j
	Sphere->SetSimulatePhysics(true);

	// SpringArm��ǉ�����
	SpringArm = CreateDefaultSubobject < USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);

	// �p�x��ύX���� FRotator(Pitch(Y), Yaw(Z), Roll(X))
	//SpringArm->SetRelativeRotation(FRotator(PitchArm, YawArm, RollArm));

	// Spring Arm�̒����𒲐�����
	SpringArm->TargetArmLength = ArmLength;

	// Pawn��ControllerRotation���g�p����
	SpringArm->bUsePawnControlRotation = true;

	// Camera��Lag��L���ɂ���
	SpringArm->bEnableCameraLag = true;

	//Camera�̒ǉ�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

    //InputMapping�̓ǂݍ���
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/input/InputMappingContext"));

	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Look"));

	//���ˈʒu
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	//��������
	MuzzlePoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

    //Attack�A�N�V�����̓ǂݍ���
	AttackAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Attack"));
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ControlBall��IA_Look��Triggered��Bind����
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADog::Look);
		// ControlBall��IA_Attack��Triggered��Bind����
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADog::Attack);
		// ControlBall��IA_Standerd��Triggered��Bind����
		EnhancedInputComponent->BindAction(StanderdAction, ETriggerEvent::Triggered, this, &ADog::Standerd);
	}
}

void ADog::Look(const FInputActionValue& Value)
{
	// input��Value��Vector2D�ɕϊ�
	FVector2D v = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(v.X);
		AddControllerPitchInput(v.Y);

		//Dog�������Ă���Control�̊p�x���擾
		FRotator controlRotate = GetControlRotation();

		// PlayerController�̊p�x��ݒ肷��
		UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(FRotator(controlRotate.Pitch,controlRotate.Yaw,0.0f));
	}
}

void ADog::Attack(const FInputActionValue& Value)
{
	if (!BallActorClass)
	{
		return;
    }  

	FVector MuzzleLocation = MuzzlePoint->GetComponentLocation();
	FRotator MuzzleRotation = Camera->GetComponentRotation(); // �J�����̌����ɔ���

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// �e�ۂ𐶐�
	ABallActor* BallActor = GetWorld()->SpawnActor<ABallActor>(BallActorClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	if (BallActor)
	{
		BallActor->InitVelocity(MuzzleRotation.Vector());
	}
}

void ADog::Standerd(const FInputActionValue& Value)
{

}
