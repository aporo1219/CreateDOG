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

	//Collision�ݒ�
	//CollisionPreset��[PhysicsActor]�ɕύX
	Sphere->SetCollisionProfileName(TEXT("PhysicsActor"));
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetNotifyRigidBodyCollision(true);

	//HitEvennt��L���ɂ���
	Sphere->BodyInstance.bNotifyRigidBodyCollision = true;

	// SpringArm��ǉ�����
	SpringArm = CreateDefaultSubobject < USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);



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
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/input/InputMappingContext1"));

	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Look"));

	//���ˈʒu
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	//��������
	MuzzlePoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

    //Attack�A�N�V�����̓ǂݍ���
	AttackAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Attack"));

	//Jump�A�N�V�����ǂݍ���
	JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Jump"));
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	//�C���b�v�g�}�b�s���O�̒ǉ�
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("MappingContext added!"));
		}
	}
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADog::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("CanJump"));
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (Other)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *Other->GetName());
	}
	CanJump = true;
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADog::Attack);
		// ControlBall��IA_Standerd��Triggered��Bind����
		EnhancedInputComponent->BindAction(StanderdAction, ETriggerEvent::Triggered, this, &ADog::Standerd);
		// ControlBall��IA_Jump��Triggered��Bind����
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADog::Jump);
	}
}

//���_����̏����֐�
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

//�U������̏����֐�
void ADog::Attack(const FInputActionValue& Value)
{
	//�X�|�[���l
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	//�ʒu�̐ݒ�
	FVector SpawnLoc = GetActorLocation() + GetActorForwardVector() * Spawnlocation;
	FRotator SpawnRot = GetActorRotation();

	ABallActor* Ball = GetWorld()->SpawnActor<ABallActor>(BallActorClass, SpawnLoc, SpawnRot, Params);

	  if (Ball)
    {
        // ���˕�����n��
        Ball->InitVelocity(GetActorForwardVector());

        UE_LOG(LogTemp, Warning, TEXT("Ball spawned and InitVelocity called!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Ball spawn failed!"));
    }
}

void ADog::Standerd(const FInputActionValue& Value)
{

}



//�W�����v����̏����֐�
void ADog::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));
	if (const bool v = Value.Get<bool>() && CanJump)
	{
		Sphere->AddImpulse(FVector(0.0f, 0.0f, JumpForce), TEXT("None"), true);
		CanJump = false;
	}
}

