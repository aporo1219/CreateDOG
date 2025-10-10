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
	
	//StaticMeshComponentを追加、RootComponentに設定
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaiticMeshComponent"));
	RootComponent = Sphere;

	// StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));

	// StaticMeshをStaticMeshComponentに設定する
	Sphere->SetStaticMesh(Mesh);

	// MaterialをStaticMeshに設定する
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	// MaterialをStaticMeshComponentに設定する
	Sphere->SetMaterial(0, Material);

	// Simulate Physicsを有効にする(物理シミュレーション）
	Sphere->SetSimulatePhysics(true);

	// SpringArmを追加する
	SpringArm = CreateDefaultSubobject < USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);

	// 角度を変更する FRotator(Pitch(Y), Yaw(Z), Roll(X))
	//SpringArm->SetRelativeRotation(FRotator(PitchArm, YawArm, RollArm));

	// Spring Armの長さを調整する
	SpringArm->TargetArmLength = ArmLength;

	// PawnのControllerRotationを使用する
	SpringArm->bUsePawnControlRotation = true;

	// CameraのLagを有効にする
	SpringArm->bEnableCameraLag = true;

	//Cameraの追加
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

    //InputMappingの読み込み
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/input/InputMappingContext"));

	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Look"));

	//発射位置
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	//高さ調整
	MuzzlePoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

    //Attackアクションの読み込み
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
		// ControlBallとIA_LookのTriggeredをBindする
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADog::Look);
		// ControlBallとIA_AttackのTriggeredをBindする
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADog::Attack);
		// ControlBallとIA_StanderdのTriggeredをBindする
		EnhancedInputComponent->BindAction(StanderdAction, ETriggerEvent::Triggered, this, &ADog::Standerd);
	}
}

void ADog::Look(const FInputActionValue& Value)
{
	// inputのValueはVector2Dに変換
	FVector2D v = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(v.X);
		AddControllerPitchInput(v.Y);

		//Dogが持っているControlの角度を取得
		FRotator controlRotate = GetControlRotation();

		// PlayerControllerの角度を設定する
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
	FRotator MuzzleRotation = Camera->GetComponentRotation(); // カメラの向きに発射

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// 弾丸を生成
	ABallActor* BallActor = GetWorld()->SpawnActor<ABallActor>(BallActorClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	if (BallActor)
	{
		BallActor->InitVelocity(MuzzleRotation.Vector());
	}
}

void ADog::Standerd(const FInputActionValue& Value)
{

}
