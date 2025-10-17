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
#include "Blueprint/AIBlueprintHelperLibrary.h"//NavMesh�ړ��ɕK�v�ȃC���N���[�h
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADog::ADog()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// ACharacter�ɂ̓f�t�H���g��CapsuleComponent��Root
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// ���̌����ځi�G���W������Sphere���g���j
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	// SkeletalMesh���܂��Ȃ��̂ŁAStaticMesh�����ŃA�^�b�`

	UStaticMesh* TestMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	UStaticMeshComponent* DummyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMesh"));
	DummyMesh->SetStaticMesh(TestMesh);

	// Root�iCapsule�j�ɃA�^�b�`
	DummyMesh->SetupAttachment(RootComponent);

	// �ʒu������������i�n�ʂɖ��܂�Ȃ��悤�Ɂj
	DummyMesh->SetRelativeLocation(FVector(50.0f,50.0f, -50.0f));

	// �R���W�����𖳌��ɂ���iCapsule�������蔻��S���j
	DummyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Material��StaticMesh�ɐݒ肷��
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); // �}�E�X�N���b�N����p

	//HitEvennt��L���ɂ���
	GetCapsuleComponent()->BodyInstance.bNotifyRigidBodyCollision = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	// �����i�J���������̌��ɂǂꂭ�炢����邩�j
	SpringArm->TargetArmLength = 600.0f;  // 

	// �J�����̊p�x���������Ɍ�����
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	// �v���C���[�̃}�E�X����ŃJ���������Ȃ��悤�ɂ���
	SpringArm->bUsePawnControlRotation = false;

	// �J�����̃��O�ŃX���[�Y�ɒǏ]
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;

	// �J�����̃Z�b�g�A�b�v
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// �J������Pawn�̉�]���g��Ȃ��i�Œ������_�j
	Camera->bUsePawnControlRotation = false;

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

    //Click�A�N�V�����ǂݍ���
	ClickAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Click"));

	//Switch�A�N�V�����ǂݍ���
	SwitchAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Switch"));

	//Standerd�A�N�V�����ǂݍ���
	StanderdAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Standerd"));

	// CharacterMovementComponent �ɃW�����v�͂�ݒ�
	GetCharacterMovement()->JumpZVelocity = JumpForce;
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

	//�}�E�X�J�[�\���̗L����
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameAndUI());
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADog::Attack);
		// ControlBall��IA_Standerd��Triggered��Bind����
		EnhancedInputComponent->BindAction(StanderdAction, ETriggerEvent::Triggered, this, &ADog::LockOn);
		// ControlBall��IA_Jump��Triggered��Bind����
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADog::Jump);
		//Contro;Ball��IA_Click��Triggered��Bind����
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ADog::MoveToMousePoint);
		//Contro;Ball��IA_Switch��Triggered��Bind����
		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &ADog::Switch);
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
	if (IsChangeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("changeattack"));
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
}

void ADog::LockOn(const FInputActionValue& Value)
{
	FVector Playerlocation = GetActorLocation();
	AActor* BestNearTarget = nullptr;
}



//�W�����v����̏����֐�
void ADog::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));
	if (Value.Get<bool>())
	{
		//�W�����v
		ACharacter::Jump();
		UE_LOG(LogTemp, Warning, TEXT("Jump triggered!"));
	}
}

//�N���b�N�����瓮�����֐�
void ADog::MoveToMousePoint(const FInputActionValue& Value)
{
	if (!IsChangeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("changewalk"));
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (!PC)
		{
			return;
		}

		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			FVector TargetLocation = Hit.ImpactPoint;

			//NavMesh����ړ�
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), TargetLocation);

			UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *TargetLocation.ToString());
		}
	}
}

//���[�h�̐؂�ւ��֐�
void ADog::Switch(const FInputActionValue& Value)
{
	if (!IsChangeAttack)
	{
		IsChangeAttack = true;
		
	}
	else
	{
		IsChangeAttack = false;
		
	}
}

