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
#include "Blueprint/AIBlueprintHelperLibrary.h"//NavMesh移動に必要なインクルード
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADog::ADog()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// ACharacterにはデフォルトでCapsuleComponentがRoot
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	RootComponent = GetCapsuleComponent();

	// 仮の見た目（エンジン内のSphereを使う）
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	//Mesh
	UStaticMesh* DogMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/model/newdog.newdog"));
	if (DogMesh)
	{
		// 新しいメッシュコンポーネントを作成
		UStaticMeshComponent* VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
		VisualMesh->SetStaticMesh(DogMesh);

		// ルート（CapsuleComponent）にアタッチ
		VisualMesh->SetupAttachment(GetCapsuleComponent());
		//大きさ変更
		VisualMesh->SetRelativeScale3D(FVector(MeshScale));
		// 向き・位置調整（モデルに合わせて調整OK）
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -AngleCon));
		VisualMesh->SetRelativeRotation(FRotator(0.0f, AngleCon, 0.0f));

		// 衝突はカプセルで扱うため、StaticMesh側は無効に
		VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 追加：メッシュで物理シミュレートがONになっていないことを保証
		VisualMesh->SetSimulatePhysics(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh: /Game/model/newdog.newdog"));
	}
	
	// MaterialをStaticMeshに設定する
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetCollisionObjectType(ECC_Pawn);

	// 全部無視ではなく、必要なものを個別に設定
	Capsule->SetCollisionResponseToAllChannels(ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	Capsule->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	// 距離（カメラが犬の後ろにどれくらい離れるか）
	SpringArm->TargetArmLength = 600.0f;  // 

	// カメラの角度を少し下に向ける
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	// プレイヤーのマウス操作でカメラが回らないようにする
	SpringArm->bUsePawnControlRotation = false;

	// カメラのラグでスムーズに追従
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;

	// カメラのセットアップ
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// カメラはPawnの回転を使わない（固定後方視点）
	Camera->bUsePawnControlRotation = false;

    //InputMappingの読み込み
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/input/InputMappingContext1"));

	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Look"));

	//発射位置
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	//高さ調整
	MuzzlePoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

    //Attackアクションの読み込み
	AttackAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Attack"));

	//Jumpアクション読み込み
	JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Jump"));

    //Clickアクション読み込み
	ClickAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Click"));

	//Switchアクション読み込み
	SwitchAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Switch"));

	//Standerdアクション読み込み
	StanderdAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Standerd"));

	// CharacterMovementComponent にジャンプ力を設定
	GetCharacterMovement()->JumpZVelocity = JumpForce;
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	//インップトマッピングの追加
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("MappingContext added!"));
		}
	}

	//マウスカーソルの有効化
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameAndUI());
	}

	//HPの初期化
	Health = MaxHealth;
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADog::Attack);
		// ControlBallとIA_StanderdのTriggeredをBindする
		EnhancedInputComponent->BindAction(StanderdAction, ETriggerEvent::Triggered, this, &ADog::LockOn);
		// ControlBallとIA_JumpのTriggeredをBindする
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADog::Jump);
		//Contro;BallとIA_ClickのTriggeredをBindする
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ADog::MoveToMousePoint);
		//Contro;BallとIA_SwitchのTriggeredをBindする
		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &ADog::Switch);
	}
}

//視点操作の処理関数
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

//攻撃操作の処理関数
void ADog::Attack(const FInputActionValue& Value)
{
	if (IsChangeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("changeattack"));
		//スポーン値
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//位置の設定
		FVector SpawnLoc = GetActorLocation() + GetActorForwardVector() * Spawnlocation;
		FRotator SpawnRot = GetActorRotation();

		ABallActor* Ball = GetWorld()->SpawnActor<ABallActor>(BallActorClass, SpawnLoc, SpawnRot, Params);

		if (Ball)
		{
			//発射方向をMuzzlePointの向き基準
			FVector ShootDir = MuzzlePoint->GetForwardVector();

			// 発射方向を渡す
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



//ジャンプ操作の処理関数
void ADog::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));
	if (Value.Get<bool>())
	{
		//ジャンプ
		ACharacter::Jump();
		UE_LOG(LogTemp, Warning, TEXT("Jump triggered!"));
	}
}

//クリックしたら動かす関数
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

			//NavMesh上を移動
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), TargetLocation);

			UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *TargetLocation.ToString());
		}
	}
}

//モードの切り替え関数
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

//ダメージ処理関数
void ADog::TakeDamege(float DamegeAmount)
{
	Health -= DamegeAmount;
	
	//ゲームオーバー処理
	if (Health < 0)
	{
		Health = 0;
		

		//UIへ通知
		OnHealthChanged.Broadcast();
	}

	//UIへ通知
	OnHealthChanged.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("Dog HP: %f"), Health);
}

